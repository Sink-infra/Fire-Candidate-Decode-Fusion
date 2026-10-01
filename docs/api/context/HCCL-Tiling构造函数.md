# HCCL Tiling构造函数<a name="ZH-CN_TOPIC_0000002120978190"></a>

## 功能说明<a name="section618mcpsimp"></a>

用于创建一个Mc2CcTilingConfig对象。

## 函数原型<a name="section620mcpsimp"></a>

```
Mc2CcTilingConfig(const std::string &groupName, uint32_t opType, const std::string &algConfig, uint32_t reduceType = 0, uint8_t dstDataType = 0, uint8_t srcDataType = 0, uint8_t commEngine = 0)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

<a name="table9646134355611"></a>
<table><thead align="left"><tr id="row964714433565"><th class="cellrowborder" valign="top" width="14.97%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="12.04%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.99%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row106481443135617"><td class="cellrowborder" valign="top" width="14.97%" headers="mcps1.2.4.1.1 "><p id="p167361341213"><a name="p167361341213"></a><a name="p167361341213"></a>groupName</p>
</td>
<td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="p137362417119"><a name="p137362417119"></a><a name="p137362417119"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p165151033101217"><a name="p165151033101217"></a><a name="p165151033101217"></a>当前通信任务所在的通信域。string类型，支持的最大长度为128字节。</p>
</td>
</tr>
<tr id="row149781515017"><td class="cellrowborder" valign="top" width="14.97%" headers="mcps1.2.4.1.1 "><p id="p8779155119"><a name="p8779155119"></a><a name="p8779155119"></a>opType</p>
</td>
<td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="p18771915917"><a name="p18771915917"></a><a name="p18771915917"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p1882120261843"><a name="p1882120261843"></a><a name="p1882120261843"></a>表示通信任务类型。uint32_t类型。HCCL API提供<a href="#table2469980529">HcclCMDType</a>枚举定义作为该参数的取值，具体支持的通信任务类型及取值请参考<a href="#table2469980529">表2</a>。</p>
</td>
</tr>
<tr id="row783154720016"><td class="cellrowborder" valign="top" width="14.97%" headers="mcps1.2.4.1.1 "><p id="p12301127515"><a name="p12301127515"></a><a name="p12301127515"></a>algConfig</p>
</td>
<td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="p1123032712112"><a name="p1123032712112"></a><a name="p1123032712112"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p179841930161220"><a name="p179841930161220"></a><a name="p179841930161220"></a>通信算法配置。string类型，支持的最大长度为128字节。</p>
<p id="p14783165817101"><a name="p14783165817101"></a><a name="p14783165817101"></a>针对<span id="ph178051946141116"><a name="ph178051946141116"></a><a name="ph178051946141116"></a>Ascend 950PR/Ascend 950DT</span>，该参数为预留字段，配置后不生效，默认仅支持FullMesh算法。FullMesh算法即NPU之间的全连接，任意两个NPU之间可以直接进行数据收发。</p>
<p id="p1767616336245"><a name="p1767616336245"></a><a name="p1767616336245"></a>针对<span id="ph2886102472812"><a name="ph2886102472812"></a><a name="ph2886102472812"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span>，当前支持的取值为：</p>
<a name="ul13413418111314"></a><a name="ul13413418111314"></a><ul id="ul13413418111314"><li>"AllReduce=level0:doublering"：AllReduce通信任务。</li><li>"AllGather=level0:doublering"：AllGather通信任务。</li><li>"ReduceScatter=level0:doublering"：ReduceScatter通信任务。</li><li>"AlltoAll=level0:fullmesh;level1:pairwise"：AlltoAllV和AlltoAll通信任务。</li><li>"BatchWrite=level0:fullmesh"：BatchWrite通信任务。</li></ul>
<p id="p2090494125016"><a name="p2090494125016"></a><a name="p2090494125016"></a>针对<span id="ph399761782716"><a name="ph399761782716"></a><a name="ph399761782716"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span>，该参数为预留字段，配置后不生效，默认仅支持FullMesh算法。FullMesh算法即NPU之间的全连接，任意两个NPU之间可以直接进行数据收发。</p>
</td>
</tr>
<tr id="row09448437020"><td class="cellrowborder" valign="top" width="14.97%" headers="mcps1.2.4.1.1 "><p id="p49211736211"><a name="p49211736211"></a><a name="p49211736211"></a>reduceType</p>
</td>
<td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="p1592143615116"><a name="p1592143615116"></a><a name="p1592143615116"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p69241958121317"><a name="p69241958121317"></a><a name="p69241958121317"></a>归约操作类型，仅对有归约操作的通信任务生效。uint32_t类型，取值详见<a href="HCCL使用说明.md#table2469980529">表2</a>。</p>
</td>
</tr>
<tr id="row11811506587"><td class="cellrowborder" valign="top" width="14.97%" headers="mcps1.2.4.1.1 "><p id="p218115011581"><a name="p218115011581"></a><a name="p218115011581"></a>dstDataType</p>
</td>
<td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="p1618119502580"><a name="p1618119502580"></a><a name="p1618119502580"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p362492520517"><a name="p362492520517"></a><a name="p362492520517"></a>通信任务中输出数据的数据类型。uint8_t类型，该参数的取值范围请参考<a href="HCCL使用说明.md#table116710585514">表1</a>。</p>
<p id="p645895003920"><a name="p645895003920"></a><a name="p645895003920"></a><span id="ph15985184838"><a name="ph15985184838"></a><a name="ph15985184838"></a>Ascend 950PR/Ascend 950DT</span>，不同通信任务支持的输出数据类型不同，具体为：</p>
<a name="ul182971329105817"></a><a name="ul182971329105817"></a><ul id="ul182971329105817"><li>对于AllReduce、AllGather、AllToAll、AllToAllV、AllToAllVWrite通信任务：输出的数据类型必须与输入的数据类型一致。各通信任务支持的输入数据类型请参考<a href="#p562895445811">srcDataType</a>。</li><li>对于ReduceScatter通信任务，当输入的数据类型为int16_t、int32_t、half、float、bfloat16_t时，输出的数据类型必须与其一致；当输入的数据类型为int8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t时，输出的数据类型必须为half、bfloat16_t、float三者之一。</li></ul>
<p id="p09613381216"><a name="p09613381216"></a><a name="p09613381216"></a><span id="ph496033141210"><a name="ph496033141210"></a><a name="ph496033141210"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，该参数暂不支持，配置后不生效。</p>
<p id="p0713917529"><a name="p0713917529"></a><a name="p0713917529"></a><span id="ph579472813512"><a name="ph579472813512"></a><a name="ph579472813512"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，该参数暂不支持，配置后不生效。</p>
</td>
</tr>
<tr id="row6628195413581"><td class="cellrowborder" valign="top" width="14.97%" headers="mcps1.2.4.1.1 "><p id="p562895445811"><a name="p562895445811"></a><a name="p562895445811"></a>srcDataType</p>
</td>
<td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="p176282054115812"><a name="p176282054115812"></a><a name="p176282054115812"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p1461191113"><a name="p1461191113"></a><a name="p1461191113"></a>通信任务中输入数据的数据类型。uint8_t类型，该参数的取值范围请参考<a href="HCCL使用说明.md#table116710585514">表1</a>。</p>
<p id="p14631313113"><a name="p14631313113"></a><a name="p14631313113"></a><span id="ph20611191313"><a name="ph20611191313"></a><a name="ph20611191313"></a>Ascend 950PR/Ascend 950DT</span>，不同通信任务支持的输入数据类型如下：</p>
<a name="ul57135435587"></a><a name="ul57135435587"></a><ul id="ul57135435587"><li>AllReduce通信任务：支持的输入类型为int16_t、half、bfloat16_t、int32_t、float。</li><li>AllGather、AllToAll、AllToAllV、AllToAllVWrite通信任务：支持的输入类型为int8_t、uint8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、double。</li><li>ReduceScatter通信任务：支持的输入类型为int8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、half、bfloat16_t、int32_t、float。</li></ul>
<p id="p3427132311211"><a name="p3427132311211"></a><a name="p3427132311211"></a><span id="ph174278236122"><a name="ph174278236122"></a><a name="ph174278236122"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>，该参数暂不支持，配置后不生效。</p>
<p id="p1881211581075"><a name="p1881211581075"></a><a name="p1881211581075"></a>针对<span id="ph188124581476"><a name="ph188124581476"></a><a name="ph188124581476"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>，该参数暂不支持，配置后不生效。</p>
</td>
</tr>
<tr id="row1025410593510"><td class="cellrowborder" valign="top" width="14.97%" headers="mcps1.2.4.1.1 "><p id="p22550599518"><a name="p22550599518"></a><a name="p22550599518"></a>commEngine</p>
</td>
<td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="p225525917513"><a name="p225525917513"></a><a name="p225525917513"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p225575910518"><a name="p225575910518"></a><a name="p225575910518"></a>通信引擎。uint8_t类型，该参数的取值范围请参考：《HCCL集合通信库接口》&gt;HCCL API (C)&gt;数据类型&gt;HcclCommConfig中的<span>hcclOpExpansionMode参数取值说明</span>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  HcclCMDType参数说明

<a name="table2469980529"></a>
<table><thead align="left"><tr id="row194691183522"><th class="cellrowborder" valign="top" width="17.119999999999997%" id="mcps1.2.3.1.1"><p id="p34692815210"><a name="p34692815210"></a><a name="p34692815210"></a>数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="82.88%" id="mcps1.2.3.1.2"><p id="p194691389528"><a name="p194691389528"></a><a name="p194691389528"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row4469388522"><td class="cellrowborder" valign="top" width="17.119999999999997%" headers="mcps1.2.3.1.1 "><p id="p2046988195218"><a name="p2046988195218"></a><a name="p2046988195218"></a>HcclCMDType</p>
</td>
<td class="cellrowborder" valign="top" width="82.88%" headers="mcps1.2.3.1.2 "><p id="p1046928165216"><a name="p1046928165216"></a><a name="p1046928165216"></a>通信任务类型。</p>
<p id="p157531691030"><a name="p157531691030"></a><a name="p157531691030"></a>针对<span id="ph1595918753613"><a name="ph1595918753613"></a><a name="ph1595918753613"></a>Ascend 950PR/Ascend 950DT</span>，当前支持的通信任务类型为HCCL_CMD_ALLREDUCE、HCCL_CMD_ALLGATHER、HCCL_CMD_REDUCE_SCATTER、HCCL_CMD_ALLTOALLV、HCCL_CMD_HALF_ALLTOALLV。</p>
<p id="p3281105825815"><a name="p3281105825815"></a><a name="p3281105825815"></a>针对<span id="ph428114589586"><a name="ph428114589586"></a><a name="ph428114589586"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_3"><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_3"><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a>Atlas A3 推理系列产品</term></span>，当前支持的通信任务类型为HCCL_CMD_ALLREDUCE、HCCL_CMD_ALLGATHER、HCCL_CMD_REDUCE_SCATTER、HCCL_CMD_ALLTOALL、HCCL_CMD_ALLTOALLV、HCCL_CMD_BATCH_WRITE。</p>
<p id="p1281658165813"><a name="p1281658165813"></a><a name="p1281658165813"></a>针对<span id="ph14281558155812"><a name="ph14281558155812"></a><a name="ph14281558155812"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_3"><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_3"><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a>Atlas A2 推理系列产品</term></span>，当前支持的通信任务类型为HCCL_CMD_ALLREDUCE、HCCL_CMD_ALLGATHER、HCCL_CMD_REDUCE_SCATTER、HCCL_CMD_ALLTOALL、HCCL_CMD_BATCH_WRITE。</p>
<a name="screen11611163816318"></a><a name="screen11611163816318"></a><pre class="screen" codetype="Cpp" id="screen11611163816318">enum class HcclCMDType { 
    HCCL_CMD_INVALID = 0,
    HCCL_CMD_BROADCAST = 1,
    HCCL_CMD_ALLREDUCE,
    HCCL_CMD_REDUCE,
    HCCL_CMD_SEND,
    HCCL_CMD_RECEIVE,
    HCCL_CMD_ALLGATHER,
    HCCL_CMD_REDUCE_SCATTER,
    HCCL_CMD_ALLTOALLV,
    HCCL_CMD_ALLTOALLVC,
    HCCL_CMD_ALLTOALL,
    HCCL_CMD_GATHER,
    HCCL_CMD_SCATTER,
    HCCL_CMD_BATCH_SEND_RECV,
    HCCL_CMD_BATCH_PUT,
    HCCL_CMD_BATCH_GET,
    HCCL_CMD_ALLGATHER_V,
    HCCL_CMD_REDUCE_SCATTER_V,
    HCCL_CMD_BATCH_WRITE,
    HCCL_CMD_HALF_ALLTOALLV = 20,
    HCCL_CMD_ALL
};</pre>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section1665082013318"></a>

```
const char *groupName = "testGroup";
uint32_t opType = HCCL_CMD_REDUCE_SCATTER;
std::string algConfig = "ReduceScatter=level0:fullmesh";
uint32_t reduceType = HCCL_REDUCE_SUM;
uint8_t dstDataType = HCCL_DATA_TYPE_FP16;
uint8_t srcDataType = HCCL_DATA_TYPE_FP16;
uint8_t commEngine = 0;
AscendC::Mc2CcTilingConfig mc2CcTilingConfig(groupName, opType, algConfig, reduceType, dstDataType, srcDataType, commEngine); // 构造函数
mc2CcTilingConfig.GetTiling(tiling->mc2InitTiling);  // tiling为算子组装的TilingData结构体
mc2CcTilingConfig.GetTiling(tiling->reduceScatterTiling);
```

