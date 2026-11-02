#pragma once

template <uint32_t Tile, uint32_t Pitch, uint32_t BatchGroup, bool StaticEvents>
__aicore__ inline void BuildNativeSixCandidateIndices(LocalTensor<int32_t> sequence,
    LocalTensor<int32_t> scratch, LocalTensor<int32_t> priorIndex,
    LocalTensor<int32_t> outputIndex, TPipe *pipe)
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t Quarter = Rows / 4;
    static_assert(Rows % 32 == 0);
    BuildDoublingCandidateIndex<320, StaticEvents>(sequence, sequence[320], pipe);
    constexpr uint32_t PriorBaseRows = Tile / BatchGroup / 4;
    static_assert(PriorBaseRows % 8 == 0);
    Muls(priorIndex, sequence, int32_t(80), PriorBaseRows);
    PipeBarrier<PIPE_V>();
    if constexpr (BatchGroup > 2) {
        for (uint32_t j = 1; j < BatchGroup / 2; ++j)
            DataCopy(priorIndex[j * PriorBaseRows], priorIndex, PriorBaseRows);
        PipeBarrier<PIPE_V>();
    }
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

#include "candidate_index_ramp.h"

template <uint32_t Tile, uint32_t BatchGroup, bool StaticEvents>
__aicore__ inline void BuildNativeEightCandidateIndices(LocalTensor<int32_t> sequence,
    LocalTensor<int32_t> scratch, LocalTensor<int32_t> priorIndex, TPipe *pipe)
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t Quarter = Rows / 8;
    constexpr uint32_t BatchRows = Tile / BatchGroup;
    constexpr uint32_t BaseRows = BatchRows / 8;
    static_assert((BatchGroup == 2 || BatchGroup == 4) && Rows % 64 == 0 && Quarter % 8 == 0);
    constexpr uint32_t SeedRows = ((Quarter + 63) / 64) * 64;
    BuildDoublingCandidateIndex<SeedRows, StaticEvents>(sequence, sequence[SeedRows], pipe);
    if constexpr (BatchGroup == 4) {
    Muls(scratch, sequence, int32_t((65536 + BaseRows - 1) / BaseRows), Quarter);
    PipeBarrier<PIPE_V>();
    ShiftRight(scratch, scratch, int32_t(16), Quarter);
    Muls(priorIndex, sequence, int32_t(160), Quarter);
    PipeBarrier<PIPE_V>();
    Muls(scratch, scratch, -int32_t(BatchRows * 20), Quarter);
    PipeBarrier<PIPE_V>();
    Add(priorIndex, priorIndex, scratch, Quarter);
    } else {
        Muls(priorIndex, sequence, int32_t(160), Quarter);
    }
    PipeBarrier<PIPE_V>();
    #pragma unroll
    for (uint32_t j = 1; j < 8; ++j)
        Adds(priorIndex[j * Quarter], priorIndex, int32_t(j * 20), Quarter);
    PipeBarrier<PIPE_V>();
}

template <uint32_t Tile, uint32_t Pitch, uint32_t SourceAddr,
    uint32_t ColumnAddr, uint32_t Phase>
__aicore__ inline void NativeEightCandidatePhase()
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t Quarter = Rows / 8;
    uint64_t sources[16], targets[16];
    #pragma unroll
    for (uint32_t j = 0; j < 8; ++j) {
        const uint32_t field = (Phase * 8 + j) % 6;
        const uint32_t subrow = (Phase * 8 + j) / 6;
        sources[j] = SourceAddr + j * 192 + Phase * 32;
        sources[j + 8] = sources[j] + Rows * 24;
        targets[j * 2] = ColumnAddr + (field * Pitch + subrow * Quarter) * 4;
        targets[j * 2 + 1] = targets[j * 2] + Rows * 4;
    }
    TransDataTo5HD<float>(targets, sources,
        TransDataTo5HDParams{false, false, uint8_t(Rows / 64),
            uint16_t(Rows == 64 ? 0 : 1), uint16_t(Rows == 64 ? 0 : 48)});
}

template <uint32_t Tile, uint32_t Pitch, uint32_t SourceAddr, uint32_t ColumnAddr>
__aicore__ inline void GatherNativeEightCandidates()
{
    NativeEightCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 0>();
    NativeEightCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 1>();
    NativeEightCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 2>();
    NativeEightCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 3>();
    NativeEightCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 4>();
    NativeEightCandidatePhase<Tile, Pitch, SourceAddr, ColumnAddr, 5>();
}

template <uint32_t Tile, uint32_t Pitch, uint32_t ColumnAddr,
    uint32_t PackedAddr, uint32_t Phase, bool FullStripe = false>
__aicore__ inline void NativeFiveCandidatePhase()
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t Quarter = FullStripe ? Tile / 8 : Rows / 8;
    uint64_t sources[16], targets[16];
    #pragma unroll
    for (uint32_t j = 0; j < 8; ++j) {
        const uint32_t field = (Phase * 8 + j) % 5;
        const uint32_t subrow = (Phase * 8 + j) / 5;
        sources[j] = ColumnAddr + (field * Pitch + subrow * Quarter) * 4;
        sources[j + 8] = sources[j] + (FullStripe ? Tile / 16 : Rows) * 4;
        targets[j * 2] = PackedAddr + j * 160 + Phase * 32;
        targets[j * 2 + 1] = targets[j * 2] + Rows * 20;
    }
    TransDataTo5HD<float>(targets, sources,
        TransDataTo5HDParams{false, false, uint8_t(Rows / 64),
            uint16_t(Rows == 64 ? 0 : 40), uint16_t(Rows == 64 ? 0 : 1)});
}

template <uint32_t Tile, uint32_t Pitch, uint32_t ColumnAddr, uint32_t PackedAddr, bool FullStripe = false>
__aicore__ inline void PackNativeFiveCandidates()
{
    NativeFiveCandidatePhase<Tile, Pitch, ColumnAddr, PackedAddr, 0, FullStripe>();
    NativeFiveCandidatePhase<Tile, Pitch, ColumnAddr, PackedAddr, 1, FullStripe>();
    NativeFiveCandidatePhase<Tile, Pitch, ColumnAddr, PackedAddr, 2, FullStripe>();
    NativeFiveCandidatePhase<Tile, Pitch, ColumnAddr, PackedAddr, 3, FullStripe>();
    NativeFiveCandidatePhase<Tile, Pitch, ColumnAddr, PackedAddr, 4, FullStripe>();
}
