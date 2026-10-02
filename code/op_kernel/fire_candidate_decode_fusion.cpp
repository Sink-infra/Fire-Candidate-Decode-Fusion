#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

using namespace AscendC;

template <class T>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t TILE = 256;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR decoded,
                               const FireCandidateDecodeFusionTilingData &data, GM_ADDR workspace) {
        const uint64_t stamp = GetSystemCycle();
        workspace_ = workspace;
        for (uint32_t k = 0; k < 8; ++k) times_[k] = 0;
        pipe_.InitBuffer(clockBuf_, 64);
        n_ = data.candidates;
        const uint32_t coreEnd = (GetBlockIdx() + 1) * data.perCore;
        end_ = data.total < coreEnd ? data.total : coreEnd;
        begin_ = GetBlockIdx() * data.perCore;
        rawGm_.SetGlobalBuffer((__gm__ T *)raw);
        priorGm_.SetGlobalBuffer((__gm__ float *)prior);
        outGm_.SetGlobalBuffer((__gm__ T *)decoded);
        pipe_.InitBuffer(rawQ_, 1, TILE * 6 * sizeof(T));
        pipe_.InitBuffer(priorQ_, 1, TILE * 5 * sizeof(float));
        pipe_.InitBuffer(outQ_, 1, TILE * 5 * sizeof(T));
        pipe_.InitBuffer(rawF_, TILE * 6 * sizeof(float));
        pipe_.InitBuffer(channels_, TILE * 6 * sizeof(float));
        pipe_.InitBuffer(priorF_, TILE * 5 * sizeof(float));
        pipe_.InitBuffer(aos_, TILE * 5 * sizeof(float));
        pipe_.InitBuffer(ones_, TILE * 6 * sizeof(float));
        pipe_.InitBuffer(offsets_, TILE * 7 * sizeof(uint32_t));
        auto offsets = offsets_.Get<uint32_t>();
        auto index = aos_.Get<float>();
        auto temporary = channels_.Get<float>();
        CreateVecIndex(index, 0.0f, TILE);
        PipeBarrier<PIPE_V>();
        Muls(temporary, index, 24.0f, TILE);
        PipeBarrier<PIPE_V>();
        Cast(offsets.ReinterpretCast<int32_t>(), temporary, RoundMode::CAST_RINT, TILE);
        Muls(temporary, index, 20.0f, TILE);
        PipeBarrier<PIPE_V>();
        Cast(offsets[TILE].ReinterpretCast<int32_t>(), temporary, RoundMode::CAST_RINT, TILE);
        CreateVecIndex(index, 0.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Muls(temporary, index, 0.2f, TILE * 5);
        PipeBarrier<PIPE_V>();
        auto outputIndex = offsets[TILE * 2].ReinterpretCast<int32_t>();
        Cast(outputIndex, temporary, RoundMode::CAST_FLOOR, TILE * 5);
        PipeBarrier<PIPE_V>();
        Cast(temporary, outputIndex, RoundMode::CAST_NONE, TILE * 5);
        Muls(index, index, 256.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Muls(temporary, temporary, -1279.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Add(index, index, temporary, TILE * 5);
        PipeBarrier<PIPE_V>();
        Muls(index, index, 4.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Cast(outputIndex, index, RoundMode::CAST_RINT, TILE * 5);
        PipeBarrier<PIPE_V>();
        Duplicate(ones_.Get<float>(), 1.0f, TILE * 6);
        PipeBarrier<PIPE_ALL>();
        times_[0] = GetSystemCycle() - stamp;
    }

    __aicore__ inline void Process() {
        for (uint32_t start = begin_; start < end_;) {
            PipeBarrier<PIPE_ALL>();
            uint64_t stamp = GetSystemCycle();
            uint64_t now;
            const uint32_t priorStart = start % n_;
            uint32_t count = end_ - start;
            if (count > n_ - priorStart) count = n_ - priorStart;
            if (count > TILE) count = TILE;
            auto raw = rawQ_.AllocTensor<T>();
            auto prior = priorQ_.AllocTensor<float>();
            DataCopyPad(raw, rawGm_[start * 6],
                        {1, static_cast<uint32_t>(count * 6 * sizeof(T)), 0, 0, 0}, {false, 0, 0, 0});
            DataCopyPad(prior, priorGm_[priorStart * 5],
                        {1, count * 20, 0, 0, 0}, {false, 0, 0, 0});
            rawQ_.EnQue(raw);
            priorQ_.EnQue(prior);
            raw = rawQ_.DeQue<T>();
            prior = priorQ_.DeQue<float>();
            auto f = rawF_.Get<float>();
            if constexpr (IsSameType<T, half>::value) {
                Cast(f, raw, RoundMode::CAST_NONE, count * 6);
            } else {
                DataCopy(f, raw, ((count * 6 + 7) / 8) * 8);
            }
            PipeBarrier<PIPE_V>();
            PipeBarrier<PIPE_ALL>();
            now = GetSystemCycle();
            times_[1] += now - stamp; stamp = now;
            Muls(f, f, -1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Exp(f, f, count * 6);
            PipeBarrier<PIPE_V>();
            Adds(f, f, 1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Div(f, ones_.Get<float>(), f, count * 6);
            PipeBarrier<PIPE_V>();

            PipeBarrier<PIPE_ALL>();
            now = GetSystemCycle();
            times_[2] += now - stamp; stamp = now;
            auto c = channels_.Get<float>();
            auto p = priorF_.Get<float>();
            auto idx = offsets_.Get<uint32_t>();
            for (uint32_t k = 0; k < 6; ++k) {
                Gather(c[k * TILE], f, idx, k * sizeof(float), count);
            }
            for (uint32_t k = 0; k < 5; ++k) {
                Gather(p[k * TILE], prior, idx[TILE], k * sizeof(float), count);
            }
            PipeBarrier<PIPE_V>();
            PipeBarrier<PIPE_ALL>();
            now = GetSystemCycle();
            times_[3] += now - stamp; stamp = now;
            Muls(c, c, 2.0f, TILE * 4);
            PipeBarrier<PIPE_V>();
            Adds(c, c, -0.5f, TILE * 2);
            Mul(c[TILE * 2], c[TILE * 2], c[TILE * 2], TILE * 2);
            Mul(c[TILE * 4], c[TILE * 4], c[TILE * 5], count);
            PipeBarrier<PIPE_V>();
            Add(c, c, p, TILE * 2);
            Mul(c[TILE * 2], c[TILE * 2], p[TILE * 3], TILE * 2);
            PipeBarrier<PIPE_V>();
            for (uint32_t k = 0; k < 4; ++k) {
                Mul(c[k * TILE], c[k * TILE], p[TILE * 2], count);
            }
            PipeBarrier<PIPE_V>();
            auto result = aos_.Get<float>();
            Gather(result, c, idx[TILE * 2], 0, count * 5);
            PipeBarrier<PIPE_V>();
            PipeBarrier<PIPE_ALL>();
            now = GetSystemCycle();
            times_[4] += now - stamp; stamp = now;
            auto out = outQ_.AllocTensor<T>();
            if constexpr (IsSameType<T, half>::value) {
                Cast(out, result, RoundMode::CAST_RINT, count * 5);
            } else {
                DataCopy(out, result, ((count * 5 + 7) / 8) * 8);
            }
            outQ_.EnQue(out);
            rawQ_.FreeTensor(raw);
            priorQ_.FreeTensor(prior);
            out = outQ_.DeQue<T>();
            DataCopyPad(outGm_[start * 5], out,
                        {1, static_cast<uint32_t>(count * 5 * sizeof(T)), 0, 0, 0});
            outQ_.FreeTensor(out);
            PipeBarrier<PIPE_ALL>();
            now = GetSystemCycle();
            times_[5] += now - stamp; stamp = now;
            start += count;
        }
        auto clocks = clockBuf_.Get<uint64_t>();
        for (uint32_t k = 0; k < 6; ++k) clocks.SetValue(k, times_[k]);
        clocks.SetValue(6, GetBlockIdx());
        clocks.SetValue(7, 0xface1234);
        const auto event = static_cast<event_t>(pipe_.FetchEventID(HardEvent::S_MTE3));
        SetFlag<HardEvent::S_MTE3>(event); WaitFlag<HardEvent::S_MTE3>(event);
        GlobalTensor<uint64_t> clocksGm;
        clocksGm.SetGlobalBuffer((__gm__ uint64_t *)workspace_);
        DataCopy(clocksGm[GetBlockIdx() * 8], clocks, 8);
    }
private:
    TPipe pipe_;
    TBuf<QuePosition::VECCALC> clockBuf_;
    GM_ADDR workspace_;
    uint64_t times_[8];
    TQue<QuePosition::VECIN, 1> rawQ_, priorQ_;
    TQue<QuePosition::VECOUT, 1> outQ_;
    TBuf<QuePosition::VECCALC> rawF_, channels_, priorF_, aos_, ones_, offsets_;
    GlobalTensor<T> rawGm_, outGm_;
    GlobalTensor<float> priorGm_;
    uint32_t n_, begin_, end_;
};

template <typename DT_RAW_PRED>
__global__ __aicore__ void fire_candidate_decode_fusion(
    GM_ADDR raw_pred, GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling) {
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, data, tiling);
    KernelFireCandidateDecodeFusion<DT_RAW_PRED> op;
    op.Init(raw_pred, priors, decoded, data, GetUserWorkspace(workspace));
    op.Process();
}
