# v2版本TilingData（废弃）<a name="ZH-CN_TOPIC_0000002024389216"></a>

> [!NOTE]说明 
>该结构体废弃，并将在后续版本移除，请不要使用该结构体。无需直接对该结构体中的成员进行设置，统一使用HCCL Tiling提供的接口设置即可。

## 功能说明<a name="section77005157231"></a>

AI CPU启动下发通信任务前，需获取固定的通信配置，如[表1](#table4835205712588)所示。在算子实现中，由Tiling组装通信配置项，通过配置固定参数和固定参数顺序的Tiling Data，将通信配置信息在调用AI CPU通信接口时传递给AI CPU。

## 参数说明<a name="section146744150261"></a>

**表 1**  v2版本HCCL TilingData参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="row118356578583"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="p48354572582"><a name="p48354572582"></a><a name="p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p583535795817"><a name="p583535795817"></a><a name="p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1835857145817"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p122027792718"><a name="p122027792718"></a><a name="p122027792718"></a>version</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p386253782517"><a name="p386253782517"></a><a name="p386253782517"></a>uint32_t类型。用于区分TilingData版本。</p>
<p id="p936825654215"><a name="p936825654215"></a><a name="p936825654215"></a>v2版本的TilingData结构体中，version字段仅支持取值为2。</p>
<p id="p552431593911"><a name="p552431593911"></a><a name="p552431593911"></a>注意：该字段在v2版本TilingData中的位置，同v1版本的preparePosition字段。当该字段取值为2时，为v2版本的结构体，当取值为1时，为v1版本的结构体，请使用<a href="v1版本TilingData（废弃）.md#table4835205712588">Mc2Msg结构体</a>。</p>
</td>
</tr>
<tr id="row18835145716587"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1383515717581"><a name="p1383515717581"></a><a name="p1383515717581"></a>mc2HcommCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p189653101104"><a name="p189653101104"></a><a name="p189653101104"></a>uint32_t类型。表示各通信域中通信任务总个数。当前该参数支持的最大取值为3。</p>
</td>
</tr>
<tr id="row14855142316314"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p64008366310"><a name="p64008366310"></a><a name="p64008366310"></a>serverCfg</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p16427144919511"><a name="p16427144919511"></a><a name="p16427144919511"></a><a href="#table96371578575">Mc2ServerCfg</a>类型。集合通信server端通用参数配置。</p>
</td>
</tr>
<tr id="row10918154010310"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p720815561636"><a name="p720815561636"></a><a name="p720815561636"></a>hcom</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1521103819511"><a name="p1521103819511"></a><a name="p1521103819511"></a><a href="#table13131010011">Mc2HcommCfg</a>类型。各通信域中每个通信任务的参数配置。在通信算子TilingData的定义中，根据各通信域中通信任务总个数，共需要定义mc2HcommCnt个Mc2HcommCfg结构体。例如：mc2HcommCnt=2，则需要依次定义2个<a href="#table13131010011">Mc2HcommCfg</a>类型的参数，自定义参数名，比如hcom1、hcom2。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  Mc2ServerCfg结构体说明

<a name="table96371578575"></a>
<table><thead align="left"><tr id="row86371257155719"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="p06371957175715"><a name="p06371957175715"></a><a name="p06371957175715"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p46371157205717"><a name="p46371157205717"></a><a name="p46371157205717"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row263719576571"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p5637115718579"><a name="p5637115718579"></a><a name="p5637115718579"></a>version</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p18637857105715"><a name="p18637857105715"></a><a name="p18637857105715"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row176371957125710"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1179261313212"><a name="p1179261313212"></a><a name="p1179261313212"></a>debugMode</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p86372571576"><a name="p86372571576"></a><a name="p86372571576"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row13637145711570"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p4637205719579"><a name="p4637205719579"></a><a name="p4637205719579"></a>sendArgIndex</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p36372057125716"><a name="p36372057125716"></a><a name="p36372057125716"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row16637165715570"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p693995915217"><a name="p693995915217"></a><a name="p693995915217"></a>recvArgIndex</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p393920591925"><a name="p393920591925"></a><a name="p393920591925"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row1936913415313"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p19169104017311"><a name="p19169104017311"></a><a name="p19169104017311"></a>commOutArgIndex</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p136037517319"><a name="p136037517319"></a><a name="p136037517319"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row17291181638"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p16502791338"><a name="p16502791338"></a><a name="p16502791338"></a>reserved</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p17502209839"><a name="p17502209839"></a><a name="p17502209839"></a>预留字段，不需要配置。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  Mc2HcommCfg结构体说明

<a name="table13131010011"></a>
<table><thead align="left"><tr id="row13381018011"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="p5351013019"><a name="p5351013019"></a><a name="p5351013019"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p19314101012"><a name="p19314101012"></a><a name="p19314101012"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1432101608"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1731514820114"><a name="p1731514820114"></a><a name="p1731514820114"></a>skipLocalRankCopy</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p5315107010"><a name="p5315107010"></a><a name="p5315107010"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row123410809"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p12912948151214"><a name="p12912948151214"></a><a name="p12912948151214"></a>skipBufferWindowCopy</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1998424141212"><a name="p1998424141212"></a><a name="p1998424141212"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row18491012018"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p128071391312"><a name="p128071391312"></a><a name="p128071391312"></a>stepSize</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p26091824111211"><a name="p26091824111211"></a><a name="p26091824111211"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row19451012017"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p15280112519129"><a name="p15280112519129"></a><a name="p15280112519129"></a>reserved</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p828022581218"><a name="p828022581218"></a><a name="p828022581218"></a>预留字段，不需要配置。</p>
</td>
</tr>
<tr id="row1222832811126"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p172281128161215"><a name="p172281128161215"></a><a name="p172281128161215"></a>groupName</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p122815285128"><a name="p122815285128"></a><a name="p122815285128"></a>当前通信任务所在的通信域。char *类型，支持最大长度128。</p>
</td>
</tr>
<tr id="row189842305126"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p7984193061219"><a name="p7984193061219"></a><a name="p7984193061219"></a>algConfig</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p179841930161220"><a name="p179841930161220"></a><a name="p179841930161220"></a>通信算法配置。char *类型，支持最大长度128。</p>
<p id="p1377295018592"><a name="p1377295018592"></a><a name="p1377295018592"></a>当前支持的取值为：</p>
<a name="ul68322362593"></a><a name="ul68322362593"></a><ul id="ul68322362593"><li>"AllGather=level0:doublering"：AllGather通信任务。</li><li>"ReduceScatter=level0:doublering"：ReduceScatter通信任务。</li><li>"AlltoAll=level0:fullmesh;level1:pairwise"：AlltoAllV通信任务。</li></ul>
</td>
</tr>
<tr id="row4515933191215"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1751573311210"><a name="p1751573311210"></a><a name="p1751573311210"></a>opType</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p165151033101217"><a name="p165151033101217"></a><a name="p165151033101217"></a>表示通信任务类型。uint32_t类型，取值详见<a href="HCCL-Tiling构造函数.md#table2469980529">HcclCMDType</a>参数说明。</p>
</td>
</tr>
<tr id="row1092475851319"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p59241258161310"><a name="p59241258161310"></a><a name="p59241258161310"></a>reduceType</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p69241958121317"><a name="p69241958121317"></a><a name="p69241958121317"></a>归约操作类型，仅对有归约操作的通信任务生效。uint32_t类型，取值详见<a href="HCCL使用说明.md#table2469980529">HcclReduceOp</a>参数说明。</p>
</td>
</tr>
</tbody>
</table>

## 约束说明<a name="section633mcpsimp"></a>

-   如果需要使用v2版本的Tiling结构体，必须设置Tiling结构体的第一个参数version=2。
-   算子的Tiling Data结构需要完整包含[v2版本HCCL TilingData参数](#table4835205712588)，其中各参数需要严格按照对应参数的结构来定义。

## 调用示例<a name="section9414023616"></a>

如下为自定义算子AlltoallvDoubleCommCustom的算子原型。该算子有两对输入输出，其中x1、y1是ep通信域的AlltoAllV任务的输入输出，x2、y2是tp通信域的AlltoAllV任务的输入输出。

```
namespace ops {
class AlltoallvDoubleCommCustom : public OpDef {
public:
    explicit AlltoallvDoubleCommCustom(const char *name) : OpDef(name)
    {
        this->Input("x1")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16, ge::DT_BF16})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND})
            .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND});
        this->Input("x2")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16, ge::DT_BF16})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND})
            .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND})
            .IgnoreContiguous();
        this->Output("y1")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16, ge::DT_BF16})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND})
            .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND});
        this->Output("y2")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16, ge::DT_BF16})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND})
            .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND});
        this->Attr("group_ep").AttrType(REQUIRED).String();
        this->Attr("group_tp").AttrType(REQUIRED).String();
        this->Attr("ep_world_size").AttrType(REQUIRED).Int();
        this->Attr("tp_world_size").AttrType(REQUIRED).Int();
        this->AICore().SetTiling(optiling::AlltoAllVDoubleCommCustomTilingFunc);
        this->AICore().AddConfig("ascendxxx"); // ascendxxx请修改为对应的AI处理器型号。
        this->MC2().HcclGroup({"group_ep", "group_tp"});
    }
};
OP_ADD(AlltoallvDoubleCommCustom);
}
```

如下为该自定义算子Tiling Data声明和实现。

该自定义算子Tiling Data的声明中：首先定义version字段，设置为2，表明为v2版本的通信算子Tiling结构体。其次，定义mc2HcommCnt字段，本例AlltoallvDoubleCommCustom算子的kernel实现中，共2个AlltoAllV通信任务，该参数取值为2。然后定义server通用参数配置，Mc2ServerCfg。最后，定义2个Mc2HcommCfg结构体，表示各通信域中的每个通信任务参数配置。

```
// HCCL TilingData声明
BEGIN_TILING_DATA_DEF(AlltoallvDoubleCommCustomTilingData)
    TILING_DATA_FIELD_DEF(uint32_t, version);                           // HCCL tiling结构体的版本，设为2
    TILING_DATA_FIELD_DEF(uint32_t, mc2HcommCnt);                       // 各通信域中的通信算子总个数，当前最多支持3个。AlltoallvDoubleCommCustom算子kernel实现中每个通信域中均用了1个AlltoAllV，因此设为2
    TILING_DATA_FIELD_DEF_STRUCT(Mc2ServerCfg, serverCfg);    // server通用参数配置，融合算子级
    TILING_DATA_FIELD_DEF_STRUCT(Mc2HcommCfg, hcom1);         // 各通信域中的每个通信任务参数配置，算子级，共有mc2HcommCnt个Mc2HcommCfg
    TILING_DATA_FIELD_DEF_STRUCT(Mc2HcommCfg, hcom2);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(AlltoallvDoubleCommCustom, AlltoallvDoubleCommCustomTilingData);
```

```
// HCCL TilingData配置片段
static ge::graphStatus AlltoAllVDoubleCommCustomTilingFunc(gert::TilingContext *context)
{
    char *group1 = const_cast<char *>(context->GetAttrs()->GetAttrPointer<char>(0));
    char *group2 = const_cast<char *>(context->GetAttrs()->GetAttrPointer<char>(1));

    AlltoallvDoubleCommCustomTilingData tiling;
    tiling.set_version(2);
    tiling.set_mc2HcommCnt(2);
    tiling.serverCfg.set_debugMode(0);

    tiling.hcom1.set_opType(8);
    tiling.hcom1.set_reduceType(4);
    tiling.hcom1.set_groupName(group1);
    tiling.hcom1.set_algConfig("AlltoAll=level0:fullmesh;level1:pairwise");

    tiling.hcom2.set_opType(8);
    tiling.hcom2.set_reduceType(4);
    tiling.hcom2.set_groupName(group2);
    tiling.hcom2.set_algConfig("AlltoAll=level0:fullmesh;level1:pairwise");
    
    tiling.SaveToBuffer(context->GetRawTilingData()->GetData(), context->GetRawTilingData()->GetCapacity());
    context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());
    return ge::GRAPH_SUCCESS;
}

```

