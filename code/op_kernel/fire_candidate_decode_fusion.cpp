#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

using namespace AscendC;

template <typename T, uint32_t Pack>
class SmallCandidateDecode {
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &d)
    {
        n_ = d.candidates; tiles_ = (n_ + Pack - 1) / Pack; total_ = d.batch * tiles_; cores_ = d.cores;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        pipe_.InitBuffer(inRaw_, Pack == 1 ? 32 : 64);
        pipe_.InitBuffer(inPrior_, Pack == 1 ? 32 : 64);
        pipe_.InitBuffer(outQueue_, Pack == 1 ? 32 : 64);
        pipe_.InitBuffer(sigBuf_, Pack == 1 ? 32 : 64);
        pipe_.InitBuffer(resultBuf_, Pack == 1 ? 32 : 64);
        pipe_.InitBuffer(oneBuf_, Pack == 1 ? 32 : 64);
        Duplicate(oneBuf_.Get<float>(), 1.0f, Pack == 1 ? 8 : 16);
    }
    __aicore__ inline void Process()
    {
        for (uint32_t task = GetBlockIdx(); task < total_; task += cores_) {
            const uint32_t i = (task / tiles_) * n_ + (task % tiles_) * Pack;
            const uint32_t remaining = n_ - (task % tiles_) * Pack;
            const uint32_t count = remaining < Pack ? remaining : Pack;
            auto raw = inRaw_.Get<T>();
            auto prior = inPrior_.Get<float>();
            DataCopyPad(raw, raw_[i * 6],
                DataCopyExtParams{1, count * 6 * uint32_t(sizeof(T)), 0, 0, 0},
                DataCopyPadExtParams<T>{false, 0, 0, 0});
            DataCopyPad(prior, prior_[(i % n_) * 5],
                DataCopyExtParams{1, count * 20, 0, 0, 0},
                DataCopyPadExtParams<float>{false, 0, 0, 0});
            auto mv = pipe_.FetchEventID(HardEvent::MTE2_V);
            SetFlag<HardEvent::MTE2_V>(mv);
            WaitFlag<HardEvent::MTE2_V>(mv);
            auto sig = sigBuf_.Get<float>();
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
            Div(sig, oneBuf_.Get<float>(), sig, count * 6);
            auto vs = pipe_.FetchEventID(HardEvent::V_S);
            SetFlag<HardEvent::V_S>(vs);
            WaitFlag<HardEvent::V_S>(vs);
            auto result = resultBuf_.Get<float>();
            for (uint32_t row = 0; row < count; ++row) {
            const float stride = prior.GetValue(row * 5 + 2);
            float x = 2.0f * sig.GetValue(row * 6 + 0);
            x = x - 0.5f;
            x = x + prior.GetValue(row * 5 + 0);
            x = x * stride;
            float y = 2.0f * sig.GetValue(row * 6 + 1);
            y = y - 0.5f;
            y = y + prior.GetValue(row * 5 + 1);
            y = y * stride;
            float w = 2.0f * sig.GetValue(row * 6 + 2);
            w = w * w;
            w = w * prior.GetValue(row * 5 + 3);
            w = w * stride;
            float h = 2.0f * sig.GetValue(row * 6 + 3);
            h = h * h;
            h = h * prior.GetValue(row * 5 + 4);
            h = h * stride;
            const float score = sig.GetValue(row * 6 + 4) * sig.GetValue(row * 6 + 5);
            result.SetValue(row * 5 + 0, x); result.SetValue(row * 5 + 1, y);
            result.SetValue(row * 5 + 2, w); result.SetValue(row * 5 + 3, h);
            result.SetValue(row * 5 + 4, score);
            }
            auto sv = pipe_.FetchEventID(HardEvent::S_V);
            SetFlag<HardEvent::S_V>(sv);
            WaitFlag<HardEvent::S_V>(sv);
            auto dst = outQueue_.Get<T>();
            if constexpr (sizeof(T) == 2)
                Cast(dst, result, RoundMode::CAST_RINT, count * 5);
            else
                DataCopy(dst, result, ((count * 5 + 7) / 8) * 8);
            auto vm = pipe_.FetchEventID(HardEvent::V_MTE3);
            SetFlag<HardEvent::V_MTE3>(vm);
            WaitFlag<HardEvent::V_MTE3>(vm);
            DataCopyPad(out_[i * 5], dst,
                DataCopyExtParams{1, count * 5 * uint32_t(sizeof(T)), 0, 0, 0});
            auto om = pipe_.FetchEventID(HardEvent::MTE3_V);
            SetFlag<HardEvent::MTE3_V>(om);
            WaitFlag<HardEvent::MTE3_V>(om);
        }
    }
private:
    TPipe pipe_;
    TBuf<TPosition::VECIN> inRaw_, inPrior_;
    TBuf<TPosition::VECOUT> outQueue_;
    TBuf<TPosition::VECCALC> sigBuf_, resultBuf_, oneBuf_;
    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint32_t n_, total_, cores_, tiles_;
};


