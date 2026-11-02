#pragma once

template <uint32_t Tile, uint32_t Pitch, uint32_t ColumnAddr, uint32_t PackedAddr, bool Pack, bool HalfNative = false, uint32_t HalfAddr = 0>
__aicore__ inline uint64_t CandidateAddressWord(uint32_t phase, uint32_t word, bool source)
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t Stripe = HalfNative ? Tile / 8 : Rows / 8;
    uint64_t result = 0;
    #pragma unroll
    for (uint32_t lane = 0; lane < 4; ++lane) {
        const uint32_t slot = word * 4 + lane;
        const uint32_t j = source ? slot % 8 : slot / 2;
        uint32_t address;
        if constexpr (Pack) {
            if (source) {
                const uint32_t field = (phase * 8 + j) % 5;
                const uint32_t stripe = (phase * 8 + j) / 5;
                address = ColumnAddr + (field * Pitch + stripe * Stripe + (slot / 8) * (HalfNative ? Tile / 16 : Rows)) * 4;
            } else {
                address = PackedAddr + j * 160 + phase * 32 + (slot % 2) * Rows * 20;
            }
        } else if constexpr (HalfNative) {
            if (source) {
                address = slot * 96 + phase * 32;
            } else {
                const uint32_t field = (phase * 16 + slot) % 6;
                const uint32_t stripe = (phase * 16 + slot) / 6;
                address = HalfAddr + (field * Pitch + stripe * (Tile / 8)) * 2;
            }
        } else {
            if (source) {
                address = j * 192 + phase * 32 + (slot / 8) * Rows * 24;
            } else {
                const uint32_t field = (phase * 8 + j) % 6;
                const uint32_t stripe = (phase * 8 + j) / 6;
                address = ColumnAddr + (field * Pitch + stripe * Stripe + (slot % 2) * Rows) * 4;
            }
        }
        result |= uint64_t(address / 32) << (lane * 16);
    }
    return result;
}

template <uint32_t Tile, uint32_t Pitch, uint32_t ColumnAddr, uint32_t PackedAddr, uint32_t CacheAddr, bool HalfNative = false, uint32_t HalfAddr = 0>
__aicore__ inline void BuildCandidateAddressCache()
{
    constexpr uint32_t InputPhases = HalfNative ? 3 : 6;
    auto table = LocalTensor<uint64_t>(TPosition::VECCALC, CacheAddr, (InputPhases + 5) * 8);
    #pragma unroll
    for (uint32_t phase = 0; phase < InputPhases; ++phase)
        #pragma unroll
        for (uint32_t word = 0; word < 4; ++word) {
            table.SetValue(phase * 8 + word,
                CandidateAddressWord<Tile, Pitch, ColumnAddr, PackedAddr, false, HalfNative, HalfAddr>(phase, word, false));
            table.SetValue(phase * 8 + 4 + word,
                CandidateAddressWord<Tile, Pitch, ColumnAddr, PackedAddr, false, HalfNative, HalfAddr>(phase, word, true));
        }
    #pragma unroll
    for (uint32_t phase = 0; phase < 5; ++phase)
        #pragma unroll
        for (uint32_t word = 0; word < 4; ++word) {
            table.SetValue((phase + InputPhases) * 8 + word,
                CandidateAddressWord<Tile, Pitch, ColumnAddr, PackedAddr, true, HalfNative, HalfAddr>(phase, word, false));
            table.SetValue((phase + InputPhases) * 8 + 4 + word,
                CandidateAddressWord<Tile, Pitch, ColumnAddr, PackedAddr, true, HalfNative, HalfAddr>(phase, word, true));
        }
    SetFlag<HardEvent::S_V>(0);
    WaitFlag<HardEvent::S_V>(0);
}

template <uint32_t Tile, uint32_t CacheAddr, bool Pack, bool HalfNative = false>
__aicore__ inline void TransposeCachedCandidates()
{
    constexpr uint32_t Rows = Tile / 2;
    constexpr uint32_t InputPhases = HalfNative ? 3 : 6;
    #pragma unroll
    for (uint32_t phase = 0; phase < (Pack ? 5u : InputPhases); ++phase) {
        const uint32_t address = CacheAddr + (phase + (Pack ? InputPhases : 0)) * 64;
        auto target = LocalTensor<uint64_t>(TPosition::VECCALC, address, 4);
        auto source = LocalTensor<uint64_t>(TPosition::VECCALC, address + 32, 4);
        if constexpr (HalfNative && !Pack) {
            TransDataTo5HDVldVaRegImpl<half>(
                (__ubuf__ uint64_t *)target.GetPhyAddr(), (__ubuf__ uint64_t *)source.GetPhyAddr(),
                TransDataTo5HDParams{false, false, uint8_t(Tile / 128), 1, 48});
        } else {
            TransDataTo5HDVldVaRegImpl<float>(
                (__ubuf__ uint64_t *)target.GetPhyAddr(), (__ubuf__ uint64_t *)source.GetPhyAddr(),
                TransDataTo5HDParams{false, false, uint8_t(Rows / 64),
                    uint16_t(Pack ? 40 : 1), uint16_t(Pack ? 1 : 48)});
        }
    }
}
