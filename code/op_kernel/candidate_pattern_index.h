#pragma once

template <uint32_t Elements>
__aicore__ inline void ExpandCandidatePatternIndex(LocalTensor<int32_t> index)
{
    #pragma unroll
    for (uint32_t filled = 320; filled < Elements; filled *= 2) {
        const uint32_t count = filled * 2 <= Elements ? filled : Elements - filled;
        Adds(index[filled], index, int32_t((filled / 5) * 4), count);
        PipeBarrier<PIPE_V>();
    }
}
