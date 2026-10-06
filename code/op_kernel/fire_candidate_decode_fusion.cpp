#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

using namespace AscendC;

template <typename T>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t Tile = 1024;
    static constexpr uint32_t Pitch = Tile + 8;
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
        pipe_.InitBuffer(columns_, Pitch * 11 * sizeof(float));
        pipe_.InitBuffer(packed_, Tile * 5 * sizeof(float));
        pipe_.InitBuffer(ones_, 64 * sizeof(float));
        pipe_.InitBuffer(rawIndex_, 6 * 32);
        pipe_.InitBuffer(priorIndex_, 5 * 32);
        pipe_.InitBuffer(packIndex_, Tile * 5 * sizeof(uint32_t));
        auto r = rawIndex_.Get<uint32_t>();
        auto p = priorIndex_.Get<uint32_t>();
        r.SetValue(0, 1090785345U);
        r.SetValue(1, 1040U);
        r.SetValue(2, 0U);
        r.SetValue(3, 0U);
        r.SetValue(4, 0U);
        r.SetValue(5, 0U);
        r.SetValue(6, 0U);
        r.SetValue(7, 0U);
        r.SetValue(8, 2181570690U);
        r.SetValue(9, 2080U);
        r.SetValue(10, 0U);
        r.SetValue(11, 0U);
        r.SetValue(12, 0U);
        r.SetValue(13, 0U);
        r.SetValue(14, 0U);
        r.SetValue(15, 0U);
        r.SetValue(16, 68174084U);
        r.SetValue(17, 4161U);
        r.SetValue(18, 0U);
        r.SetValue(19, 0U);
        r.SetValue(20, 0U);
        r.SetValue(21, 0U);
        r.SetValue(22, 0U);
        r.SetValue(23, 0U);
        r.SetValue(24, 136348168U);
        r.SetValue(25, 8322U);
        r.SetValue(26, 0U);
        r.SetValue(27, 0U);
        r.SetValue(28, 0U);
        r.SetValue(29, 0U);
        r.SetValue(30, 0U);
        r.SetValue(31, 0U);
        r.SetValue(32, 272696336U);
        r.SetValue(33, 16644U);
        r.SetValue(34, 0U);
        r.SetValue(35, 0U);
        r.SetValue(36, 0U);
        r.SetValue(37, 0U);
        r.SetValue(38, 0U);
        r.SetValue(39, 0U);
        r.SetValue(40, 545392672U);
        r.SetValue(41, 33288U);
        r.SetValue(42, 0U);
        r.SetValue(43, 0U);
        r.SetValue(44, 0U);
        r.SetValue(45, 0U);
        r.SetValue(46, 0U);
        r.SetValue(47, 0U);
        p.SetValue(0, 1108378657U);
        p.SetValue(1, 8U);
        p.SetValue(2, 0U);
        p.SetValue(3, 0U);
        p.SetValue(4, 0U);
        p.SetValue(5, 0U);
        p.SetValue(6, 0U);
        p.SetValue(7, 0U);
        p.SetValue(8, 2216757314U);
        p.SetValue(9, 16U);
        p.SetValue(10, 0U);
        p.SetValue(11, 0U);
        p.SetValue(12, 0U);
        p.SetValue(13, 0U);
        p.SetValue(14, 0U);
        p.SetValue(15, 0U);
        p.SetValue(16, 138547332U);
        p.SetValue(17, 33U);
        p.SetValue(18, 0U);
        p.SetValue(19, 0U);
        p.SetValue(20, 0U);
        p.SetValue(21, 0U);
        p.SetValue(22, 0U);
        p.SetValue(23, 0U);
        p.SetValue(24, 277094664U);
        p.SetValue(25, 66U);
        p.SetValue(26, 0U);
        p.SetValue(27, 0U);
        p.SetValue(28, 0U);
        p.SetValue(29, 0U);
        p.SetValue(30, 0U);
        p.SetValue(31, 0U);
        p.SetValue(32, 554189328U);
        p.SetValue(33, 132U);
        p.SetValue(34, 0U);
        p.SetValue(35, 0U);
        p.SetValue(36, 0U);
        p.SetValue(37, 0U);
        p.SetValue(38, 0U);
        p.SetValue(39, 0U);
        auto event = pipe_.FetchEventID(HardEvent::S_V);
        SetFlag<HardEvent::S_V>(event);
        WaitFlag<HardEvent::S_V>(event);
        auto seq = packed_.Get<float>();
        auto row = rawFloat_.Get<float>();
        auto mod = columns_.Get<float>();
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
        Muls(mod, mod, 1032.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Add(mod, mod, row, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(mod, mod, 4.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(index, mod, RoundMode::CAST_RINT, Tile * 5);
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
            Muls(sig, sig, -1.0f, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            PipeBarrier<PIPE_V>();
            Exp(sig, sig, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            PipeBarrier<PIPE_V>();
            Adds(sig, sig, 1.0f, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            PipeBarrier<PIPE_V>();
            Div(sig, ones_.Get<float>(), sig, uint64_t(64),
                uint8_t((count * 6 + 63) / 64), {1, 1, 1, 8, 0, 8});
            PipeBarrier<PIPE_V>();
            auto c = columns_.Get<float>();
            const auto ri = rawIndex_.Get<uint32_t>();
            const auto pi = priorIndex_.Get<uint32_t>();
            uint64_t selected = 0;
            for (uint32_t j = 0; j < 6; ++j)
                GatherMask(c[j * Pitch], sig, ri[j * 8], true, 48,
                           {1, uint16_t((count + 7) / 8), 6, 0}, selected);
            for (uint32_t j = 0; j < 5; ++j)
                GatherMask(c[(j + 6) * Pitch], prior, pi[j * 8], true, 40,
                           {1, uint16_t((count + 7) / 8), 5, 0}, selected);
            PipeBarrier<PIPE_V>();
            Muls(c, c, 2.0f, Pitch * 4);
            PipeBarrier<PIPE_V>();
            Adds(c, c, -0.5f, Pitch * 2);
            Mul(c[2 * Pitch], c[2 * Pitch], c[2 * Pitch], Pitch * 2);
            Mul(c[4 * Pitch], c[4 * Pitch], c[5 * Pitch], count);
            PipeBarrier<PIPE_V>();
            Add(c, c, c[6 * Pitch], Pitch * 2);
            Mul(c[2 * Pitch], c[2 * Pitch], c[9 * Pitch], Pitch * 2);
            PipeBarrier<PIPE_V>();
            for (uint32_t j = 0; j < 4; ++j)
                Mul(c[j * Pitch], c[j * Pitch], c[8 * Pitch], count);
            PipeBarrier<PIPE_V>();
            auto packed = packed_.Get<float>();
            Gather(packed, c, packIndex_.Get<uint32_t>(), 0, uint64_t(64), uint8_t((count * 5 + 63) / 64), 8);
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
    TBuf<TPosition::VECCALC> rawIndex_, priorIndex_, packIndex_;
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
