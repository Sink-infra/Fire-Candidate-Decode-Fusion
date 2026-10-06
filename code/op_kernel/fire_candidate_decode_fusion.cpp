#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"
using namespace AscendC;

template <typename T>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t Tile = 256;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &d)
    {
        batch_ = d.batch; n_ = d.candidates; cores_ = d.cores;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        pipe_.InitBuffer(inRaw_, 1, Tile * 6 * sizeof(T));
        pipe_.InitBuffer(inPrior_, 1, Tile * 8 * 4);
        pipe_.InitBuffer(outQueue_, 1, Tile * (sizeof(T) == 2 ? 16 : 8) * sizeof(T));
        pipe_.InitBuffer(rawFloat_, (Tile * 6 + 8) * 4);
        pipe_.InitBuffer(rows_, Tile * 8 * 4);
        pipe_.InitBuffer(scratch_, Tile * 16 * 4);
        pipe_.InitBuffer(ones_, Tile * 6 * 4);
        pipe_.InitBuffer(rowIndex_, Tile * 4);
        pipe_.InitBuffer(expandIndex_, Tile * 8 * 4);
        auto idx = rowIndex_.Get<int32_t>();
        CreateVecIndex(idx, int32_t(0), Tile);
        PipeBarrier<PIPE_V>();
        Muls(idx, idx, int32_t(32), Tile);
        auto seq = scratch_.Get<float>();
        auto row = scratch_.Get<float>()[Tile * 8];
        auto map = expandIndex_.Get<int32_t>();
        CreateVecIndex(seq, 0.0f, Tile * 8);
        PipeBarrier<PIPE_V>();
        Muls(row, seq, 0.125f, Tile * 8);
        PipeBarrier<PIPE_V>();
        Cast(map, row, RoundMode::CAST_FLOOR, Tile * 8);
        PipeBarrier<PIPE_V>();
        Cast(row, map, RoundMode::CAST_NONE, Tile * 8);
        PipeBarrier<PIPE_V>();
        Muls(row, row, 2.0f, Tile * 8);
        PipeBarrier<PIPE_V>();
        Sub(seq, seq, row, Tile * 8);
        PipeBarrier<PIPE_V>();
        Muls(seq, seq, 4.0f, Tile * 8);
        PipeBarrier<PIPE_V>();
        Cast(map, seq, RoundMode::CAST_RINT, Tile * 8);
        Duplicate(ones_.Get<float>(), 1.0f, Tile * 6);
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
                DataCopyExtParams{uint16_t(count), 20, 0, 0, 0},
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
            Div(sig, ones_.Get<float>(), sig, count * 6);
            PipeBarrier<PIPE_V>();
            auto r = rows_.Get<float>();
            Gather(r, sig, expandIndex_.Get<uint32_t>(), 0, count * 8);
            auto tmp = scratch_.Get<float>();
            const auto idx = rowIndex_.Get<uint32_t>();
            auto obj = tmp;
            auto fire = tmp[Tile];
            auto stride = tmp[Tile * 2];
            Gather(obj, r, idx, 16, count);
            Gather(fire, r, idx, 20, count);
            Gather(stride, prior, idx, 8, count);
            PipeBarrier<PIPE_V>();
            Mul(obj, obj, fire, count);
            PipeBarrier<PIPE_V>();
            auto broadcast = tmp[Tile * 8];
            Brcb(broadcast, obj, uint8_t((count + 7) / 8), {1, 8});
            PipeBarrier<PIPE_V>();
            uint64_t scoreMask[2] = {0x1010101010101010ULL, 0};
            uint64_t xyMask[2] = {0x0303030303030303ULL, 0};
            uint64_t whMask[2] = {0x0c0c0c0c0c0c0c0cULL, 0};
            uint64_t boxMask[2] = {0x0f0f0f0f0f0f0f0fULL, 0};
            const uint8_t repeats = (count + 7) / 8;
            Adds(r, broadcast, 0.0f, scoreMask, repeats, {1, 1, 8, 8});
            Muls(r, r, 2.0f, boxMask, repeats, {1, 1, 8, 8});
            PipeBarrier<PIPE_V>();
            Adds(r, r, -0.5f, xyMask, repeats, {1, 1, 8, 8});
            Mul(r, r, r, whMask, repeats, {1, 1, 1, 8, 8, 8});
            PipeBarrier<PIPE_V>();
            Add(r, r, prior, xyMask, repeats, {1, 1, 1, 8, 8, 8});
            auto seq = expandIndex_.Get<uint32_t>();
            auto offset = tmp.ReinterpretCast<int32_t>();
            CreateVecIndex(offset, int32_t(0), count * 8);
            PipeBarrier<PIPE_V>();
            Muls(offset, offset, int32_t(4), count * 8);
            PipeBarrier<PIPE_V>();
            Gather(broadcast, prior, offset.ReinterpretCast<uint32_t>(), 4, count * 8);
            PipeBarrier<PIPE_V>();
            Mul(r, r, broadcast, whMask, repeats, {1, 1, 1, 8, 8, 8});
            Gather(stride, prior, idx, 8, count);
            PipeBarrier<PIPE_V>();
            Brcb(broadcast, stride, repeats, {1, 8});
            PipeBarrier<PIPE_V>();
            Mul(r, r, broadcast, boxMask, repeats, {1, 1, 1, 8, 8, 8});
            PipeBarrier<PIPE_V>();
            auto dst = outQueue_.AllocTensor<T>();
            if constexpr (sizeof(T) == 2) {
                for (uint32_t i = 0; i < count; i += 128) {
                    const uint8_t nrows = count - i > 128 ? 128 : count - i;
                    Cast(dst[i * 16], r[i * 8], RoundMode::CAST_RINT,
                         uint64_t(8), nrows, {1, 1, 1, 1});
                }
            } else {
                DataCopy(dst, r, count * 8);
            }
            outQueue_.EnQue(dst); dst = outQueue_.DeQue<T>();
            DataCopyPad(out_[(b * n_ + start) * 5], dst,
                DataCopyExtParams{uint16_t(count), 5 * uint32_t(sizeof(T)), 0, 0, 0});
            outQueue_.FreeTensor(dst); inRaw_.FreeTensor(raw); inPrior_.FreeTensor(prior);
        }
    }
private:
    TPipe pipe_;
    TQue<QuePosition::VECIN, 1> inRaw_, inPrior_;
    TQue<QuePosition::VECOUT, 1> outQueue_;
    TBuf<TPosition::VECCALC> rawFloat_, rows_, scratch_, ones_, rowIndex_, expandIndex_;
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
