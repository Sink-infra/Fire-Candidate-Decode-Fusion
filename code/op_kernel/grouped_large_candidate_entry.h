#pragma once

template <typename T, uint32_t Tile, bool Preload, bool NativePrior, bool AlignedTransfers = false>
__aicore__ inline void RunLargeCandidatePath(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    InitSocState();
    PairedBatchCandidateDecode<T, Tile, false, true,
        sizeof(T) == 4, false, true, true, 2, true, true, Preload, NativePrior, true, sizeof(T) == 2 && NativePrior, AlignedTransfers, true> op;
    op.Init(raw, prior, out, data, nullptr);
    op.Process();
}

template <typename T, uint32_t Tile>
__aicore__ inline void RunNativeLargeCandidate(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    if (data.candidates % 16 == 0) {
        RunLargeCandidatePath<T, Tile, true, true, true>(raw, prior, out, data);
        return;
    }
    if constexpr (sizeof(T) == 4) {
        if (data.candidates % 16 != 0) {
            RunLargeCandidatePath<T, Tile, false, false>(raw, prior, out, data);
            return;
        }
    }
    RunLargeCandidatePath<T, Tile, true, true>(raw, prior, out, data);
}
