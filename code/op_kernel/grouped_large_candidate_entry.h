#pragma once

template <typename T, uint32_t Tile, bool Preload, bool NativePrior>
__aicore__ inline void RunLargeCandidatePath(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    TPipe pipe;
    PairedBatchCandidateDecode<T, Tile, false, true,
        sizeof(T) == 4, false, true, false, 2, true, true, Preload, NativePrior, false, sizeof(T) == 2 && NativePrior> op;
    op.Init(raw, prior, out, data, &pipe);
    op.Process();
}

template <typename T, uint32_t Tile>
__aicore__ inline void RunNativeLargeCandidate(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    if constexpr (sizeof(T) == 4) {
        if (data.candidates % 16 != 0) {
            RunLargeCandidatePath<T, Tile, false, false>(raw, prior, out, data);
            return;
        }
    }
    RunLargeCandidatePath<T, Tile, true, true>(raw, prior, out, data);
}
