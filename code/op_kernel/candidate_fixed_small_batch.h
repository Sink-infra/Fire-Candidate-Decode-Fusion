#pragma once

template <typename T>
__aicore__ inline void ProcessFixedSmallBatch(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
    const FireCandidateDecodeFusionTilingData &data)
{
    const uint32_t tiles = (data.candidates + 7) / 8;
    const uint32_t task = uint32_t(GetBlockIdx());
    const uint32_t batch = task / tiles;
    const uint32_t row = (task - batch * tiles) * 8;
    const uint32_t offset = batch * data.candidates + row;
    const uint32_t remaining = data.candidates - row;
    if (remaining >= 8) {
        SingleTaskSmallCandidateDecode<T, 8, true, true, true> op;
        op.Init(raw + offset * 6 * sizeof(T), prior + row * 20,
            out + offset * 5 * sizeof(T), data);
        op.Process();
    } else if (remaining == 1) {
        SingleTaskSmallCandidateDecode<T, 1, false, true, true> op;
        op.Init(raw + offset * 6 * sizeof(T), prior + row * 20,
            out + offset * 5 * sizeof(T), data);
        op.Process();
    } else {
        SmallCandidateDecode<T, 8, false, true> op;
        op.Init(raw, prior, out, data);
        op.Process();
    }
}
