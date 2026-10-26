#pragma once

template <typename T, bool Aligned>
__aicore__ inline void CopyCandidateInput(LocalTensor<T> local, GlobalTensor<T> global,
    uint32_t index, uint16_t groups, uint32_t count, uint32_t localPitch, uint32_t globalPitch)
{
    const uint32_t bytes = count * uint32_t(sizeof(T));
    if constexpr (Aligned) {
        DataCopy(local, global[index], DataCopyParams{groups, uint16_t(bytes / 32),
            uint16_t(groups == 1 ? 0 : (globalPitch - count) * sizeof(T) / 32),
            uint16_t(groups == 1 ? 0 : (localPitch - count) * sizeof(T) / 32)});
    } else {
        DataCopyPad(local, global[index],
            DataCopyExtParams{groups, bytes,
                groups == 1 ? 0 : (globalPitch - count) * uint32_t(sizeof(T)),
                groups == 1 ? 0 : (localPitch * uint32_t(sizeof(T)) - ((bytes + 31) / 32) * 32) / 32, 0},
            DataCopyPadExtParams<T>{false, 0, 0, 0});
    }
}

template <typename T, bool Aligned>
__aicore__ inline void CopyCandidateOutput(GlobalTensor<T> global, LocalTensor<T> local,
    uint32_t index, uint16_t groups, uint32_t count, uint32_t localPitch, uint32_t globalPitch)
{
    const uint32_t bytes = count * uint32_t(sizeof(T));
    if constexpr (Aligned) {
        DataCopy(global[index], local, DataCopyParams{groups, uint16_t(bytes / 32),
            uint16_t((localPitch - count) * sizeof(T) / 32),
            uint16_t((globalPitch - count) * sizeof(T) / 32)});
    } else {
        DataCopyPad(global[index], local,
            DataCopyExtParams{groups, bytes,
                (localPitch * uint32_t(sizeof(T)) - ((bytes + 31) / 32) * 32) / 32,
                (globalPitch - count) * uint32_t(sizeof(T)), 0});
    }
}
