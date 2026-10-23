#pragma once

template <typename T, uint32_t Tile>
__aicore__ inline void RunNativeLargeCandidate(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    TPipe pipe;
    PairedBatchCandidateDecode<T, Tile, false, true,
        sizeof(T) == 4, false, true, false, 2, true, true> op;
    op.Init(raw, prior, out, data, &pipe);
    op.Process();
}
