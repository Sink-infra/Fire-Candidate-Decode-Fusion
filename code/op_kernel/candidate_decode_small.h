#pragma once
#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"

using namespace AscendC;

template <typename T, uint32_t Pack, bool Unroll = false>
class SingleTaskSmallCandidateDecode {
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &d)
    {

        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        Duplicate(Local<float>(5), 1.0f, ((Pack * 24 + 31) / 32) * 8);
    }
    __aicore__ inline void Process()
    {
        {
            const uint32_t task = GetBlockIdx();
            const uint32_t i = task * Pack;
            constexpr uint32_t count = Pack;
            auto raw = Local<T>(0);
            auto prior = Local<float>(1);
            if constexpr (Pack == 8) {
                DataCopy(raw, raw_[i * 6], count * 6);
                DataCopy(prior, prior_[i * 5], count * 5);
            } else {
            DataCopyPad(raw, raw_[i * 6],
                DataCopyExtParams{1, count * 6 * uint32_t(sizeof(T)), 0, 0, 0},
                DataCopyPadExtParams<T>{false, 0, 0, 0});
            DataCopyPad(prior, prior_[i * 5],
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
            if constexpr (Pack == 4 && Unroll) {
                if (count == 4) {
                    DecodeRow<0>(sig, prior, result);
                    DecodeRow<1>(sig, prior, result);
                    DecodeRow<2>(sig, prior, result);
                    DecodeRow<3>(sig, prior, result);
                } else {
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
                }
            } else {
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
    template <uint32_t Row>
    __aicore__ inline void DecodeRow(LocalTensor<float> sig, LocalTensor<float> prior, LocalTensor<float> result)
    {
        constexpr uint32_t row = Row;
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
};


template <typename T, uint32_t Pack, bool Unroll = false>
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
            if constexpr (Pack == 4 && Unroll) {
                if (count == 4) {
                    DecodeRow<0>(sig, prior, result);
                    DecodeRow<1>(sig, prior, result);
                    DecodeRow<2>(sig, prior, result);
                    DecodeRow<3>(sig, prior, result);
                } else {
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
                }
            } else {
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
    template <uint32_t Row>
    __aicore__ inline void DecodeRow(LocalTensor<float> sig, LocalTensor<float> prior, LocalTensor<float> result)
    {
        constexpr uint32_t row = Row;
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
