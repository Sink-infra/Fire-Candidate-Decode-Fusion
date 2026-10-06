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
        Duplicate(Local<float>(5), 1.0f, ((Pack * 24 + 31) / 32) * 8);
    }
    __aicore__ inline void Process()
    {
        for (uint32_t task = GetBlockIdx(); task < total_; task += cores_) {
            const uint32_t i = (task / tiles_) * n_ + (task % tiles_) * Pack;
            const uint32_t remaining = n_ - (task % tiles_) * Pack;
            const uint32_t count = remaining < Pack ? remaining : Pack;
            auto raw = Local<T>(0);
            auto prior = Local<float>(1);
            if constexpr (Pack == 8) {
                DataCopy(raw, raw_[i * 6], count * 6);
                DataCopy(prior, prior_[(i % n_) * 5], count * 5);
            } else {
            DataCopyPad(raw, raw_[i * 6],
                DataCopyExtParams{1, count * 6 * uint32_t(sizeof(T)), 0, 0, 0},
                DataCopyPadExtParams<T>{false, 0, 0, 0});
            DataCopyPad(prior, prior_[(i % n_) * 5],
                DataCopyExtParams{1, count * 20, 0, 0, 0},
                DataCopyPadExtParams<float>{false, 0, 0, 0});
            }
            constexpr uint32_t mv = 0;
            SetFlag<HardEvent::MTE2_V>(mv);
            WaitFlag<HardEvent::MTE2_V>(mv);
            auto sig = Local<float>(3);
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
            Div(sig, Local<float>(5), sig, count * 6);
            constexpr uint32_t vs = 0;
            SetFlag<HardEvent::V_S>(vs);
            WaitFlag<HardEvent::V_S>(vs);
            auto result = Local<float>(4);
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
            constexpr uint32_t sv = 0;
            SetFlag<HardEvent::S_V>(sv);
            WaitFlag<HardEvent::S_V>(sv);
            auto dst = Local<T>(2);
            if constexpr (sizeof(T) == 2)
                Cast(dst, result, RoundMode::CAST_RINT, count * 5);
            else
                DataCopy(dst, result, ((count * 5 + 7) / 8) * 8);
            constexpr uint32_t vm = 0;
            SetFlag<HardEvent::V_MTE3>(vm);
            WaitFlag<HardEvent::V_MTE3>(vm);
            if constexpr (Pack == 8 && sizeof(T) == 4)
                DataCopy(out_[i * 5], dst, count * 5);
            else {
            DataCopyPad(out_[i * 5], dst,
                DataCopyExtParams{1, count * 5 * uint32_t(sizeof(T)), 0, 0, 0});
            }
            constexpr uint32_t om = 0;
            SetFlag<HardEvent::MTE3_V>(om);
            WaitFlag<HardEvent::MTE3_V>(om);
        }
    }
private:
    template <typename U>
    __aicore__ inline LocalTensor<U> Local(uint32_t slot)
    {
        constexpr uint32_t Bytes = ((Pack * 24 + 31) / 32) * 32;
        const TPosition pos = slot < 2 ? TPosition::VECIN :
            (slot == 2 ? TPosition::VECOUT : TPosition::VECCALC);
        return LocalTensor<U>(pos, slot * Bytes, Bytes / sizeof(U));
    }
    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint32_t n_, total_, cores_, tiles_;
};


