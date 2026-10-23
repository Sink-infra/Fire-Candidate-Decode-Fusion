#pragma once

template <typename T>
__aicore__ inline void RunCoarseMediumCandidate(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    InitSocState();
    const uint32_t groups = (data.candidates + 15) / 16;
    const uint32_t begin = (groups * GetBlockIdx() / data.cores) * 16;
    uint32_t end = (groups * (GetBlockIdx() + 1) / data.cores) * 16;
    if (end > data.candidates) end = data.candidates;
    if (end - begin <= 256) {
        PairedBatchCandidateDecode<T, 1024, false, false, true, true, true, true, 4, true, true, true, true> op;
        op.Init(raw, prior, out, data, nullptr);
        op.Process();
    } else {
        PairedBatchCandidateDecode<T, 1280, false, false, true, true, true, true, 4, true, true, true, true> op;
        op.Init(raw, prior, out, data, nullptr);
        op.Process();
    }
}
