#pragma once

template <uint32_t Pitch, uint32_t Tile>
__aicore__ inline void DecodeCandidateGeometry(LocalTensor<float> c, uint32_t count)
{
    if (count * 2 <= Tile) {
        for (uint32_t j = 0; j < 4; ++j)
            Muls(c[j * Pitch], c[j * Pitch], 2.0f, count);
        PipeBarrier<PIPE_V>();
        for (uint32_t j = 0; j < 2; ++j)
            Adds(c[j * Pitch], c[j * Pitch], -0.5f, count);
        Mul(c[2 * Pitch], c[2 * Pitch], c[2 * Pitch], count);
        Mul(c[3 * Pitch], c[3 * Pitch], c[3 * Pitch], count);
        Mul(c[4 * Pitch], c[4 * Pitch], c[5 * Pitch], count);
        PipeBarrier<PIPE_V>();
        Add(c, c, c[6 * Pitch], count);
        Add(c[Pitch], c[Pitch], c[7 * Pitch], count);
        Mul(c[2 * Pitch], c[2 * Pitch], c[9 * Pitch], count);
        Mul(c[3 * Pitch], c[3 * Pitch], c[10 * Pitch], count);
        PipeBarrier<PIPE_V>();
    } else {
        Muls(c, c, 2.0f, Pitch * 4);
        PipeBarrier<PIPE_V>();
        Adds(c, c, -0.5f, Pitch * 2);
        Mul(c[2 * Pitch], c[2 * Pitch], c[2 * Pitch], Pitch * 2);
        Mul(c[4 * Pitch], c[4 * Pitch], c[5 * Pitch], count);
        PipeBarrier<PIPE_V>();
        Add(c, c, c[6 * Pitch], Pitch * 2);
        Mul(c[2 * Pitch], c[2 * Pitch], c[9 * Pitch], Pitch * 2);
        PipeBarrier<PIPE_V>();
    }
    for (uint32_t j = 0; j < 4; ++j)
        Mul(c[j * Pitch], c[j * Pitch], c[8 * Pitch], count);
    PipeBarrier<PIPE_V>();
}
