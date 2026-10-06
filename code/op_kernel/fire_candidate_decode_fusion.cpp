#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"
using namespace AscendC;
template <typename T>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t Tile = 512;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &d)
    {
        batch_ = d.batch; n_ = d.candidates; cores_ = d.cores;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        pipe_.InitBuffer(inRaw_, 1, Tile * 6 * sizeof(T));
        pipe_.InitBuffer(inPrior_, 1, (Tile * 5 + 8) * 4);
        pipe_.InitBuffer(outQueue_, 1, Tile * 5 * sizeof(T));
        pipe_.InitBuffer(rawFloat_, Tile * 6 * 4);
        pipe_.InitBuffer(priorExpanded_, Tile * 6 * 4);
        pipe_.InitBuffer(scratch_, Tile * 6 * 4);
        pipe_.InitBuffer(packed_, Tile * 5 * 4);
        pipe_.InitBuffer(ones_, 64 * 4);
        pipe_.InitBuffer(gridIndex_, Tile * 6 * 4);
        pipe_.InitBuffer(strideIndex_, Tile * 6 * 4);
        pipe_.InitBuffer(packIndex_, Tile * 5 * 4);
        pipe_.InitBuffer(fireIndex_, Tile * 5 * 4);
        auto seq = rawFloat_.Get<float>();
        auto row = scratch_.Get<float>();
        auto gi = gridIndex_.Get<int32_t>();
        auto si = strideIndex_.Get<int32_t>();
        CreateVecIndex(seq, 0.0f, Tile * 6);
        PipeBarrier<PIPE_V>();
        Muls(row, seq, 1.0f / 6.0f, Tile * 6);
        PipeBarrier<PIPE_V>();
        Cast(gi, row, RoundMode::CAST_FLOOR, Tile * 6);
        PipeBarrier<PIPE_V>();
        Cast(row, gi, RoundMode::CAST_NONE, Tile * 6);
        PipeBarrier<PIPE_V>();
        Sub(seq, seq, row, Tile * 6);
        Muls(row, row, 20.0f, Tile * 6);
        PipeBarrier<PIPE_V>();
        uint64_t wh[2] = {0x30c30c30c30cULL, 0};
        Adds(seq, seq, 1.0f, wh, uint8_t(Tile / 8), {1, 1, 6, 6});
        Adds(row, row, 8.0f, Tile * 6);
        PipeBarrier<PIPE_V>();
        Muls(seq, seq, 4.0f, Tile * 6);
        Cast(si, row, RoundMode::CAST_RINT, Tile * 6);
        PipeBarrier<PIPE_V>();
        Cast(gi, seq, RoundMode::CAST_RINT, Tile * 6);
        auto pack = packed_.Get<float>();
        auto pi = packIndex_.Get<int32_t>();
        auto fi = fireIndex_.Get<int32_t>();
        CreateVecIndex(pack, 0.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(row, pack, 0.2f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(pi, row, RoundMode::CAST_FLOOR, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(row, pi, RoundMode::CAST_NONE, Tile * 5);
        PipeBarrier<PIPE_V>();
        Add(pack, pack, row, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(pack, pack, 4.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(pi, pack, RoundMode::CAST_RINT, Tile * 5);
        PipeBarrier<PIPE_V>();
        Adds(fi, pi, int32_t(4), Tile * 5);
        Duplicate(ones_.Get<float>(), 1.0f, 64);
        PipeBarrier<PIPE_V>();
    }
    __aicore__ inline void Process()
    {
        const uint32_t tiles = (n_ + Tile - 1) / Tile;
        for (uint32_t task = GetBlockIdx(); task < batch_ * tiles; task += cores_) {
            const uint32_t b = task / tiles;
            const uint32_t start = (task % tiles) * Tile;
            const uint32_t count = start + Tile <= n_ ? Tile : n_ - start;
            auto raw = inRaw_.AllocTensor<T>();
            auto prior = inPrior_.AllocTensor<float>();
            DataCopyPad(raw, raw_[(b * n_ + start) * 6],
                DataCopyExtParams{1, count * 6 * uint32_t(sizeof(T)), 0, 0, 0},
                DataCopyPadExtParams<T>{false, 0, 0, 0});
            DataCopyPad(prior, prior_[start * 5],
                DataCopyExtParams{1, count * 20, 0, 0, 0},
                DataCopyPadExtParams<float>{false, 0, 0, 0});
            inRaw_.EnQue(raw); inPrior_.EnQue(prior);
            raw = inRaw_.DeQue<T>(); prior = inPrior_.DeQue<float>();
            auto sig = rawFloat_.Get<float>();
            if constexpr (sizeof(T) == 2)
                Cast(sig, raw, RoundMode::CAST_NONE, count * 6);
            else
                DataCopy(sig, raw, ((count * 6 + 7) / 8) * 8);
            PipeBarrier<PIPE_V>();
            Muls(sig, sig, -1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Exp(sig, sig, count * 6);
            PipeBarrier<PIPE_V>();
            Adds(sig, sig, 1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Div(sig, ones_.Get<float>(), sig, uint64_t(64),
                uint8_t((count * 6 + 63) / 64), {1, 1, 1, 8, 0, 8});
            PipeBarrier<PIPE_V>();
            auto pg = priorExpanded_.Get<float>();
            auto tmp = scratch_.Get<float>();
            Gather(pg, prior, gridIndex_.Get<uint32_t>(), 0, count * 6);
            Gather(tmp, prior, strideIndex_.Get<uint32_t>(), 0, count * 6);
            PipeBarrier<PIPE_V>();
            uint64_t xy[2] = {0xc30c30c30c3ULL, 0};
            uint64_t wh[2] = {0x30c30c30c30cULL, 0};
            uint64_t box[2] = {0x3cf3cf3cf3cfULL, 0};
            const uint8_t repeats = (count + 7) / 8;
            Muls(sig, sig, 2.0f, box, repeats, {1, 1, 6, 6});
            PipeBarrier<PIPE_V>();
            Adds(sig, sig, -0.5f, xy, repeats, {1, 1, 6, 6});
            Mul(sig, sig, sig, wh, repeats, {1, 1, 1, 6, 6, 6});
            PipeBarrier<PIPE_V>();
            Add(sig, sig, pg, xy, repeats, {1, 1, 1, 6, 6, 6});
            Mul(sig, sig, pg, wh, repeats, {1, 1, 1, 6, 6, 6});
            PipeBarrier<PIPE_V>();
            Mul(sig, sig, tmp, box, repeats, {1, 1, 1, 6, 6, 6});
            PipeBarrier<PIPE_V>();
            auto packed = packed_.Get<float>();
            Gather(packed, sig, packIndex_.Get<uint32_t>(), 0, count * 5);
            Gather(tmp, sig, fireIndex_.Get<uint32_t>(), 0, count * 5);
            PipeBarrier<PIPE_V>();
            uint64_t score[2] = {0x8421084210ULL, 0};
            Mul(packed, packed, tmp, score, repeats, {1, 1, 1, 5, 5, 5});
            PipeBarrier<PIPE_V>();
            auto dst = outQueue_.AllocTensor<T>();
            if constexpr (sizeof(T) == 2)
                Cast(dst, packed, RoundMode::CAST_RINT, count * 5);
            else
                DataCopy(dst, packed, ((count * 5 + 7) / 8) * 8);
            outQueue_.EnQue(dst); dst = outQueue_.DeQue<T>();
            DataCopyPad(out_[(b * n_ + start) * 5], dst,
                DataCopyExtParams{1, count * 5 * uint32_t(sizeof(T)), 0, 0, 0});
            outQueue_.FreeTensor(dst); inRaw_.FreeTensor(raw); inPrior_.FreeTensor(prior);
        }
    }
private:
    TPipe pipe_;
    TQue<QuePosition::VECIN, 1> inRaw_, inPrior_;
    TQue<QuePosition::VECOUT, 1> outQueue_;
    TBuf<TPosition::VECCALC> rawFloat_, priorExpanded_, scratch_, packed_, ones_;
    TBuf<TPosition::VECCALC> gridIndex_, strideIndex_, packIndex_, fireIndex_;
    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint32_t batch_, n_, cores_;
};
template <typename DT_RAW_PRED>
__global__ __aicore__ void fire_candidate_decode_fusion(GM_ADDR raw_pred,
    GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling)
{
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, data, tiling);
    KernelFireCandidateDecodeFusion<DT_RAW_PRED> op;
    op.Init(raw_pred, priors, decoded, data);
    op.Process();
}