template <typename T, uint32_t Tile, bool Balanced = false>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t Pitch = Tile + 8;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &data)
    {
        const int64_t initBegin = GetSystemCycle();
        batch_ = data.batch;
        n_ = data.candidates;
        cores_ = data.cores;
        segments_ = data.segments;
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
        pipe_.InitBuffer(rawIndex_, Tile * sizeof(uint32_t));
        pipe_.InitBuffer(priorIndex_, Tile * sizeof(uint32_t));
        pipe_.InitBuffer(packIndex_, Tile * 5 * sizeof(uint32_t));
        auto r = rawIndex_.Get<int32_t>();
        auto p = priorIndex_.Get<int32_t>();
        CreateVecIndex(r, int32_t(0), Tile);
        CreateVecIndex(p, int32_t(0), Tile);
        PipeBarrier<PIPE_V>();
        Muls(r, r, int32_t(24), Tile);
        Muls(p, p, int32_t(20), Tile);
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
        Muls(mod, mod, (Tile == 512 ? 520.0f : (Tile == 576 ? 584.0f : 1032.0f)), Tile * 5);
        PipeBarrier<PIPE_V>();
        Add(mod, mod, row, Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(mod, mod, 4.0f, Tile * 5);
        PipeBarrier<PIPE_V>();
        Cast(index, mod, RoundMode::CAST_RINT, Tile * 5);
        Duplicate(ones_.Get<float>(), 1.0f, 64);
        PipeBarrier<PIPE_ALL>();
        stages_[0] = GetSystemCycle() - initBegin;
        for (uint32_t q = 1; q < 7; ++q) stages_[q] = 0;
    }

    __aicore__ inline void Process()
    {
        const uint32_t tiles = Balanced ? segments_ : (n_ + Tile - 1) / Tile;
        for (uint32_t task = GetBlockIdx(); task < batch_ * tiles; task += cores_) {
            const uint32_t b = task / tiles;
            const uint32_t start = Balanced ? n_ * (task % tiles) / tiles : (task % tiles) * Tile;
            const uint32_t count = Balanced ? n_ * ((task % tiles) + 1) / tiles - start : (start + Tile <= n_ ? Tile : n_ - start);
            PipeBarrier<PIPE_ALL>();
            int64_t mark = GetSystemCycle();
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
            PipeBarrier<PIPE_ALL>();
            stages_[1] += GetSystemCycle() - mark;
            mark = GetSystemCycle();
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
            PipeBarrier<PIPE_ALL>();
            stages_[2] += GetSystemCycle() - mark;
            mark = GetSystemCycle();
            auto c = columns_.Get<float>();
            const auto ri = rawIndex_.Get<uint32_t>();
            const auto pi = priorIndex_.Get<uint32_t>();
            for (uint32_t j = 0; j < 6; ++j)
                Gather(c[j * Pitch], sig, ri, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            for (uint32_t j = 0; j < 5; ++j)
                Gather(c[(j + 6) * Pitch], prior, pi, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            PipeBarrier<PIPE_V>();
            PipeBarrier<PIPE_ALL>();
            stages_[3] += GetSystemCycle() - mark;
            mark = GetSystemCycle();
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
            PipeBarrier<PIPE_ALL>();
            stages_[4] += GetSystemCycle() - mark;
            mark = GetSystemCycle();
            auto packed = packed_.Get<float>();
            Gather(packed, c, packIndex_.Get<uint32_t>(), 0, uint64_t(64), uint8_t((count * 5 + 63) / 64), 8);
            PipeBarrier<PIPE_V>();
            auto dst = outQueue_.AllocTensor<T>();
            if constexpr (sizeof(T) == 2) {
                Cast(dst, packed, RoundMode::CAST_RINT, count * 5);
            } else {
                DataCopy(dst, packed, ((count * 5 + 7) / 8) * 8);
            }
            PipeBarrier<PIPE_ALL>();
            stages_[5] += GetSystemCycle() - mark;
            mark = GetSystemCycle();
            outQueue_.EnQue(dst);
            dst = outQueue_.DeQue<T>();
            DataCopyPad(out_[(b * n_ + start) * 5], dst,
                        DataCopyExtParams{1, count * 5 * uint32_t(sizeof(T)), 0, 0, 0});
            outQueue_.FreeTensor(dst);
            inRaw_.FreeTensor(raw);
            inPrior_.FreeTensor(prior);
            PipeBarrier<PIPE_ALL>();
            stages_[6] += GetSystemCycle() - mark;
            mark = GetSystemCycle();

        }
        PipeBarrier<PIPE_ALL>();
        const uint32_t tiles = Balanced ? segments_ : (n_ + Tile - 1) / Tile;
        const uint32_t task = GetBlockIdx();
        const uint32_t b = task / tiles;
        const uint32_t start = Balanced ? n_ * (task % tiles) / tiles : (task % tiles) * Tile;
        if constexpr (sizeof(T) == 4) {
            for (uint32_t q = 0; q < 7; ++q)
                out_.SetValue((b * n_ + start) * 5 + q, float(stages_[q]) * 0.02f);
        }
    }
private:
    int64_t stages_[7];
    TPipe pipe_;
    TQue<QuePosition::VECIN, 1> inRaw_, inPrior_;
    TQue<QuePosition::VECOUT, 1> outQueue_;
    TBuf<TPosition::VECCALC> rawFloat_, columns_, packed_, ones_;
    TBuf<TPosition::VECCALC> rawIndex_, priorIndex_, packIndex_;
    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint32_t batch_, n_, cores_, segments_;
};

template <typename DT_RAW_PRED, uint32_t PATH>
__global__ __aicore__ void fire_candidate_decode_fusion(GM_ADDR raw_pred,
    GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling)
{
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, data, tiling);
    if constexpr (PATH == 0) {
        SmallCandidateDecode<DT_RAW_PRED, 1> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 3) {
        SmallCandidateDecode<DT_RAW_PRED, 2> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 4) {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 576, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 2) {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 512> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 1024> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    }
}
