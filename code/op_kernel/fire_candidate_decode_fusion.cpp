#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

using namespace AscendC;
#include "candidate_index_ramp.h"
#include "candidate_geometry.h"
#include "candidate_grouped_tail.h"

#include "small_candidate_decode.h"
#include "medium_candidate_decode.h"
#include "generic_candidate_decode.h"

#include "paired_batch_candidate_decode.h"
#include "candidate_fixed_small_batch.h"
#include "grouped_medium_candidate_entry.h"
#include "grouped_large_candidate_entry.h"
#include "coarse_medium_candidate_entry.h"
#include "single_group_medium_candidate_decode.h"

template <typename DT_RAW_PRED, uint32_t PATH>
__global__ __aicore__ void fire_candidate_decode_fusion(GM_ADDR raw_pred,
    GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling)
{
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    if constexpr (PATH == 34) {
        InitSocState();
        const uint32_t candidates = uint32_t(GetBlockNum()) * 256;
        const uint32_t begin = uint32_t(GetBlockIdx()) * 256;
        SingleGroupMediumCandidateDecode<DT_RAW_PRED, 1024> op;
        op.Run(raw_pred, priors, decoded, candidates, begin, 256);
    } else {
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, data, tiling);
    if constexpr (PATH == 33) {
        RunCoarseMediumCandidate<DT_RAW_PRED, sizeof(DT_RAW_PRED) == 2>(raw_pred, priors, decoded, data);
    } else if constexpr (PATH == 31 || PATH == 32) {
        RunFourBatchMediumCandidate<DT_RAW_PRED, PATH == 32>(raw_pred, priors, decoded, data);
    } else if constexpr (PATH == 26) {
        InitSocState();
        ProcessFixedSmallBatch<DT_RAW_PRED>(raw_pred, priors, decoded, data);
    } else if constexpr (PATH == 24) {
        SmallCandidateDecode<DT_RAW_PRED, 8, false, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 21) {
        const uint32_t groups = (data.candidates + 15) / 16;
        const uint32_t begin = (groups * GetBlockIdx() / data.cores) * 16;
        uint32_t end = (groups * (GetBlockIdx() + 1) / data.cores) * 16;
        if (end > data.candidates) end = data.candidates;
        InitSocState();
        if (end - begin <= 128) {
            PairedBatchCandidateDecode<DT_RAW_PRED, 256, false, false, true, true, true, true> op;
            op.Init(raw_pred, priors, decoded, data, nullptr);
            op.Process();
        } else {
            PairedBatchCandidateDecode<DT_RAW_PRED, 320, true, false, true, true, true, true> op;
            op.Init(raw_pred, priors, decoded, data, nullptr);
            op.Process();
        }
    } else if constexpr (PATH == 19) {
        RunNativeLargeCandidate<DT_RAW_PRED, 1664>(raw_pred, priors, decoded, data);
    } else if constexpr (PATH == 18) {
        RunNativeLargeCandidate<DT_RAW_PRED, 1408>(raw_pred, priors, decoded, data);
    } else if constexpr (PATH == 15) {
        InitSocState();
        const uint32_t offset = GetBlockIdx() * 4;
        SingleTaskSmallCandidateDecode<DT_RAW_PRED, 4, true, true> op;
        op.Init(raw_pred + offset * 6 * sizeof(DT_RAW_PRED), priors + offset * 20,
            decoded + offset * 5 * sizeof(DT_RAW_PRED), data);
        op.Process();
    } else if constexpr (PATH == 14) {
        SmallCandidateDecode<DT_RAW_PRED, 4, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 12 || PATH == 13) {
        InitSocState();
        PairedBatchCandidateDecode<DT_RAW_PRED, PATH == 12 ? 256 : 320, PATH == 13, false, true, false, true, true> op;
        op.Init(raw_pred, priors, decoded, data, nullptr);
        op.Process();
    } else if constexpr (PATH == 10) {
        SmallCandidateDecode<DT_RAW_PRED, 4> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 8 || PATH == 9) {
        TPipe pipe;
        PairedBatchCandidateDecode<DT_RAW_PRED, PATH == 8 ? 1408 : 1664> op;
        op.Init(raw_pred, priors, decoded, data, &pipe);
        op.Process();
    } else if constexpr (PATH == 0) {
        SmallCandidateDecode<DT_RAW_PRED, 1> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 5) {
        SmallCandidateDecode<DT_RAW_PRED, 8> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 3) {
        SmallCandidateDecode<DT_RAW_PRED, 2> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 6) {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 768, false, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 7) {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 1024, false, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 4) {
        MediumCandidateDecodeFusion<DT_RAW_PRED, 576, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 2) {
        MediumCandidateDecodeFusion<DT_RAW_PRED, 512> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else {
        KernelFireCandidateDecodeFusion<DT_RAW_PRED, 1024> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    }
    }
}
