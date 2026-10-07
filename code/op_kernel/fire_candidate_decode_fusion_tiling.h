#pragma once
#include <cstdint>
struct FireCandidateDecodeFusionTilingData {
    uint32_t batch;
    uint32_t candidates;
    uint32_t cores;
    uint32_t segments;
};
struct FireCandidateDecodeFusionIndexedTilingData {
    FireCandidateDecodeFusionTilingData base;
    uint32_t padding[4];
    uint32_t rawIndex[576];
    uint32_t priorIndex[576];
    uint32_t packIndex[640];
};
