#pragma once

template <typename T, bool Adaptive>
__aicore__ inline void RunFourBatchMediumCandidate(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    InitSocState();
    if constexpr (!Adaptive) {
        PairedBatchCandidateDecode<T, 512, false, false, true, false, true, true, 4> op;
        op.Init(raw, prior, out, data, nullptr);
        op.Process();
    } else {
        const uint32_t groups = (data.candidates + 15) / 16;
        const uint32_t begin = (groups * GetBlockIdx() / data.cores) * 16;
        uint32_t end = (groups * (GetBlockIdx() + 1) / data.cores) * 16;
        if (end > data.candidates) end = data.candidates;
        if (end - begin <= 128) {
            PairedBatchCandidateDecode<T, 512, false, false, true, true, true, true, 4> op;
            op.Init(raw, prior, out, data, nullptr);
            op.Process();
        } else {
            PairedBatchCandidateDecode<T, 640, true, false, true, true, true, true, 4> op;
            op.Init(raw, prior, out, data, nullptr);
            op.Process();
        }
    }
}
