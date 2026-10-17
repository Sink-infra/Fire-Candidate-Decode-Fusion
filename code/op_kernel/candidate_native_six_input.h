#pragma once

template <uint32_t Tile, uint32_t Pitch, bool StaticEvents>
__aicore__ inline void BuildNativeSixCandidateIndices(LocalTensor<int32_t> sequence,
    LocalTensor<int32_t> scratch, LocalTensor<int32_t> priorIndex,
    LocalTensor<int32_t> outputIndex, TPipe *pipe)
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t Quarter = Rows / 4;
    static_assert(Rows % 32 == 0);
    BuildDoublingCandidateIndex<320, StaticEvents>(sequence, sequence[320], pipe);
    Muls(priorIndex, sequence, int32_t(80), Quarter);
    PipeBarrier<PIPE_V>();
    for (uint32_t j = 1; j < 4; ++j)
        Adds(priorIndex[j * Quarter], priorIndex, int32_t(j * 20), Quarter);
    PipeBarrier<PIPE_V>();
    auto row = scratch;
    auto quarter = scratch[320];
    Muls(row, sequence, int32_t(13108), 320);
    PipeBarrier<PIPE_V>();
    ShiftRight(row, row, int32_t(16), 320);
    PipeBarrier<PIPE_V>();
    ShiftRight(quarter, row, int32_t(2), 320);
    Muls(sequence, sequence, int32_t(4 * Pitch), 320);
    PipeBarrier<PIPE_V>();
    Muls(row, row, int32_t(4 * Quarter - 20 * Pitch), 320);
    Muls(quarter, quarter, int32_t(4 - 16 * Quarter), 320);
    PipeBarrier<PIPE_V>();
    Add(outputIndex, sequence, row, 320);
    PipeBarrier<PIPE_V>();
    Add(outputIndex, outputIndex, quarter, 320);
    PipeBarrier<PIPE_V>();
    for (uint32_t filled = 320; filled < Rows * 5; filled *= 2) {
        const uint32_t count = filled * 2 <= Rows * 5 ? filled : Rows * 5 - filled;
        Adds(outputIndex[filled], outputIndex, int32_t(filled / 5), count);
        PipeBarrier<PIPE_V>();
    }
    Adds(outputIndex[Rows * 5], outputIndex, int32_t(Rows * 4), Rows * 5);
    PipeBarrier<PIPE_V>();
}

template <uint32_t Tile, uint32_t Pitch, uint32_t SourceAddr,
    uint32_t ColumnAddr, uint32_t Phase>
__aicore__ inline void NativeSixCandidatePhase()
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t Quarter = Rows / 4;
    uint64_t sources[16], targets[16];
    #pragma unroll
    for (uint32_t j = 0; j < 8; ++j) {
        const uint32_t field = (Phase * 8 + j) % 6;
        const uint32_t subrow = (Phase * 8 + j) / 6;
        sources[j] = SourceAddr + j * 96 + Phase * 32;
        sources[j + 8] = sources[j] + Rows * 24;
        targets[j * 2] = ColumnAddr + (field * Pitch + subrow * Quarter) * 4;
        targets[j * 2 + 1] = targets[j * 2] + Rows * 4;
    }
    TransDataTo5HD<float>(targets, sources,
        TransDataTo5HDParams{false, false, uint8_t(Rows / 32), 1, 24});
}

template <uint32_t Tile, uint32_t Pitch, uint32_t SourceAddr, uint32_t ColumnAddr>
__aicore__ inline void GatherNativeSixCandidates()
{
    NativeSixCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 0>();
    NativeSixCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 1>();
    NativeSixCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 2>();
}
