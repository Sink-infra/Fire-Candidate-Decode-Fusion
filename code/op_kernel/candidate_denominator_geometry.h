#pragma once

template <uint32_t Pitch, uint32_t Tile>
__aicore__ inline void DecodeCandidateGeometry(LocalTensor<float> c, uint32_t count)
{
    if (count * 2 <= Tile) {
        for (uint32_t j = 0; j < 4; ++j)
            Muls(c[j * Pitch], c[j * Pitch], 2.0f, count);
        PipeBarrier<PIPE_V>();
        for (uint32_t j = 0; j < 2; ++j)
            Adds(c[j * Pitch], c[j * Pitch], -0.5f, count);
        Mul(c[2 * Pitch], c[2 * Pitch], c[2 * Pitch], count);
        Mul(c[3 * Pitch], c[3 * Pitch], c[3 * Pitch], count);
        Mul(c[4 * Pitch], c[4 * Pitch], c[5 * Pitch], count);
        PipeBarrier<PIPE_V>();
        Add(c, c, c[6 * Pitch], count);
        Add(c[Pitch], c[Pitch], c[7 * Pitch], count);
        Mul(c[2 * Pitch], c[2 * Pitch], c[9 * Pitch], count);
        Mul(c[3 * Pitch], c[3 * Pitch], c[10 * Pitch], count);
        PipeBarrier<PIPE_V>();
    } else {
        Muls(c, c, 2.0f, Pitch * 4);
        PipeBarrier<PIPE_V>();
        Adds(c, c, -0.5f, Pitch * 2);
        Mul(c[2 * Pitch], c[2 * Pitch], c[2 * Pitch], Pitch * 2);
        Mul(c[4 * Pitch], c[4 * Pitch], c[5 * Pitch], count);
        PipeBarrier<PIPE_V>();
        Add(c, c, c[6 * Pitch], Pitch * 2);
        Mul(c[2 * Pitch], c[2 * Pitch], c[9 * Pitch], Pitch * 2);
        PipeBarrier<PIPE_V>();
    }
    for (uint32_t j = 0; j < 4; ++j)
        Mul(c[j * Pitch], c[j * Pitch], c[8 * Pitch], count);
    PipeBarrier<PIPE_V>();
}

template <uint32_t Tile, uint32_t Pitch, bool FastConfidence = false, bool ReplicatedStride = false, bool TransformedPriors = false>
__aicore__ inline void DecodeCandidateDenominators(LocalTensor<float> columns,
    LocalTensor<float> two, LocalTensor<float> one, LocalTensor<float> firstStride)
{
    static_assert(Tile >= 1024 && Tile % 64 == 0 && (Pitch == Tile + 8 || Pitch == Tile + 16));
    SetMaskCount();
    SetVectorMask<float, MaskMode::COUNTER>(Tile);
    Mul<float, false>(columns[4 * Pitch], columns[4 * Pitch], columns[5 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    SetVectorMask<float, MaskMode::COUNTER>(Pitch * 4);
    Div<float, false>(columns, two, columns, MASK_PLACEHOLDER, 1, {1, 0, 1, 8, 0, 8});
    SetVectorMask<float, MaskMode::COUNTER>(Tile);
    if constexpr (FastConfidence) {
        Reciprocal<float, false>(columns[4 * Pitch], columns[4 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 8, 8});
    } else {
        Div<float, false>(columns[4 * Pitch], one, columns[4 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 0, 1, 8, 0, 8});
    }
    SetVectorMask<float, MaskMode::COUNTER>(Pitch * 2);
    if constexpr (TransformedPriors) {
        Mul<float, false>(columns, columns, firstStride,
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[2 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        Add<float, false>(columns, columns, columns[6 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[9 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    } else {
    Adds<float, false>(columns, columns, -0.5f, MASK_PLACEHOLDER, 1, {1, 1, 8, 8});
    Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[2 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    Add<float, false>(columns, columns, columns[6 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[9 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    if constexpr (ReplicatedStride) {
        SetVectorMask<float, MaskMode::COUNTER>(Pitch * 4);
        Mul<float, false>(columns, columns, firstStride,
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    } else {
        SetVectorMask<float, MaskMode::COUNTER>(Tile);
        Mul<float, false>(columns, columns, firstStride,
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        for (uint32_t field = 1; field < 4; ++field)
            Mul<float, false>(columns[field * Pitch], columns[field * Pitch], columns[8 * Pitch],
                MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    }
    }
    SetMaskNorm();
    ResetMask();
}

template <uint32_t Tile, uint32_t Pitch, bool FastConfidence = false>
__aicore__ inline void DecodeCandidateDenominators(LocalTensor<float> columns,
    LocalTensor<float> two, LocalTensor<float> one)
{
    DecodeCandidateDenominators<Tile, Pitch, FastConfidence>(columns, two, one, columns[8 * Pitch]);
}
