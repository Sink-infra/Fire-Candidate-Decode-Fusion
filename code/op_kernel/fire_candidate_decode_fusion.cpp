#include "kernel_operator.h"
#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

using namespace AscendC;

#include "candidate_decode_small.h"
#include "candidate_decode_medium.h"
#include "candidate_decode_range.h"
#include "candidate_decode_paired.h"

template <typename DT_RAW_PRED, uint32_t PATH>
__global__ __aicore__ void fire_candidate_decode_fusion(GM_ADDR raw_pred,
    GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling)
{
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, data, tiling);
    if constexpr (PATH == 19 || PATH == 20) {
        PairedBatchCandidateDecode<DT_RAW_PRED, PATH == 19 ? 512 : 576,
            PATH == 20, true, true, 4> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 18) {
        PairedBatchCandidateDecode<DT_RAW_PRED, 1408, false, true,
            sizeof(DT_RAW_PRED) == 4> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 15) {
        SingleTaskSmallCandidateDecode<DT_RAW_PRED, 4, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 14) {
        SmallCandidateDecode<DT_RAW_PRED, 4, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 12 || PATH == 13) {
        PairedBatchCandidateDecode<DT_RAW_PRED, PATH == 12 ? 256 : 320, PATH == 13, true, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 10) {
        SmallCandidateDecode<DT_RAW_PRED, 4, true> op;
        op.Init(raw_pred, priors, decoded, data);
        op.Process();
    } else if constexpr (PATH == 8 || PATH == 9) {
        PairedBatchCandidateDecode<DT_RAW_PRED, PATH == 8 ? 1408 : 1664,
            false, PATH == 9, sizeof(DT_RAW_PRED) == 4> op;
        op.Init(raw_pred, priors, decoded, data);
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
