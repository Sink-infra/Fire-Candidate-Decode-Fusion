#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

using namespace AscendC;

template <bool EXTERNAL>
struct FirePipeType { using Type = TPipe; };

template <>
struct FirePipeType<true> { using Type = TPipe *; };

template <class T, uint32_t TILE, bool EXTERNAL, bool PIPELINED = false, bool EARLY = true, bool MASKED = false>
class KernelFireCandidateDecodeFusion {
    static constexpr uint32_t OUTPUT_INDEX = MASKED ? 3904 : TILE * 2;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR decoded,
                               const FireCandidateDecodeFusionTilingData &data, TPipe *pipe = nullptr) {
        if constexpr (EXTERNAL) pipe_ = pipe;
        n_ = data.candidates;
        const uint32_t coreEnd = (GetBlockIdx() + 1) * data.perCore;
        end_ = data.total < coreEnd ? data.total : coreEnd;
        begin_ = GetBlockIdx() * data.perCore;
        rawGm_.SetGlobalBuffer((__gm__ T *)raw);
        priorGm_.SetGlobalBuffer((__gm__ float *)prior);
        outGm_.SetGlobalBuffer((__gm__ T *)decoded);
        Pipeline()->InitBuffer(rawQ_, PIPELINED ? 2 : 1, TILE * 6 * sizeof(T));
        Pipeline()->InitBuffer(priorQ_, PIPELINED ? 2 : 1, TILE * 5 * sizeof(float));
        Pipeline()->InitBuffer(outQ_, 1, TILE * 5 * sizeof(T));
        Pipeline()->InitBuffer(rawF_, TILE * 6 * sizeof(float));
        Pipeline()->InitBuffer(channels_, TILE * 6 * sizeof(float));
        Pipeline()->InitBuffer(priorF_, TILE * 5 * sizeof(float));
        Pipeline()->InitBuffer(aos_, TILE * 5 * sizeof(float));
        Pipeline()->InitBuffer(ones_, TILE * 6 * sizeof(float));
        Pipeline()->InitBuffer(offsets_, (OUTPUT_INDEX + TILE * 5) * sizeof(uint32_t));
        auto offsets = offsets_.Get<uint32_t>();
        auto index = aos_.Get<float>();
        auto temporary = channels_.Get<float>();
        if constexpr (MASKED) {
            InitMasks();
        } else {
            CreateVecIndex(index, 0.0f, TILE);
            PipeBarrier<PIPE_V>();
            Muls(temporary, index, 24.0f, TILE);
            PipeBarrier<PIPE_V>();
            Cast(offsets.ReinterpretCast<int32_t>(), temporary, RoundMode::CAST_RINT, TILE);
            Muls(temporary, index, 20.0f, TILE);
            PipeBarrier<PIPE_V>();
            Cast(offsets[TILE].ReinterpretCast<int32_t>(), temporary, RoundMode::CAST_RINT, TILE);
        }
        CreateVecIndex(index, 0.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Muls(temporary, index, 0.2f, TILE * 5);
        PipeBarrier<PIPE_V>();
        auto outputIndex = offsets[OUTPUT_INDEX].ReinterpretCast<int32_t>();
        Cast(outputIndex, temporary, RoundMode::CAST_FLOOR, TILE * 5);
        PipeBarrier<PIPE_V>();
        Cast(temporary, outputIndex, RoundMode::CAST_NONE, TILE * 5);
        Muls(index, index, TILE == 512 ? 512.0f : 256.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Muls(temporary, temporary, TILE == 512 ? -2559.0f : -1279.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Add(index, index, temporary, TILE * 5);
        PipeBarrier<PIPE_V>();
        Muls(index, index, 4.0f, TILE * 5);
        PipeBarrier<PIPE_V>();
        Cast(outputIndex, index, RoundMode::CAST_RINT, TILE * 5);
        PipeBarrier<PIPE_V>();
        Duplicate(ones_.Get<float>(), 1.0f, TILE * 6);
    }

    __aicore__ inline void InitMasks() {
        auto pattern = aos_.Get<uint32_t>();
        auto masks = offsets_.Get<uint32_t>();
        pattern.SetValue(0, 0x41041041u);
        pattern.SetValue(1, 0x10410410u);
        pattern.SetValue(8, 0x04104104u);
        pattern.SetValue(9, 0x41041041u);
        pattern.SetValue(16, 0x10410410u);
        pattern.SetValue(17, 0x04104104u);
        pattern.SetValue(24, 0x82082082u);
        pattern.SetValue(25, 0x20820820u);
        pattern.SetValue(32, 0x08208208u);
        pattern.SetValue(33, 0x82082082u);
        pattern.SetValue(40, 0x20820820u);
        pattern.SetValue(41, 0x08208208u);
        pattern.SetValue(48, 0x04104104u);
        pattern.SetValue(49, 0x41041041u);
        pattern.SetValue(56, 0x10410410u);
        pattern.SetValue(57, 0x04104104u);
        pattern.SetValue(64, 0x41041041u);
        pattern.SetValue(65, 0x10410410u);
        pattern.SetValue(72, 0x08208208u);
        pattern.SetValue(73, 0x82082082u);
        pattern.SetValue(80, 0x20820820u);
        pattern.SetValue(81, 0x08208208u);
        pattern.SetValue(88, 0x82082082u);
        pattern.SetValue(89, 0x20820820u);
        pattern.SetValue(96, 0x10410410u);
        pattern.SetValue(97, 0x04104104u);
        pattern.SetValue(104, 0x41041041u);
        pattern.SetValue(105, 0x10410410u);
        pattern.SetValue(112, 0x04104104u);
        pattern.SetValue(113, 0x41041041u);
        pattern.SetValue(120, 0x20820820u);
        pattern.SetValue(121, 0x08208208u);
        pattern.SetValue(128, 0x82082082u);
        pattern.SetValue(129, 0x20820820u);
        pattern.SetValue(136, 0x08208208u);
        pattern.SetValue(137, 0x82082082u);
        pattern.SetValue(144, 0x42108421u);
        pattern.SetValue(145, 0x10842108u);
        pattern.SetValue(152, 0x84210842u);
        pattern.SetValue(153, 0x21084210u);
        pattern.SetValue(160, 0x08421084u);
        pattern.SetValue(161, 0x42108421u);
        pattern.SetValue(168, 0x10842108u);
        pattern.SetValue(169, 0x84210842u);
        pattern.SetValue(176, 0x21084210u);
        pattern.SetValue(177, 0x08421084u);
        pattern.SetValue(184, 0x84210842u);
        pattern.SetValue(185, 0x21084210u);
        pattern.SetValue(192, 0x08421084u);
        pattern.SetValue(193, 0x42108421u);
        pattern.SetValue(200, 0x10842108u);
        pattern.SetValue(201, 0x84210842u);
        pattern.SetValue(208, 0x21084210u);
        pattern.SetValue(209, 0x08421084u);
        pattern.SetValue(216, 0x42108421u);
        pattern.SetValue(217, 0x10842108u);
        pattern.SetValue(224, 0x08421084u);
        pattern.SetValue(225, 0x42108421u);
        pattern.SetValue(232, 0x10842108u);
        pattern.SetValue(233, 0x84210842u);
        pattern.SetValue(240, 0x21084210u);
        pattern.SetValue(241, 0x08421084u);
        pattern.SetValue(248, 0x42108421u);
        pattern.SetValue(249, 0x10842108u);
        pattern.SetValue(256, 0x84210842u);
        pattern.SetValue(257, 0x21084210u);
        pattern.SetValue(264, 0x10842108u);
        pattern.SetValue(265, 0x84210842u);
        pattern.SetValue(272, 0x21084210u);
        pattern.SetValue(273, 0x08421084u);
        pattern.SetValue(280, 0x42108421u);
        pattern.SetValue(281, 0x10842108u);
        pattern.SetValue(288, 0x84210842u);
        pattern.SetValue(289, 0x21084210u);
        pattern.SetValue(296, 0x08421084u);
        pattern.SetValue(297, 0x42108421u);
        pattern.SetValue(304, 0x21084210u);
        pattern.SetValue(305, 0x08421084u);
        pattern.SetValue(312, 0x42108421u);
        pattern.SetValue(313, 0x10842108u);
        pattern.SetValue(320, 0x84210842u);
        pattern.SetValue(321, 0x21084210u);
        pattern.SetValue(328, 0x08421084u);
        pattern.SetValue(329, 0x42108421u);
        pattern.SetValue(336, 0x10842108u);
        pattern.SetValue(337, 0x84210842u);
        PipeBarrier<PIPE_ALL>();
        ShiftLeft(masks[0], pattern[0], uint32_t(0), uint64_t(24), uint8_t(16), {1, 1, 3, 0});
        ShiftLeft(masks[384], pattern[24], uint32_t(0), uint64_t(24), uint8_t(16), {1, 1, 3, 0});
        ShiftLeft(masks[768], pattern[48], uint32_t(0), uint64_t(24), uint8_t(16), {1, 1, 3, 0});
        ShiftLeft(masks[1152], pattern[72], uint32_t(0), uint64_t(24), uint8_t(16), {1, 1, 3, 0});
        ShiftLeft(masks[1536], pattern[96], uint32_t(0), uint64_t(24), uint8_t(16), {1, 1, 3, 0});
        ShiftLeft(masks[1920], pattern[120], uint32_t(0), uint64_t(24), uint8_t(16), {1, 1, 3, 0});
        ShiftLeft(masks[2304], pattern[144], uint32_t(0), uint64_t(40), uint8_t(8), {1, 1, 5, 0});
        ShiftLeft(masks[2624], pattern[184], uint32_t(0), uint64_t(40), uint8_t(8), {1, 1, 5, 0});
        ShiftLeft(masks[2944], pattern[224], uint32_t(0), uint64_t(40), uint8_t(8), {1, 1, 5, 0});
        ShiftLeft(masks[3264], pattern[264], uint32_t(0), uint64_t(40), uint8_t(8), {1, 1, 5, 0});
        ShiftLeft(masks[3584], pattern[304], uint32_t(0), uint64_t(40), uint8_t(8), {1, 1, 5, 0});
        PipeBarrier<PIPE_V>();
    }

    __aicore__ inline void Process() {
        if constexpr (PIPELINED) ProcessPipeline();
        else ProcessSingle();
    }

    __aicore__ inline void ProcessSingle() {
        for (uint32_t start = begin_; start < end_;) {
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
            Muls(f, f, -1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Exp(f, f, count * 6);
            PipeBarrier<PIPE_V>();
            Adds(f, f, 1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Div(f, ones_.Get<float>(), f, count * 6);
            PipeBarrier<PIPE_V>();

            auto c = channels_.Get<float>();
            auto p = priorF_.Get<float>();
            auto idx = offsets_.Get<uint32_t>();
            if constexpr (MASKED) {
                uint64_t selected = 0;
                const uint16_t rawRepeats = (count * 6 + 63) / 64;
                const uint16_t priorRepeats = (count * 5 + 63) / 64;
                for (uint32_t k = 0; k < 6; ++k) {
                    GatherMask(c[k * TILE], f, idx[k * 384], false, 0,
                               {1, rawRepeats, 8, 1}, selected);
                }
                for (uint32_t k = 0; k < 5; ++k) {
                    GatherMask(p[k * TILE], prior, idx[2304 + k * 320], false, 0,
                               {1, priorRepeats, 8, 1}, selected);
                }
            } else {
                for (uint32_t k = 0; k < 6; ++k) {
                    Gather(c[k * TILE], f, idx, k * sizeof(float), count);
                }
                for (uint32_t k = 0; k < 5; ++k) {
                    Gather(p[k * TILE], prior, idx[TILE], k * sizeof(float), count);
                }
            }
            PipeBarrier<PIPE_V>();
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
            Gather(result, c, idx[OUTPUT_INDEX], 0, count * 5);
            PipeBarrier<PIPE_V>();
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
            start += count;
        }
    }
    __aicore__ inline uint32_t ChunkCount(uint32_t start) {
        uint32_t count = end_ - start;
        const uint32_t remain = n_ - start % n_;
        if (count > remain) count = remain;
        if (count > TILE) count = TILE;
        return count;
    }

    __aicore__ inline void CopyInput(uint32_t start, uint32_t count) {
        const uint32_t priorStart = start % n_;
        auto raw = rawQ_.AllocTensor<T>();
        auto prior = priorQ_.AllocTensor<float>();
        DataCopyPad(raw, rawGm_[start * 6],
                    {1, static_cast<uint32_t>(count * 6 * sizeof(T)), 0, 0, 0}, {false, 0, 0, 0});
        DataCopyPad(prior, priorGm_[priorStart * 5],
                    {1, count * 20, 0, 0, 0}, {false, 0, 0, 0});
        rawQ_.EnQue(raw);
        priorQ_.EnQue(prior);
    }

    __aicore__ inline void ProcessPipeline() {
        if constexpr (EARLY) ProcessPipelineEarly();
        else ProcessPipelineLate();
    }

    __aicore__ inline void ProcessPipelineEarly() {
        if (begin_ >= end_) return;
        uint32_t start = begin_;
        uint32_t count = ChunkCount(start);
        CopyInput(start, count);
        for (; start < end_;) {
            auto raw = rawQ_.DeQue<T>();
            auto prior = priorQ_.DeQue<float>();
            const uint32_t next = start + count;
            uint32_t nextCount = 0;
            if (next < end_) {
                nextCount = ChunkCount(next);
                CopyInput(next, nextCount);
            }
            auto f = rawF_.Get<float>();
            if constexpr (IsSameType<T, half>::value) {
                Cast(f, raw, RoundMode::CAST_NONE, count * 6);
            } else {
                DataCopy(f, raw, ((count * 6 + 7) / 8) * 8);
            }
            PipeBarrier<PIPE_V>();
            Muls(f, f, -1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Exp(f, f, count * 6);
            PipeBarrier<PIPE_V>();
            Adds(f, f, 1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Div(f, ones_.Get<float>(), f, count * 6);
            PipeBarrier<PIPE_V>();

            auto c = channels_.Get<float>();
            auto p = priorF_.Get<float>();
            auto idx = offsets_.Get<uint32_t>();
            if constexpr (MASKED) {
                uint64_t selected = 0;
                const uint16_t rawRepeats = (count * 6 + 63) / 64;
                const uint16_t priorRepeats = (count * 5 + 63) / 64;
                for (uint32_t k = 0; k < 6; ++k) {
                    GatherMask(c[k * TILE], f, idx[k * 384], false, 0,
                               {1, rawRepeats, 8, 1}, selected);
                }
                for (uint32_t k = 0; k < 5; ++k) {
                    GatherMask(p[k * TILE], prior, idx[2304 + k * 320], false, 0,
                               {1, priorRepeats, 8, 1}, selected);
                }
            } else {
                for (uint32_t k = 0; k < 6; ++k) {
                    Gather(c[k * TILE], f, idx, k * sizeof(float), count);
                }
                for (uint32_t k = 0; k < 5; ++k) {
                    Gather(p[k * TILE], prior, idx[TILE], k * sizeof(float), count);
                }
            }
            PipeBarrier<PIPE_V>();
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
            Gather(result, c, idx[OUTPUT_INDEX], 0, count * 5);
            PipeBarrier<PIPE_V>();
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
            start = next;
            count = nextCount;
        }
    }

    __aicore__ inline void ProcessPipelineLate() {
        if (begin_ >= end_) return;
        uint32_t start = begin_;
        uint32_t count = ChunkCount(start);
        CopyInput(start, count);
        for (; start < end_;) {
            auto raw = rawQ_.DeQue<T>();
            auto prior = priorQ_.DeQue<float>();
            auto f = rawF_.Get<float>();
            if constexpr (IsSameType<T, half>::value) {
                Cast(f, raw, RoundMode::CAST_NONE, count * 6);
            } else {
                DataCopy(f, raw, ((count * 6 + 7) / 8) * 8);
            }
            PipeBarrier<PIPE_V>();
            Muls(f, f, -1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Exp(f, f, count * 6);
            PipeBarrier<PIPE_V>();
            Adds(f, f, 1.0f, count * 6);
            PipeBarrier<PIPE_V>();
            Div(f, ones_.Get<float>(), f, count * 6);
            PipeBarrier<PIPE_V>();

            auto c = channels_.Get<float>();
            auto p = priorF_.Get<float>();
            auto idx = offsets_.Get<uint32_t>();
            if constexpr (MASKED) {
                uint64_t selected = 0;
                const uint16_t rawRepeats = (count * 6 + 63) / 64;
                const uint16_t priorRepeats = (count * 5 + 63) / 64;
                for (uint32_t k = 0; k < 6; ++k) {
                    GatherMask(c[k * TILE], f, idx[k * 384], false, 0,
                               {1, rawRepeats, 8, 1}, selected);
                }
                for (uint32_t k = 0; k < 5; ++k) {
                    GatherMask(p[k * TILE], prior, idx[2304 + k * 320], false, 0,
                               {1, priorRepeats, 8, 1}, selected);
                }
            } else {
                for (uint32_t k = 0; k < 6; ++k) {
                    Gather(c[k * TILE], f, idx, k * sizeof(float), count);
                }
                for (uint32_t k = 0; k < 5; ++k) {
                    Gather(p[k * TILE], prior, idx[TILE], k * sizeof(float), count);
                }
            }
            PipeBarrier<PIPE_V>();
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
            Gather(result, c, idx[OUTPUT_INDEX], 0, count * 5);
            PipeBarrier<PIPE_V>();
            auto out = outQ_.AllocTensor<T>();
            if constexpr (IsSameType<T, half>::value) {
                Cast(out, result, RoundMode::CAST_RINT, count * 5);
            } else {
                DataCopy(out, result, ((count * 5 + 7) / 8) * 8);
            }
            outQ_.EnQue(out);
            rawQ_.FreeTensor(raw);
            priorQ_.FreeTensor(prior);
            const uint32_t next = start + count;
            uint32_t nextCount = 0;
            if (next < end_) {
                nextCount = ChunkCount(next);
                CopyInput(next, nextCount);
            }
            out = outQ_.DeQue<T>();
            DataCopyPad(outGm_[start * 5], out,
                        {1, static_cast<uint32_t>(count * 5 * sizeof(T)), 0, 0, 0});
            outQ_.FreeTensor(out);
            start = next;
            count = nextCount;
        }
    }

private:
    __aicore__ inline TPipe *Pipeline() {
        if constexpr (EXTERNAL) return pipe_;
        else return &pipe_;
    }
    typename FirePipeType<EXTERNAL>::Type pipe_;
    TQue<QuePosition::VECIN, 1> rawQ_, priorQ_;
    TQue<QuePosition::VECOUT, 1> outQ_;
    TBuf<QuePosition::VECCALC> rawF_, channels_, priorF_, aos_, ones_, offsets_;
    GlobalTensor<T> rawGm_, outGm_;
    GlobalTensor<float> priorGm_;
    uint32_t n_, begin_, end_;
};

template <typename DT_RAW_PRED, uint32_t EXEC_PATH>
__global__ __aicore__ void fire_candidate_decode_fusion(
    GM_ADDR raw_pred, GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling) {
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, data, tiling);
    if constexpr (EXEC_PATH != 0) {
        if constexpr (IsSameType<DT_RAW_PRED, float>::value) {
            TPipe pipe;
            KernelFireCandidateDecodeFusion<DT_RAW_PRED, 512, true, true, EXEC_PATH == 1, EXEC_PATH == 3> op;
            op.Init(raw_pred, priors, decoded, data, &pipe);
            op.Process();
        } else {
            KernelFireCandidateDecodeFusion<DT_RAW_PRED, 512, false, true, EXEC_PATH == 1> op;
            op.Init(raw_pred, priors, decoded, data);
            op.Process();
        }
    } else {
    if constexpr (IsSameType<DT_RAW_PRED, float>::value) {
        TPipe pipe;
        if (data.perCore > 256 && data.candidates > 256) {
            KernelFireCandidateDecodeFusion<DT_RAW_PRED, 512, true> op;
            op.Init(raw_pred, priors, decoded, data, &pipe);
            op.Process();
        } else {
            KernelFireCandidateDecodeFusion<DT_RAW_PRED, 256, true> op;
            op.Init(raw_pred, priors, decoded, data, &pipe);
            op.Process();
        }
    } else {
        if (data.perCore > 256 && data.candidates > 256) {
            KernelFireCandidateDecodeFusion<DT_RAW_PRED, 512, false> op;
            op.Init(raw_pred, priors, decoded, data);
            op.Process();
        } else {
            KernelFireCandidateDecodeFusion<DT_RAW_PRED, 256, false> op;
            op.Init(raw_pred, priors, decoded, data);
            op.Process();
        }
    }
    }
}
