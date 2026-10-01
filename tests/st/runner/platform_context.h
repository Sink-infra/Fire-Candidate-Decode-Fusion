#pragma once
#include <cstdint>

extern "C" std::uint32_t ConfigurePlatformLabel(
    std::uint32_t device, const char *physicalSoc, const char *canonicalLabel);

extern "C" std::uint32_t ReadRuntimeDeviceResources(
    std::uint32_t device, std::int64_t *values);
