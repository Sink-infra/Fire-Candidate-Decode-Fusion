#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

using namespace AscendC;

template <typename T>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t Tile = 256;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &data)
    {
        batch_ = data.batch;
        n_ = data.candidates;
        cores_ = data.cores;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        pipe_.InitBuffer(inRaw_, 1, Tile * 6 * sizeof(T));
        pipe_.InitBuffer(inPrior_, 1, Tile * 5 * sizeof(float));
        pipe_.InitBuffer(outQueue_, 1, Tile * 5 * sizeof(T));
        pipe_.InitBuffer(rawFloat_, Tile * 6 * sizeof(float));
        pipe_.InitBuffer(columns_, Tile * 11 * sizeof(float));
        pipe_.InitBuffer(packed_, Tile * 5 * sizeof(float));
        pipe_.InitBuffer(ones_, Tile * 6 * sizeof(float));
        pipe_.InitBuffer(rawIndex_, Tile * sizeof(uint32_t));
        pipe_.InitBuffer(priorIndex_, Tile * sizeof(uint32_t));
        pipe_.InitBuffer(packIndex_, Tile * 5 * sizeof(uint32_t));
        pipe_.InitBuffer(indexScratch_, Tile * 15 * sizeof(float));
        auto r = rawIndex_.Get<int32_t>();
        auto p = priorIndex_.Get<int32_t>();
        CreateVecIndex(r, int32_t(0), Tile);
        CreateVecIndex(p, int32_t(0), Tile);
        PipeBarrier<PIPE_V>();
        Muls(r, r, int32_t(24), Tile);
        Muls(p, p, int32_t(20), Tile);
        auto scratch = indexScratch_.Get<float>();
        auto seq = scratch;
        auto row = scratch[Tile * 5];
        auto mod = scratch[Tile * 10];
        auto index = packIndex_.Get<int32_t>();
        CreateVecIndex(seq, 0.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(row, seq, 0.2f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(index, row, RoundMode::CAST_FLOOR, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(row, index, RoundMode::CAST_NONE, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(mod, row, 5.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Sub(mod, seq, mod, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(mod, mod, float(Tile), Tile * 5);
        PipeBarrier<PIPE_V>();
        Add(mod, mod, row, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(mod, mod, 4.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(index, mod, RoundMode::CAST_RINT, Tile * 5);
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
                        DataCopyExtParams{1, count * 20, 0, 0, 0},
                        DataCopyPadExtParams<float>{false, 0, 0, 0});
            inRaw_.EnQue(raw);
            inPrior_.EnQue(prior);
            raw = inRaw_.DeQue<T>();
            prior = inPrior_.DeQue<float>();
            auto sig = rawFloat_.Get<float>();
            if constexpr (sizeof(T) == 2) {
                Cast(sig, raw, RoundMode::CAST_NONE, count * 6);
            } else {
                DataCopy(sig, raw, ((count * 6 + 7) / 8) * 8);
            }
            PipeBarrier<PIPE_V>();
            Muls(sig, sig, -1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Exp(sig, sig, count * 6);
            PipeBarrier<PIPE_V>();
            Adds(sig, sig, 1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Div(sig, ones_.Get<float>(), sig, count * 6);
            PipeBarrier<PIPE_V>();
            auto c = columns_.Get<float>();
            const auto ri = rawIndex_.Get<uint32_t>();
            const auto pi = priorIndex_.Get<uint32_t>();
            for (uint32_t j = 0; j < 6; ++j)
                Gather(c[j * Tile], sig, ri, j * 4, count);
            for (uint32_t j = 0; j < 5; ++j)
                Gather(c[(j + 6) * Tile], prior, pi, j * 4, count);
            PipeBarrier<PIPE_V>();
            Muls(c, c, 2.0f, count);
            Muls(c[Tile], c[Tile], 2.0f, count);
            Muls(c[2 * Tile], c[2 * Tile], 2.0f, count);
            Muls(c[3 * Tile], c[3 * Tile], 2.0f, count);
            PipeBarrier<PIPE_V>();
            Adds(c, c, -0.5f, count);
            Adds(c[Tile], c[Tile], -0.5f, count);
            Mul(c[2 * Tile], c[2 * Tile], c[2 * Tile], count);
            Mul(c[3 * Tile], c[3 * Tile], c[3 * Tile], count);
            Mul(c[4 * Tile], c[4 * Tile], c[5 * Tile], count);
            PipeBarrier<PIPE_V>();
            Add(c, c, c[6 * Tile], count);
            Add(c[Tile], c[Tile], c[7 * Tile], count);
            Mul(c[2 * Tile], c[2 * Tile], c[9 * Tile], count);
            Mul(c[3 * Tile], c[3 * Tile], c[10 * Tile], count);
            PipeBarrier<PIPE_V>();
            for (uint32_t j = 0; j < 4; ++j)
                Mul(c[j * Tile], c[j * Tile], c[8 * Tile], count);
            PipeBarrier<PIPE_V>();
            auto packed = packed_.Get<float>();
            Gather(packed, c, packIndex_.Get<uint32_t>(), 0, count * 5);
            PipeBarrier<PIPE_V>();
            auto dst = outQueue_.AllocTensor<T>();
            if constexpr (sizeof(T) == 2) {
                Cast(dst, packed, RoundMode::CAST_RINT, count * 5);
            } else {
                DataCopy(dst, packed, ((count * 5 + 7) / 8) * 8);
            }
            outQueue_.EnQue(dst);
            dst = outQueue_.DeQue<T>();
            DataCopyPad(out_[(b * n_ + start) * 5], dst,
                        DataCopyExtParams{1, count * 5 * uint32_t(sizeof(T)), 0, 0, 0});
            outQueue_.FreeTensor(dst);
            inRaw_.FreeTensor(raw);
            inPrior_.FreeTensor(prior);
        }
    }
private:
    TPipe pipe_;
    TQue<QuePosition::VECIN, 1> inRaw_, inPrior_;
    TQue<QuePosition::VECOUT, 1> outQueue_;
    TBuf<TPosition::VECCALC> rawFloat_, columns_, packed_, ones_;
    TBuf<TPosition::VECCALC> rawIndex_, priorIndex_, packIndex_, indexScratch_;
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
