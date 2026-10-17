#pragma once

template <uint32_t Elements, bool StaticEvents = false>
__aicore__ inline void BuildDoublingCandidateIndex(LocalTensor<int32_t> sequence,
    LocalTensor<int32_t> seed, TPipe *pipe)
{
    for (uint32_t i = 0; i < 8; ++i) seed.SetValue(i, int32_t(i));
    int32_t ready = 0;
    if constexpr (!StaticEvents) ready = pipe->FetchEventID(HardEvent::S_V);
    SetFlag<HardEvent::S_V>(ready);
    WaitFlag<HardEvent::S_V>(ready);
    Brcb(sequence, seed, uint8_t(1), BrcbRepeatParams{1, 8});
    PipeBarrier<PIPE_V>();
    Muls(sequence, sequence, int32_t(8), 64);
    PipeBarrier<PIPE_V>();
    Add(sequence, sequence, seed, uint64_t(64), uint8_t(1), {1, 1, 0, 8, 8, 0});
    PipeBarrier<PIPE_V>();
    #pragma unroll
    for (uint32_t filled = 64; filled < Elements; filled *= 2) {
        const uint32_t count = filled * 2 <= Elements ? filled : Elements - filled;
        Adds(sequence[filled], sequence, int32_t(filled), count);
        PipeBarrier<PIPE_V>();
    }
}
