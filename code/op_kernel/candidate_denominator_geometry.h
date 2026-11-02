#pragma once

template <uint32_t Tile, uint32_t Pitch, bool FastConfidence = false, bool ReplicatedStride = false, bool TransformedPriors = false>
__aicore__ inline void DecodeCandidateDenominators(LocalTensor<float> columns,
    LocalTensor<float> two, LocalTensor<float> one, LocalTensor<float> firstStride)
{
    static_assert(Tile >= 1024 && Tile % 64 == 0 && (Pitch == Tile + 8 || Pitch == Tile + 16));
    SetMaskCount();
    SetVectorMask<float, MaskMode::COUNTER>(Tile);
    Mul<float, false>(columns[4 * Pitch], columns[4 * Pitch], columns[5 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    SetVectorMask<float, MaskMode::COUNTER>(Pitch * 4);
    Div<float, false>(columns, two, columns, MASK_PLACEHOLDER, 1, {1, 0, 1, 8, 0, 8});
    SetVectorMask<float, MaskMode::COUNTER>(Tile);
    if constexpr (FastConfidence) {
        Reciprocal<float, false>(columns[4 * Pitch], columns[4 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 8, 8});
    } else {
        Div<float, false>(columns[4 * Pitch], one, columns[4 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 0, 1, 8, 0, 8});
    }
    SetVectorMask<float, MaskMode::COUNTER>(Pitch * 2);
    if constexpr (TransformedPriors) {
        Mul<float, false>(columns, columns, firstStride,
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[2 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        Add<float, false>(columns, columns, columns[6 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[9 * Pitch],
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    } else {
    Adds<float, false>(columns, columns, -0.5f, MASK_PLACEHOLDER, 1, {1, 1, 8, 8});
    Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[2 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    Add<float, false>(columns, columns, columns[6 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    Mul<float, false>(columns[2 * Pitch], columns[2 * Pitch], columns[9 * Pitch],
        MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    if constexpr (ReplicatedStride) {
        SetVectorMask<float, MaskMode::COUNTER>(Pitch * 4);
        Mul<float, false>(columns, columns, firstStride,
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    } else {
        SetVectorMask<float, MaskMode::COUNTER>(Tile);
        Mul<float, false>(columns, columns, firstStride,
            MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
        for (uint32_t field = 1; field < 4; ++field)
            Mul<float, false>(columns[field * Pitch], columns[field * Pitch], columns[8 * Pitch],
                MASK_PLACEHOLDER, 1, {1, 1, 1, 8, 8, 8});
    }
    }
    SetMaskNorm();
    ResetMask();
}

template <uint32_t Tile, uint32_t Pitch, bool FastConfidence = false>
__aicore__ inline void DecodeCandidateDenominators(LocalTensor<float> columns,
    LocalTensor<float> two, LocalTensor<float> one)
{
    DecodeCandidateDenominators<Tile, Pitch, FastConfidence>(columns, two, one, columns[8 * Pitch]);
}
