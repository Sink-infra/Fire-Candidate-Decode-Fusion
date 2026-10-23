#pragma once

template <uint32_t Tile, uint32_t Pitch, uint32_t PriorAddr,
    uint32_t ColumnAddr, uint32_t Phase>
__aicore__ inline void NativePriorCandidatePhase()
{
    constexpr uint32_t BatchRows = Tile / 4;
    constexpr uint32_t Quarter = BatchRows / 8;
    static_assert(BatchRows % 64 == 0);
    uint64_t sources[16], targets[16];
    #pragma unroll
    for (uint32_t j = 0; j < 8; ++j) {
        const uint32_t field = (Phase * 8 + j) % 5;
        const uint32_t subrow = (Phase * 8 + j) / 5;
        sources[j] = PriorAddr + j * 160 + Phase * 32;
        sources[j + 8] = sources[j];
        targets[j * 2] = ColumnAddr + ((field + 6) * Pitch + subrow * Quarter * 2) * 4;
        targets[j * 2 + 1] = targets[j * 2] + Quarter * 4;
    }
    TransDataTo5HD<float>(targets, sources,
        TransDataTo5HDParams{false, false, uint8_t(BatchRows / 64), 1, 40});
}

template <uint32_t Tile, uint32_t Pitch, uint32_t PriorAddr, uint32_t ColumnAddr>
__aicore__ inline void GatherNativePriorCandidates()
{
    NativePriorCandidatePhase<Tile, Pitch, PriorAddr, ColumnAddr, 0>();
    NativePriorCandidatePhase<Tile, Pitch, PriorAddr, ColumnAddr, 1>();
    NativePriorCandidatePhase<Tile, Pitch, PriorAddr, ColumnAddr, 2>();
    NativePriorCandidatePhase<Tile, Pitch, PriorAddr, ColumnAddr, 3>();
    NativePriorCandidatePhase<Tile, Pitch, PriorAddr, ColumnAddr, 4>();
}
