// Kernel侧核函数实现
#include "kernel_operator.h"

#include "fire_candidate_decode_fusion_tiling.h"
#include "tiling_key_fire_candidate_decode_fusion.h"

template <class DT_RAW_PRED>
class KernelFireCandidateDecodeFusion {
public:
    __aicore__ inline KernelFireCandidateDecodeFusion() {}
    __aicore__ inline void Init(GM_ADDR raw_pred, GM_ADDR priors, GM_ADDR decoded, uint32_t length) {

    }
    __aicore__ inline void Process() {

    }
private:

};

template <typename DT_RAW_PRED>
 __global__ __aicore__ void fire_candidate_decode_fusion(GM_ADDR raw_pred, GM_ADDR priors, GM_ADDR decoded, GM_ADDR workspace, GM_ADDR tiling) {
    REGISTER_TILING_DEFAULT(FireCandidateDecodeFusionTilingData);
    GET_TILING_DATA_WITH_STRUCT(FireCandidateDecodeFusionTilingData, tiling_data, tiling);
    KernelFireCandidateDecodeFusion<DT_RAW_PRED> op;
    op.Init(raw_pred, priors, decoded, tiling_data.length);
    op.Process();
}
