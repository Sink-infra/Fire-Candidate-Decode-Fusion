#include "register/op_def_registry.h"
#include "register/op_impl_registry.h"
#include "tiling/platform/platform_ascendc.h"
#include "../op_kernel/fire_candidate_decode_fusion_tiling.h"
#include "../op_kernel/tiling_key_fire_candidate_decode_fusion.h"

namespace optiling {
static ge::graphStatus TilingFunc(gert::TilingContext *context) {
    const auto *raw = context->GetInputShape(0);
    const auto *prior = context->GetInputShape(1);
    const auto *rawDesc = context->GetInputDesc(0);
    const auto *priorDesc = context->GetInputDesc(1);
    if (!raw || !prior || !rawDesc || !priorDesc) return ge::GRAPH_FAILED;
    const auto &r = raw->GetStorageShape();
    const auto &p = prior->GetStorageShape();
    if (r.GetDimNum() != 3 || p.GetDimNum() != 2 ||
        r.GetDim(0) < 1 || r.GetDim(1) < 1 ||
        r.GetDim(2) != 6 || p.GetDim(0) != r.GetDim(1) || p.GetDim(1) != 5 ||
        priorDesc->GetDataType() != ge::DT_FLOAT ||
        (rawDesc->GetDataType() != ge::DT_FLOAT && rawDesc->GetDataType() != ge::DT_FLOAT16)) {
        return ge::GRAPH_FAILED;
    }
    const uint64_t maxCandidates = UINT32_MAX / 6;
    if (static_cast<uint64_t>(r.GetDim(0)) >
        maxCandidates / static_cast<uint64_t>(r.GetDim(1))) return ge::GRAPH_FAILED;
    auto platform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
    const uint32_t n = r.GetDim(1);
    const uint32_t total = r.GetDim(0) * n;
    uint32_t cores = platform.GetCoreNumAiv();
    if (cores == 0) cores = 1;
    const uint32_t groups = (total + 63) / 64;
    if (cores > groups) cores = groups;
    if (n >= 256 && total <= cores * 256) {
        const uint32_t tileGroups = (total + 255) / 256;
        if (cores > tileGroups && tileGroups * 2 >= cores) cores = tileGroups;
    }
    auto *data = context->GetTilingData<FireCandidateDecodeFusionTilingData>();
    data->candidates = n;
    data->total = total;
    data->perCore = ((total + cores * 64 - 1) / (cores * 64)) * 64;
    context->SetBlockDim(cores);
    context->GetWorkspaceSizes(1)[0] = 0;
    const uint32_t DT_RAW_PRED = static_cast<uint32_t>(rawDesc->GetDataType());
    const uint32_t EXEC_PATH = data->perCore > 256 && n > 256 ? 1 : 0;
    ASCENDC_TPL_SEL_PARAM(context, DT_RAW_PRED, EXEC_PATH);
    return ge::GRAPH_SUCCESS;
}
IMPL_OP_OPTILING(FireCandidateDecodeFusion).Tiling(TilingFunc);
}

namespace ge {
static graphStatus InferShape(gert::InferShapeContext *context) {
    const auto *raw = context->GetInputShape(0);
    auto *out = context->GetOutputShape(0);
    if (!raw || !out || raw->GetDimNum() != 3) return GRAPH_FAILED;
    *out = *raw;
    out->SetDim(2, 5);
    return GRAPH_SUCCESS;
}
static graphStatus InferDataType(gert::InferDataTypeContext *context) {
    context->SetOutputDataType(0, context->GetInputDataType(0));
    return GRAPH_SUCCESS;
}
}

namespace ops {
class FireCandidateDecodeFusion : public OpDef {
public:
    explicit FireCandidateDecodeFusion(const char *name) : OpDef(name) {
        this->Input("raw_pred").ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT, ge::DT_FLOAT16}).Format({ge::FORMAT_ND, ge::FORMAT_ND});
        this->Input("priors").ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT, ge::DT_FLOAT}).Format({ge::FORMAT_ND, ge::FORMAT_ND});
        this->Output("decoded").ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT, ge::DT_FLOAT16}).Format({ge::FORMAT_ND, ge::FORMAT_ND});
        this->SetInferShape(ge::InferShape).SetInferDataType(ge::InferDataType);
        this->AICore().SetTiling(optiling::TilingFunc).AddConfig("ascend910b");
    }
};
OP_ADD(FireCandidateDecodeFusion);
}
