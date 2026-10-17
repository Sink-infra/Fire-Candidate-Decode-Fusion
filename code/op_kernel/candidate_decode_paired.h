#pragma once
#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"

using namespace AscendC;

template <typename T, uint32_t Tile, bool CounterPrior = false, bool FastRamp = false, bool RowScratch = true, uint32_t BatchGroup = 2, bool BlockedRaw = false>
class PairedBatchCandidateDecode {
    static constexpr uint32_t HalfTile = Tile / BatchGroup;
    static constexpr uint32_t Pitch = Tile + 8;
    static constexpr bool Reuse = sizeof(T) == 4;
    static constexpr uint32_t RawBytes = Tile * 6 * sizeof(T);
    static constexpr uint32_t FloatAddr = RawBytes;
    static constexpr uint32_t SharedAddr = FloatAddr + (Reuse ? 0 : Tile * 24);
    static constexpr uint32_t SharedBytes = Tile * 5 * sizeof(T);
    static constexpr uint32_t ColumnAddr = SharedAddr + SharedBytes;
    static constexpr uint32_t OneAddr = ColumnAddr + Pitch * 44;
    static constexpr uint32_t RawIdxAddr = OneAddr + 256;
    static constexpr uint32_t PriorIdxAddr = RawIdxAddr + Tile * 4;
    static constexpr uint32_t PriorRows = FastRamp ? ((HalfTile + 63) / 64) * 64 : HalfTile;
    static constexpr uint32_t PackIdxAddr = PriorIdxAddr + PriorRows * 4;
    static constexpr uint32_t RampCacheAddr = RowScratch ? ColumnAddr + Tile * 20 : 0;
public:
    __aicore__ inline void Init(GM_ADDR raw, GM_ADDR prior, GM_ADDR out,
                               const FireCandidateDecodeFusionTilingData &d)
    {
        batch_ = d.batch; n_ = d.candidates; cores_ = d.cores;
        raw_.SetGlobalBuffer((__gm__ T *)raw);
        prior_.SetGlobalBuffer((__gm__ float *)prior);
        out_.SetGlobalBuffer((__gm__ T *)out);
        auto ri = At<int32_t>(RawIdxAddr, Tile);
        auto pi = At<int32_t>(PriorIdxAddr, HalfTile);
        if constexpr (FastRamp) {
            BuildRampCache();
            if constexpr (!BlockedRaw) FillRamp(ri, Tile);
            FillRamp(At<int32_t>(PriorIdxAddr, PriorRows), PriorRows);
        } else {
        if constexpr (!BlockedRaw) CreateVecIndex(ri, int32_t(0), Tile);
        CreateVecIndex(pi, int32_t(0), HalfTile);
        }
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        if constexpr (!BlockedRaw) Muls(ri, ri, int32_t(24), Tile);
        Muls(pi, pi, int32_t(20), HalfTile);
        auto seq = At<int32_t>(ColumnAddr, Pitch * 11);
        auto row = seq[Tile * 5];
        auto index = At<int32_t>(PackIdxAddr, Tile * 5);
        constexpr uint32_t seedCount = Tile >= 1024 ? 320 : Tile * 5;
        if constexpr (FastRamp) FillRamp(seq, seedCount);
        else CreateVecIndex(seq, int32_t(0), seedCount);
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Muls(row, seq, int32_t(13108), seedCount);
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        ShiftRight(row, row, int32_t(16), seedCount);
        Muls(seq, seq, int32_t(4 * Pitch), seedCount);
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Muls(row, row, int32_t(4 - 20 * Pitch), seedCount);
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Add(index, seq, row, seedCount);
        if constexpr (BlockedRaw) {
            auto quarter = At<int32_t>(ColumnAddr + Tile * 24, seedCount);
            FillShortRamp(row, seedCount);
            Muls(row, row, int32_t(13108), seedCount);
            ShiftRight(row, row, int32_t(16), seedCount);
            ShiftRight(quarter, row, int32_t(2), seedCount);
            Muls(row, row, int32_t(60), seedCount);
            Muls(quarter, quarter, int32_t(252), seedCount);
            Add(index, index, row, seedCount);
            Sub(index, index, quarter, seedCount);
        }
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        for (uint32_t filled = seedCount; filled < Tile * 5; filled *= 2) {
            const uint32_t remaining = Tile * 5 - filled;
            const uint32_t count = filled < remaining ? filled : remaining;
            Adds(index[filled], index, int32_t(filled / 5 * 4), count);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        }
        if constexpr (BlockedRaw) {
            FillShortRamp(pi, PriorRows);
            auto lo = At<int32_t>(ColumnAddr, PriorRows);
            auto hi = At<int32_t>(ColumnAddr + PriorRows * 4, PriorRows);
            ShiftRight(lo, pi, int32_t(4), PriorRows);
            ShiftRight(hi, pi, int32_t(6), PriorRows);
            Muls(pi, pi, int32_t(4), PriorRows);
            Muls(lo, lo, int32_t(63), PriorRows);
            Muls(hi, hi, int32_t(60), PriorRows);
            Sub(pi, pi, lo, PriorRows);
            Add(pi, pi, hi, PriorRows);
            Muls(pi, pi, int32_t(20), PriorRows);
            const uint64_t input = At<float>(Reuse ? 0 : FloatAddr, Tile * 6).GetPhyAddr();
            const uint64_t columns = At<float>(ColumnAddr, Pitch * 11).GetPhyAddr();
            for (uint32_t phase = 0; phase < 3; ++phase) {
                for (uint32_t i = 0; i < 16; ++i)
                    blockSrc_[phase * 16 + i] = input + (i * 3 + phase) * 32;
                for (uint32_t lane = 0; lane < 8; ++lane) {
                    const uint32_t value = phase * 8 + lane;
                    blockDst_[phase * 16 + lane * 2] = columns +
                        ((value % 6) * Pitch + (value / 6) * 16) * 4;
                    blockDst_[phase * 16 + lane * 2 + 1] =
                        blockDst_[phase * 16 + lane * 2] + 32;
                }
            }
        }
        Duplicate(At<float>(OneAddr, 64), 2.0f, 64);
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        if constexpr (FastRamp && !RowScratch) {
            SetFlag<HardEvent::V_MTE2>(rawFree_);
            WaitFlag<HardEvent::V_MTE2>(rawFree_);
        }
    }
    __aicore__ inline void Process()
    {
        const uint32_t groups = (n_ + 15) / 16;
        const uint32_t begin = (groups * GetBlockIdx() / cores_) * 16;
        uint32_t end = (groups * (GetBlockIdx() + 1) / cores_) * 16;
        if (end > n_) end = n_;
        for (uint32_t start = begin; start < end; start += HalfTile) {
            const uint32_t count = start + HalfTile <= end ? HalfTile : end - start;
            if (poolPending_) {
                WaitFlag<HardEvent::MTE3_MTE2>(poolFree_);
                poolPending_ = false;
            }
            auto prior = At<float>(SharedAddr, HalfTile * 5, TPosition::VECIN);
            DataCopyPad(prior, prior_[start * 5],
                DataCopyExtParams{1, count * 20, 0, 0, 0},
                DataCopyPadExtParams<float>{false, 0, 0, 0});
            SetFlag<HardEvent::MTE2_V>(priorReady_);
            WaitFlag<HardEvent::MTE2_V>(priorReady_);
            auto c = At<float>(ColumnAddr, Pitch * 11);
            for (uint32_t j = 0; j < 5; ++j) {
                if constexpr (CounterPrior)
                    Gather(c[(j + 6) * Pitch], prior, At<uint32_t>(PriorIdxAddr, HalfTile),
                        j * 4, count);
                else
                    Gather(c[(j + 6) * Pitch], prior, At<uint32_t>(PriorIdxAddr, HalfTile),
                        j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            }
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            for (uint32_t group = 1; group < BatchGroup; ++group)
                for (uint32_t j = 0; j < 5; ++j)
                    DataCopy(c[(j + 6) * Pitch + group * HalfTile], c[(j + 6) * Pitch], HalfTile);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            LoadRaw(0, start, count);
            for (uint32_t b = 0; b < batch_; b += BatchGroup) {
                WaitFlag<HardEvent::MTE2_V>(rawReady_);
                auto raw = At<T>(0, Tile * 6, TPosition::VECIN);
                Compute(raw, prior, b, start, count);
            }
        }
        if (outPending_) WaitFlag<HardEvent::MTE3_V>(outFree_);
        if (poolPending_) WaitFlag<HardEvent::MTE3_MTE2>(poolFree_);
        if (rawUsed_) WaitFlag<HardEvent::V_MTE2>(rawFree_);
    }
private:
    __aicore__ inline void BuildRampCache()
    {
        auto seed = At<int32_t>(OneAddr, 8);
        for (uint32_t i = 0; i < 8; ++i) seed.SetValue(i, int32_t(i));
        constexpr int32_t ready = 2;
        SetFlag<HardEvent::S_V>(ready);
        WaitFlag<HardEvent::S_V>(ready);
        auto lanes = At<int32_t>(RampCacheAddr, 64);
        Brcb(lanes, seed, uint8_t(1), BrcbRepeatParams{1, 8});
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Muls(lanes, lanes, int32_t(8), 64);
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Add(lanes, lanes, seed, uint64_t(64), uint8_t(1), {1, 1, 0, 8, 8, 0});
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        auto steps = At<int32_t>(RampCacheAddr + 256, 192);
        DataCopy(steps, lanes, 64);
        Adds(steps[64], lanes, int32_t(64), uint64_t(64), uint8_t(1), {1, 1, 8, 8});
        Adds(steps[128], lanes, int32_t(128), uint64_t(64), uint8_t(1), {1, 1, 8, 8});
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Muls(steps, steps, int32_t(64), 192);
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
    }
    __aicore__ inline void FillShortRamp(LocalTensor<int32_t> target, uint32_t elements)
    {
        auto seed = At<int32_t>(OneAddr, 8);
        Brcb(target, seed, uint8_t(1), BrcbRepeatParams{1, 8});
        Muls(target, target, int32_t(8), 64);
        Add(target, target, seed, uint64_t(64), uint8_t(1), {1, 1, 0, 8, 8, 0});
        for (uint32_t filled = 64; filled < elements; filled *= 2) {
            const uint32_t count = elements - filled < filled ? elements - filled : filled;
            Adds(target[filled], target, int32_t(filled), count);
        }
    }
    __aicore__ inline void FillRamp(LocalTensor<int32_t> target, uint32_t elements)
    {
        const uint32_t repeats = (elements + 63) / 64;
        auto spread = At<int32_t>(RampCacheAddr + 1024, ((repeats + 7) / 8) * 64);
        Brcb(spread, At<int32_t>(RampCacheAddr + 256, 192), uint8_t((repeats + 7) / 8),
            BrcbRepeatParams{1, 8});
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Brcb(target, spread, uint8_t(repeats), BrcbRepeatParams{1, 8});
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
        Add(target, target, At<int32_t>(RampCacheAddr, 64), uint64_t(64), uint8_t(repeats),
            {1, 1, 1, 8, 8, 0});
        if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
    }
    template <typename U>
    __aicore__ inline LocalTensor<U> At(uint32_t addr, uint32_t count, TPosition pos = TPosition::VECCALC)
    {
        return LocalTensor<U>(pos, addr, count);
    }
    __aicore__ inline void LoadRaw(uint32_t b, uint32_t start, uint32_t count)
    {
        if (rawUsed_) WaitFlag<HardEvent::V_MTE2>(rawFree_);
        auto raw = At<T>(0, Tile * 6, TPosition::VECIN);
        const uint32_t bytes = count * 6 * uint32_t(sizeof(T));
        const uint32_t localGap = (HalfTile * 6 * uint32_t(sizeof(T)) - ((bytes + 31) / 32) * 32) / 32;
        DataCopyPad(raw, raw_[(b * n_ + start) * 6],
            DataCopyExtParams{BatchGroup, bytes, (n_ - count) * 6 * uint32_t(sizeof(T)), localGap, 0},
            DataCopyPadExtParams<T>{false, 0, 0, 0});
        SetFlag<HardEvent::MTE2_V>(rawReady_);
        rawUsed_ = true;
    }
    __aicore__ inline void ReleaseRawAndPrefetch(uint32_t b, uint32_t start, uint32_t count)
    {
        SetFlag<HardEvent::V_MTE2>(rawFree_);
        if (b + BatchGroup < batch_) LoadRaw(b + BatchGroup, start, count);
    }
    __aicore__ inline void Compute(LocalTensor<T> raw, LocalTensor<float> prior,
                                  uint32_t b, uint32_t start, uint32_t validCount)
    {
            constexpr uint32_t count = Tile;
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
            if constexpr (!Reuse) ReleaseRawAndPrefetch(b, start, validCount);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            Muls(sig, sig, -1.0f, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            Exp(sig, sig, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            Adds(sig, sig, 1.0f, uint64_t(64), uint8_t((count * 6 + 63) / 64), {1, 1, 8, 8});
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            auto c = At<float>(ColumnAddr, Pitch * 11);
            const auto ri = At<uint32_t>(RawIdxAddr, Tile);

            if constexpr (BlockedRaw) {
                PipeBarrier<PIPE_V>();
                TransDataTo5HDParams params;
                params.dstHighHalf = false;
                params.srcHighHalf = false;
                params.repeatTimes = Tile / 64;
                params.dstRepStride = 8;
                params.srcRepStride = 48;
                for (uint32_t phase = 0; phase < 3; ++phase)
                    TransDataTo5HD<float>(&blockDst_[phase * 16], &blockSrc_[phase * 16], params);
                PipeBarrier<PIPE_V>();
            } else {
                for (uint32_t j = 0; j < 6; ++j)
                    Gather(c[j * Pitch], sig, ri, j * 4, uint64_t(64), uint8_t((count + 63) / 64), 8);
            }
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            if constexpr (Reuse) ReleaseRawAndPrefetch(b, start, validCount);
            Mul(c[4 * Pitch], c[4 * Pitch], c[5 * Pitch], count);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            Div(c, At<float>(OneAddr, 64), c, uint64_t(64),
                uint8_t((Pitch * 5 + 63) / 64), {1, 1, 1, 8, 0, 8});
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            Adds(c, c, -0.5f, Pitch * 2);
            Mul(c[2 * Pitch], c[2 * Pitch], c[2 * Pitch], Pitch * 2);
            Muls(c[4 * Pitch], c[4 * Pitch], 0.5f, count);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            Add(c, c, c[6 * Pitch], Pitch * 2);
            Mul(c[2 * Pitch], c[2 * Pitch], c[9 * Pitch], Pitch * 2);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            for (uint32_t j = 0; j < 4; ++j)
                Mul(c[j * Pitch], c[j * Pitch], c[8 * Pitch], count);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            if (outPending_) WaitFlag<HardEvent::MTE3_V>(outFree_);
            auto dst = At<T>(SharedAddr, Tile * 5, TPosition::VECOUT);
            LocalTensor<float> packed;
            if constexpr (Reuse) packed = dst;
            else packed = At<float>(FloatAddr, Tile * 6);
            Gather(packed, c, At<uint32_t>(PackIdxAddr, Tile * 5), 0, uint64_t(64),
                uint8_t((count * 5 + 63) / 64), 8);
            if constexpr (sizeof(T) == 2) PipeBarrier<PIPE_V>();
            if constexpr (sizeof(T) == 2)
                Cast(dst, packed, RoundMode::CAST_RINT, count * 5);
            else if constexpr (!Reuse)
                DataCopy(dst, packed, ((count * 5 + 7) / 8) * 8);
            SetFlag<HardEvent::V_MTE3>(outReady_);
            WaitFlag<HardEvent::V_MTE3>(outReady_);
            const uint32_t bytes = validCount * 5 * uint32_t(sizeof(T));
            const uint32_t localGap = (HalfTile * 5 * uint32_t(sizeof(T)) - ((bytes + 31) / 32) * 32) / 32;
            DataCopyPad(out_[(b * n_ + start) * 5], dst,
                DataCopyExtParams{BatchGroup, bytes, localGap, (n_ - validCount) * 5 * uint32_t(sizeof(T)), 0});
            SetFlag<HardEvent::MTE3_V>(outFree_);
            outPending_ = true;
            if (b + BatchGroup >= batch_) {
                SetFlag<HardEvent::MTE3_MTE2>(poolFree_);
                poolPending_ = true;
            }
    }

    GlobalTensor<T> raw_, out_;
    GlobalTensor<float> prior_;
    uint64_t blockSrc_[BlockedRaw ? 48 : 1], blockDst_[BlockedRaw ? 48 : 1];
    uint32_t batch_, n_, cores_;
    static constexpr int32_t rawReady_ = 2;
    static constexpr int32_t priorReady_ = 3;
    static constexpr int32_t rawFree_ = 2;
    static constexpr int32_t outReady_ = 2;
    static constexpr int32_t outFree_ = 2;
    static constexpr int32_t poolFree_ = 2;
    bool rawUsed_ = false, outPending_ = false, poolPending_ = false;
};
