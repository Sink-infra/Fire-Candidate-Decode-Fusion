#pragma once

template <uint32_t Rows>
__aicore__ inline void ClearCandidatePriorTail(LocalTensor<float> prior, uint32_t count)
{
    if (count == Rows) return;
    const uint32_t valid = count * 5;
    const uint32_t begin = valid / 8 * 8;
    const uint32_t remaining = Rows * 5 - begin;
    const uint32_t first = remaining < 64 ? remaining : 64;
    const uint64_t active = first == 64 ? ~uint64_t(0) : (uint64_t(1) << first) - 1;
    uint64_t mask[2] = {active & ~((uint64_t(1) << (valid - begin)) - 1), 0};
    Duplicate(prior[begin], 0.0f, mask, uint8_t(1), uint16_t(1), uint8_t(8));
    if (remaining > 64) Duplicate(prior[begin + 64], 0.0f, remaining - 64);
    PipeBarrier<PIPE_V>();
}

template <uint32_t Tile, uint32_t Pitch, uint32_t Rows>
__aicore__ inline bool CanTransformCandidatePriors(LocalTensor<float> prior,
    LocalTensor<float> columns, LocalTensor<float> result,
    LocalTensor<float> temporary)
{
    constexpr uint32_t PriorBytes = ((Rows * 5 / 8 + 31) / 32) * 32;
    constexpr uint32_t StrideBytes = ((Tile / 8 + 31) / 32) * 32;
    constexpr uint32_t Words = Rows * 5 / 32;
    constexpr uint32_t Repeats = (Words + 63) / 64;
    static_assert(Rows * 5 % 64 == 0 && Tile % 64 == 0);
    auto bytes = temporary.template ReinterpretCast<uint8_t>();
    auto priorMask = bytes;
    auto strideMask = bytes[PriorBytes];
    auto flags = bytes[PriorBytes + StrideBytes].template ReinterpretCast<float>();
    Duplicate(flags, 0.0f, Repeats * 64);
    PipeBarrier<PIPE_V>();
    CompareScalar(priorMask, prior, 1.0e18f, CMPMODE::GT, Rows * 5);
    CompareScalar(strideMask, columns[8 * Pitch], 32.0f, CMPMODE::GT, Tile);
    PipeBarrier<PIPE_V>();
    Or(priorMask.template ReinterpretCast<uint16_t>(),
        priorMask.template ReinterpretCast<uint16_t>(),
        strideMask.template ReinterpretCast<uint16_t>(), Tile / 16);
    PipeBarrier<PIPE_V>();
    Cast(flags, priorMask.template ReinterpretCast<int32_t>(), RoundMode::CAST_NONE, Words);
    Abs(flags, flags, Repeats * 64);
    WholeReduceMax(result[8], flags, 64, Repeats, 1, 1, 8,
        ReduceOrder::ORDER_ONLY_VALUE);
    PipeBarrier<PIPE_V>();
    WholeReduceMax(result, result[8], Repeats, 1, 1, 1, 8,
        ReduceOrder::ORDER_ONLY_VALUE);
    SetFlag<HardEvent::V_S>(0);
    WaitFlag<HardEvent::V_S>(0);
    return result.GetValue(0) == 0.0f;
}

template <uint32_t Pitch>
__aicore__ inline void TransformCandidatePriors(LocalTensor<float> columns,
    LocalTensor<float> stride)
{
    Adds(columns[6 * Pitch], columns[6 * Pitch], -0.5f, Pitch * 2);
    Mul(columns[6 * Pitch], columns[6 * Pitch], stride, Pitch * 2);
    Mul(columns[9 * Pitch], columns[9 * Pitch], stride, Pitch * 2);
}
