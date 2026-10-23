#pragma once

template <typename T, bool FuseConfidence = false>
__aicore__ inline void RunCoarseMediumCandidate(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    InitSocState();
    const uint32_t groups = (data.candidates + 15) / 16;
    const uint32_t left = groups * GetBlockIdx();
    const uint32_t right = groups * (GetBlockIdx() + 1);
    const uint32_t begin = (data.segments < 32 ? left >> data.segments : left / data.cores) * 16;
    uint32_t end = (data.segments < 32 ? right >> data.segments : right / data.cores) * 16;
    if (end > data.candidates) end = data.candidates;
    if (end - begin <= 256) {
        PairedBatchCandidateDecode<T, 1024, false, false, true, true, true, true, 4, true, true, true, true, FuseConfidence, sizeof(T) == 2> op;
        op.Init(raw, prior, out, data, nullptr, begin, end);
        op.Process();
    } else {
        PairedBatchCandidateDecode<T, 1280, false, false, true, true, true, true, 4, true, true, true, true, FuseConfidence, sizeof(T) == 2> op;
        op.Init(raw, prior, out, data, nullptr, begin, end);
        op.Process();
    }
}