template <typename T, uint32_t Tile, bool Balanced = false>
class MediumCandidateDecodeFusion {
    static constexpr uint32_t Pitch = Tile + 8;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &data)
    {
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
        PipeBarrier<PIPE_V>();
    }

    __aicore__ inline void Process()
    {
        const uint32_t tiles = Balanced ? segments_ : (n_ + Tile - 1) / Tile;
        for (uint32_t task = GetBlockIdx(); task < batch_ * tiles; task += cores_) {
            const uint32_t b = task / tiles;
            const uint32_t start = Balanced ? n_ * (task % tiles) / tiles : (task % tiles) * Tile;
            const uint32_t count = Balanced ? n_ * ((task % tiles) + 1) / tiles - start : (start + Tile <= n_ ? Tile : n_ - start);
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
            for (uint32_t j = 0; j < 6; ++j)
                Gather(c[j * Pitch], sig, ri, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            for (uint32_t j = 0; j < 5; ++j)
                Gather(c[(j + 6) * Pitch], prior, pi, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
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
    uint32_t batch_, n_, cores_, segments_;
};

template <typename T, uint32_t Tile, bool Balanced = false, bool CachePrior = false>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t Pitch = Tile + 8;
    static constexpr bool Reuse = (Tile == 1024 || CachePrior) && sizeof(T) == 4;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &data)
    {
        batch_ = data.batch;
        n_ = data.candidates;
        cores_ = data.cores;
        segments_ = data.segments;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        pipe_.InitBuffer(inRaw_, (Tile == 1024 || CachePrior) ? 2 : 1, Tile * 6 * sizeof(T));
        pipe_.InitBuffer(inPrior_, (Tile == 1024 && !CachePrior) ? 2 : 1, Tile * 5 * sizeof(float));
        pipe_.InitBuffer(outQueue_, 1, Tile * 5 * sizeof(T));
        if constexpr (!Reuse) pipe_.InitBuffer(rawFloat_, Tile * 6 * sizeof(float));
        pipe_.InitBuffer(columns_, Pitch * 11 * sizeof(float));
        if constexpr (!Reuse) pipe_.InitBuffer(packed_, Tile * 5 * sizeof(float));
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
        LocalTensor<int32_t> seq, row;
        if constexpr (Reuse) {
            seq = columns_.Get<int32_t>();
            row = seq[Tile * 5];
        } else {
            seq = packed_.Get<int32_t>();
            row = rawFloat_.Get<int32_t>();
        }
        auto index = packIndex_.Get<int32_t>();
        CreateVecIndex(seq, int32_t(0), Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(row, seq, int32_t(13108), Tile * 5);
        PipeBarrier<PIPE_V>();
        ShiftRight(row, row, int32_t(16), Tile * 5);
        Muls(seq, seq, int32_t(4 * Pitch), Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(row, row, int32_t(4 - 20 * Pitch), Tile * 5);
        PipeBarrier<PIPE_V>();
        Add(index, seq, row, Tile * 5);
        Duplicate(ones_.Get<float>(), 1.0f, 64);
        PipeBarrier<PIPE_V>();
    }

    __aicore__ inline void Process()
    {
        if constexpr (CachePrior) { ProcessCached(); return; }
        const uint32_t tiles = Balanced ? segments_ : (n_ + Tile - 1) / Tile;
        if constexpr (Tile == 1024) {
            if (GetBlockIdx() < batch_ * tiles) Load(GetBlockIdx(), tiles);
        }
        for (uint32_t task = GetBlockIdx(); task < batch_ * tiles; task += cores_) {
            const uint32_t b = task / tiles;
            const uint32_t start = Balanced ? n_ * (task % tiles) / tiles : (task % tiles) * Tile;
            const uint32_t count = Balanced ? n_ * ((task % tiles) + 1) / tiles - start : (start + Tile <= n_ ? Tile : n_ - start);
            if constexpr (Tile != 1024) Load(task, tiles);
            auto raw = inRaw_.DeQue<T>();
            auto prior = inPrior_.DeQue<float>();
            if constexpr (Tile == 1024) {
                if (task + cores_ < batch_ * tiles) Load(task + cores_, tiles);
            }
            Compute(raw, prior, b, start, count);
            inRaw_.FreeTensor(raw);
            inPrior_.FreeTensor(prior);
        }
    }
private:
    __aicore__ inline void Compute(LocalTensor<T> raw, LocalTensor<float> prior,
                                  uint32_t b, uint32_t start, uint32_t count)
    {
            LocalTensor<float> sig;
            if constexpr (Reuse) {
                sig = raw;
            } else {
                sig = rawFloat_.Get<float>();
                if constexpr (sizeof(T) == 2)
                    Cast(sig, raw, RoundMode::CAST_NONE, count * 6);
                else
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
            for (uint32_t j = 0; j < 6; ++j)
                Gather(c[j * Pitch], sig, ri, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            if constexpr (!CachePrior) {
            for (uint32_t j = 0; j < 5; ++j)
                Gather(c[(j + 6) * Pitch], prior, pi, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            }
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
            auto dst = outQueue_.AllocTensor<T>();
            LocalTensor<float> packed;
            if constexpr (Reuse) packed = dst;
            else packed = packed_.Get<float>();
            Gather(packed, c, packIndex_.Get<uint32_t>(), 0, uint64_t(64),
                uint8_t((count * 5 + 63) / 64), 8);
            PipeBarrier<PIPE_V>();
            if constexpr (sizeof(T) == 2)
                Cast(dst, packed, RoundMode::CAST_RINT, count * 5);
            else if constexpr (!Reuse)
                DataCopy(dst, packed, ((count * 5 + 7) / 8) * 8);
            outQueue_.EnQue(dst);
            dst = outQueue_.DeQue<T>();
            DataCopyPad(out_[(b * n_ + start) * 5], dst,
                        DataCopyExtParams{1, count * 5 * uint32_t(sizeof(T)), 0, 0, 0});
            outQueue_.FreeTensor(dst);
    }
    __aicore__ inline void LoadRaw(uint32_t b, uint32_t start, uint32_t count)
    {
        auto raw = inRaw_.AllocTensor<T>();
        DataCopyPad(raw, raw_[(b * n_ + start) * 6],
            DataCopyExtParams{1, count * 6 * uint32_t(sizeof(T)), 0, 0, 0},
            DataCopyPadExtParams<T>{false, 0, 0, 0});
        inRaw_.EnQue(raw);
    }
    __aicore__ inline void ProcessCached()
    {
        const uint32_t groups = (n_ + 7) / 8;
        const uint32_t begin = (groups * GetBlockIdx() / cores_) * 8;
        uint32_t end = (groups * (GetBlockIdx() + 1) / cores_) * 8;
        if (end > n_) end = n_;
        for (uint32_t start = begin; start < end; start += Tile) {
            const uint32_t count = start + Tile <= end ? Tile : end - start;
            auto prior = inPrior_.AllocTensor<float>();
            DataCopyPad(prior, prior_[start * 5],
                DataCopyExtParams{1, count * 20, 0, 0, 0},
                DataCopyPadExtParams<float>{false, 0, 0, 0});
            inPrior_.EnQue(prior);
            prior = inPrior_.DeQue<float>();
            auto c = columns_.Get<float>();
            for (uint32_t j = 0; j < 5; ++j)
                Gather(c[(j + 6) * Pitch], prior, priorIndex_.Get<uint32_t>(), j * 4,
                       uint64_t(64), uint8_t((count + 63) / 64), 8);
            PipeBarrier<PIPE_V>();
            inPrior_.FreeTensor(prior);
            LoadRaw(0, start, count);
            for (uint32_t b = 0; b < batch_; ++b) {
                auto raw = inRaw_.DeQue<T>();
                if (b + 1 < batch_) LoadRaw(b + 1, start, count);
                LocalTensor<float> unused;
                Compute(raw, unused, b, start, count);
                inRaw_.FreeTensor(raw);
            }
        }
    }

    __aicore__ inline void Load(uint32_t task, uint32_t tiles)
    {
        const uint32_t b = task / tiles;
        const uint32_t start = Balanced ? n_ * (task % tiles) / tiles : (task % tiles) * Tile;
        const uint32_t count = Balanced ? n_ * ((task % tiles) + 1) / tiles - start :
            (start + Tile <= n_ ? Tile : n_ - start);
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
    }

    TPipe pipe_;
    TQue<QuePosition::VECIN, 1> inRaw_, inPrior_;
    TQue<QuePosition::VECOUT, 1> outQueue_;
    TBuf<TPosition::VECCALC> rawFloat_, columns_, packed_, ones_;
    TBuf<TPosition::VECCALC> rawIndex_, priorIndex_, packIndex_;
    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint32_t batch_, n_, cores_, segments_;
};


template <typename T>
class StaticCachedCandidateDecode {
    static constexpr uint32_t Tile = 768;
    static constexpr uint32_t Pitch = Tile + 8;
    static constexpr bool Reuse = sizeof(T) == 4;
    static constexpr uint32_t RawBytes = Tile * 6 * sizeof(T);
    static constexpr uint32_t PriorAddr = RawBytes * 2;
    static constexpr uint32_t OutputAddr = PriorAddr + Tile * 20;
    static constexpr uint32_t FloatAddr = OutputAddr + Tile * 5 * sizeof(T);
    static constexpr uint32_t ColumnAddr = FloatAddr + (Reuse ? 0 : Tile * 24);
    static constexpr uint32_t PackedAddr = ColumnAddr + Pitch * 44;
    static constexpr uint32_t OneAddr = PackedAddr + (Reuse ? 0 : Tile * 20);
    static constexpr uint32_t RawIdxAddr = OneAddr + 256;
    static constexpr uint32_t PriorIdxAddr = RawIdxAddr + Tile * 4;
    static constexpr uint32_t PackIdxAddr = PriorIdxAddr + Tile * 4;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &d, TPipe *pipe)
    {
        pipe_ = pipe;
        batch_ = d.batch; n_ = d.candidates; cores_ = d.cores;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        for (uint32_t j = 0; j < 2; ++j) {
            rawReady_[j] = pipe->AllocEventID<HardEvent::MTE2_V>();
            rawFree_[j] = pipe->AllocEventID<HardEvent::V_MTE2>();
        }
        priorReady_ = pipe->AllocEventID<HardEvent::MTE2_V>();
        priorFree_ = pipe->AllocEventID<HardEvent::V_MTE2>();
        outReady_ = pipe->FetchEventID(HardEvent::V_MTE3);
        outFree_ = pipe->FetchEventID(HardEvent::MTE3_V);
        auto ri = At<int32_t>(RawIdxAddr, Tile);
        auto pi = At<int32_t>(PriorIdxAddr, Tile);
        CreateVecIndex(ri, int32_t(0), Tile);
        CreateVecIndex(pi, int32_t(0), Tile);
        PipeBarrier<PIPE_V>();
        Muls(ri, ri, int32_t(24), Tile);
        Muls(pi, pi, int32_t(20), Tile);
        LocalTensor<int32_t> seq, row;
        if constexpr (Reuse) {
            seq = At<int32_t>(ColumnAddr, Pitch * 11);
            row = seq[Tile * 5];
        } else {
            seq = At<int32_t>(PackedAddr, Tile * 5);
            row = At<int32_t>(FloatAddr, Tile * 6);
        }
        auto index = At<int32_t>(PackIdxAddr, Tile * 5);
        CreateVecIndex(seq, int32_t(0), Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(row, seq, int32_t(13108), Tile * 5);
        PipeBarrier<PIPE_V>();
        ShiftRight(row, row, int32_t(16), Tile * 5);
        Muls(seq, seq, int32_t(4 * Pitch), Tile * 5);
        PipeBarrier<PIPE_V>();
        Muls(row, row, int32_t(4 - 20 * Pitch), Tile * 5);
        PipeBarrier<PIPE_V>();
        Add(index, seq, row, Tile * 5);
        Duplicate(At<float>(OneAddr, 64), 1.0f, 64);
        PipeBarrier<PIPE_V>();
    }
    __aicore__ inline void Process()
    {
        const uint32_t groups = (n_ + 7) / 8;
        const uint32_t begin = (groups * GetBlockIdx() / cores_) * 8;
        uint32_t end = (groups * (GetBlockIdx() + 1) / cores_) * 8;
        if (end > n_) end = n_;
        for (uint32_t start = begin; start < end; start += Tile) {
            const uint32_t count = start + Tile <= end ? Tile : end - start;
            if (priorUsed_) WaitFlag<HardEvent::V_MTE2>(priorFree_);
            auto prior = At<float>(PriorAddr, Tile * 5, TPosition::VECIN);
            DataCopyPad(prior, prior_[start * 5],
                DataCopyExtParams{1, count * 20, 0, 0, 0},
                DataCopyPadExtParams<float>{false, 0, 0, 0});
            SetFlag<HardEvent::MTE2_V>(priorReady_);
            WaitFlag<HardEvent::MTE2_V>(priorReady_);
            auto c = At<float>(ColumnAddr, Pitch * 11);
            for (uint32_t j = 0; j < 5; ++j)
                Gather(c[(j + 6) * Pitch], prior, At<uint32_t>(PriorIdxAddr, Tile),
                    j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            PipeBarrier<PIPE_V>();
            SetFlag<HardEvent::V_MTE2>(priorFree_);
            priorUsed_ = true;
            LoadRaw(0, start, count);
            for (uint32_t b = 0; b < batch_; ++b) {
                const uint32_t slot = b % 2;
                WaitFlag<HardEvent::MTE2_V>(rawReady_[slot]);
                if (b + 1 < batch_) LoadRaw(b + 1, start, count);
                auto raw = At<T>(slot * RawBytes, Tile * 6, TPosition::VECIN);
                Compute(raw, prior, b, start, count, slot);
            }
        }
        if (outPending_) WaitFlag<HardEvent::MTE3_V>(outFree_);
        if (priorUsed_) WaitFlag<HardEvent::V_MTE2>(priorFree_);
        for (uint32_t j = 0; j < 2; ++j) {
            if (rawUsed_[j]) WaitFlag<HardEvent::V_MTE2>(rawFree_[j]);
            pipe_->ReleaseEventID<HardEvent::MTE2_V>(rawReady_[j]);
            pipe_->ReleaseEventID<HardEvent::V_MTE2>(rawFree_[j]);
        }
        pipe_->ReleaseEventID<HardEvent::MTE2_V>(priorReady_);
        pipe_->ReleaseEventID<HardEvent::V_MTE2>(priorFree_);
    }
private:
    template <typename U>
    __aicore__ inline LocalTensor<U> At(uint32_t addr, uint32_t count, TPosition pos = TPosition::VECCALC)
    {
        return LocalTensor<U>(pos, addr, count);
    }
    __aicore__ inline void LoadRaw(uint32_t b, uint32_t start, uint32_t count)
    {
        const uint32_t slot = b % 2;
        if (rawUsed_[slot]) WaitFlag<HardEvent::V_MTE2>(rawFree_[slot]);
        auto raw = At<T>(slot * RawBytes, Tile * 6, TPosition::VECIN);
        DataCopyPad(raw, raw_[(b * n_ + start) * 6],
            DataCopyExtParams{1, count * 6 * uint32_t(sizeof(T)), 0, 0, 0},
            DataCopyPadExtParams<T>{false, 0, 0, 0});
        SetFlag<HardEvent::MTE2_V>(rawReady_[slot]);
        rawUsed_[slot] = true;
    }
    __aicore__ inline void Compute(LocalTensor<T> raw, LocalTensor<float> prior,
                                  uint32_t b, uint32_t start, uint32_t count, uint32_t slot)
    {
            LocalTensor<float> sig;
            if constexpr (Reuse) {
                sig = raw;
            } else {
                sig = At<float>(FloatAddr, Tile * 6);
                if constexpr (sizeof(T) == 2)
                    Cast(sig, raw, RoundMode::CAST_NONE, count * 6);
                else
                    DataCopy(sig, raw, ((count * 6 + 7) / 8) * 8);
            }
            PipeBarrier<PIPE_V>();
            Muls(sig, sig, -1.0f, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            PipeBarrier<PIPE_V>();
            Exp(sig, sig, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            PipeBarrier<PIPE_V>();
            Adds(sig, sig, 1.0f, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            PipeBarrier<PIPE_V>();
            Div(sig, At<float>(OneAddr, 64), sig, uint64_t(64),
                uint8_t((count * 6 + 63) / 64), {1, 1, 1, 8, 0, 8});
            PipeBarrier<PIPE_V>();
            auto c = At<float>(ColumnAddr, Pitch * 11);
            const auto ri = At<uint32_t>(RawIdxAddr, Tile);
            const auto pi = At<uint32_t>(PriorIdxAddr, Tile);
            for (uint32_t j = 0; j < 6; ++j)
                Gather(c[j * Pitch], sig, ri, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            PipeBarrier<PIPE_V>();
            SetFlag<HardEvent::V_MTE2>(rawFree_[slot]);
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
            if (outPending_) {
                WaitFlag<HardEvent::MTE3_V>(outFree_);
                outPending_ = false;
            }
            auto dst = At<T>(OutputAddr, Tile * 5, TPosition::VECOUT);
            LocalTensor<float> packed;
            if constexpr (Reuse) packed = dst;
            else packed = At<float>(PackedAddr, Tile * 5);
            Gather(packed, c, At<uint32_t>(PackIdxAddr, Tile * 5), 0, uint64_t(64),
                uint8_t((count * 5 + 63) / 64), 8);
            PipeBarrier<PIPE_V>();
            if constexpr (sizeof(T) == 2)
                Cast(dst, packed, RoundMode::CAST_RINT, count * 5);
            else if constexpr (!Reuse)
                DataCopy(dst, packed, ((count * 5 + 7) / 8) * 8);
            SetFlag<HardEvent::V_MTE3>(outReady_);
            WaitFlag<HardEvent::V_MTE3>(outReady_);
            DataCopyPad(out_[(b * n_ + start) * 5], dst,
                        DataCopyExtParams{1, count * 5 * uint32_t(sizeof(T)), 0, 0, 0});
            SetFlag<HardEvent::MTE3_V>(outFree_);
            outPending_ = true;
    }

    TPipe *pipe_;
    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint32_t batch_, n_, cores_;
    int32_t rawReady_[2], rawFree_[2], priorReady_, priorFree_, outReady_, outFree_;
    bool rawUsed_[2] = {false, false};
    bool priorUsed_ = false, outPending_ = false;
};

template <typename DT_RAW_PRED, uint32_t PATH>
__global__ __aicore__ void fire_candidate_decode_fusion(GM_ADDR raw_pred,
    GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling)
{
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, data, tiling);
    if constexpr (PATH == 8) {
        TPipe pipe;
        StaticCachedCandidateDecode<DT_RAW_PRED> op;
        op.Init(raw_pred, priors, decoded, data, &pipe);
        op.Process();
    } else if constexpr (PATH == 0) {
        SmallCandidateDecode<DT_RAW_PRED, 1> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 5) {
        SmallCandidateDecode<DT_RAW_PRED, 8> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 3) {
        SmallCandidateDecode<DT_RAW_PRED, 2> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 6) {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 768, false, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 7) {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 1024, false, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 4) {
        MediumCandidateDecodeFusion<DT_RAW_PRED, 576, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 2) {
        MediumCandidateDecodeFusion<DT_RAW_PRED, 512> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 1024> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    }
}
