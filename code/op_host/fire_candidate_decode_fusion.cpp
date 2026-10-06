#include "register/op_def_registry.h"
#include "tiling/platform/platform_ascendc.h"
#include "../op_kernel/fire_candidate_decode_fusion_tiling.h"
#include "../op_kernel/tiling_key_fire_candidate_decode_fusion.h"

namespace optiling {
static ge::graphStatus TilingFunc(gert::TilingContext *context)
{
    const auto *raw = context->GetRequiredInputTensor(0);
    const auto *prior = context->GetRequiredInputTensor(1);
    if (raw == nullptr || prior == nullptr) return ge::GRAPH_FAILED;
    const auto &r = raw->GetStorageShape();
    const auto &p = prior->GetStorageShape();
    const auto dtype = raw->GetDataType();
    if (r.GetDimNum() != 3 || p.GetDimNum() != 2 ||
        r.GetDim(0) < 1 || r.GetDim(0) > 8 || r.GetDim(1) < 1 ||
        r.GetDim(1) > 1048576 || r.GetDim(2) != 6 ||
        p.GetDim(0) != r.GetDim(1) || p.GetDim(1) != 5 ||
        (dtype != ge::DT_FLOAT && dtype != ge::DT_FLOAT16) ||
        prior->GetDataType() != ge::DT_FLOAT) return ge::GRAPH_FAILED;
    auto platform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
    uint32_t cores = platform.GetCoreNumAiv();
    if (cores == 0) return ge::GRAPH_FAILED;
    auto *data = context->GetTilingData<FireCandidateDecodeFusionTilingData>();
    data->batch = r.GetDim(0);
    data->candidates = r.GetDim(1);
    const uint32_t tiles = (data->candidates + 255) / 256;
    if (cores > tiles * data->batch) cores = tiles * data->batch;
    const uint32_t total = data->batch * data->candidates;
    uint32_t PATH = total <= 128 ? (total <= platform.GetCoreNumAiv() ? 0 : 3) : (total <= 65536 ? 2 : 1);
    data->segments = 0;
    if (PATH == 2) {
        cores = platform.GetCoreNumAiv();
        if (cores > 32) cores = 32;
        const uint32_t tasks = data->batch * ((data->candidates + 511) / 512);
        if (cores > tasks) cores = tasks;
        const uint32_t segments = cores / data->batch;
        if (segments > 0 && tasks > cores &&
            (data->candidates + segments - 1) / segments <= 576) {
            PATH = 4;
            data->segments = segments;
            cores = segments * data->batch;
        }
    }
    if (PATH == 3) {
        cores = platform.GetCoreNumAiv();
        const uint32_t tasks = data->batch * ((data->candidates + 1) / 2);
        if (cores > tasks) cores = tasks;
    }
    if (PATH == 0) {
        cores = platform.GetCoreNumAiv();
        if (cores > data->batch * data->candidates) cores = data->batch * data->candidates;
    }
    data->cores = cores;
    context->SetBlockDim(cores);
    uint32_t DT_RAW_PRED = static_cast<uint32_t>(dtype);
    ASCENDC_TPL_SEL_PARAM(context, DT_RAW_PRED, PATH);
    context->GetWorkspaceSizes(1)[0] = 0;
    return ge::GRAPH_SUCCESS;
}
}

namespace ge {
static graphStatus InferShape(gert::InferShapeContext *context)
{
    const auto *raw = context->GetInputShape(0);
    const auto *prior = context->GetInputShape(1);
    auto *out = context->GetOutputShape(0);
    if (!raw || !prior || !out || raw->GetDimNum() != 3 ||
        prior->GetDimNum() != 2 || raw->GetDim(2) != 6 ||
        prior->GetDim(1) != 5 || raw->GetDim(1) != prior->GetDim(0))
        return GRAPH_FAILED;
    *out = *raw;
    out->SetDim(2, 5);
    return GRAPH_SUCCESS;
}
static graphStatus InferDataType(gert::InferDataTypeContext *context)
{
    auto dtype = context->GetInputDataType(0);
    if ((dtype != DT_FLOAT && dtype != DT_FLOAT16) ||
        context->GetInputDataType(1) != DT_FLOAT) return GRAPH_FAILED;
    context->SetOutputDataType(0, dtype);
    return GRAPH_SUCCESS;
}
}

namespace ops {
class FireCandidateDecodeFusion : public OpDef {
public:
    explicit FireCandidateDecodeFusion(const char *name) : OpDef(name)
    {
        this->Input("raw_pred").ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT, ge::DT_FLOAT16})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND});
        this->Input("priors").ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT, ge::DT_FLOAT})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND});
        this->Output("decoded").ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT, ge::DT_FLOAT16})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND});
        this->SetInferShape(ge::InferShape).SetInferDataType(ge::InferDataType);
        this->AICore().SetTiling(optiling::TilingFunc).AddConfig("ascend910b");
    }
};
OP_ADD(FireCandidateDecodeFusion);
}
