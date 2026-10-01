# SetMadType<a name="ZH-CN_TOPIC_0000001571075490"></a>

## 功能说明<a name="section618mcpsimp"></a>

设置是否使能HF32模式。**当前版本暂不支持。**

设置是否使能HF32模式（**当前版本暂不支持**）、设置是否使能MxMatmul场景。在MxMatmul场景，必须调用该接口并配置为使能MxMatmul场景，从而保证该场景下正确计算并返回Tiling参数**。**

## 函数原型<a name="section620mcpsimp"></a>

```
int32_t SetMadType(MatrixMadType madType)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

<a name="table9646134355611"></a>
<table><thead align="left"><tr id="row964714433565"><th class="cellrowborder" valign="top" width="14.99%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="12.02%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.99%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row106481443135617"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p63391231164514"><a name="p63391231164514"></a><a name="p63391231164514"></a>madType</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p14339133120459"><a name="p14339133120459"></a><a name="p14339133120459"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p31537014914"><a name="p31537014914"></a><a name="p31537014914"></a>设置Matmul模式。MatrixMadType类型，定义如下。</p>
<a name="screen511415305152"></a><a name="screen511415305152"></a><pre class="screen" codetype="Cpp" id="screen511415305152">enum class MatrixMadType : int32_t {
NORMAL = 0,
HF32 = 1, 
MXMODE = 2,
}; </pre>
<a name="ul372412456"></a><a name="ul372412456"></a><ul id="ul372412456"><li>MatrixMadType::NORMAL：普通模式，即非HF32模式、非MxMatmul场景。</li><li>MatrixMadType::HF32：使能HF32模式。</li><li>MatrixMadType::MXMODE：使能MxMatmul场景。</li></ul>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

-1表示设置失败； 0表示设置成功。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section1665082013318"></a>

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MatmulApiTiling tiling(ascendcPlatform); 
tiling.SetAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16); 
tiling.SetBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);  
tiling.SetCType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
tiling.SetBiasType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);   
tiling.SetShape(1024, 1024, 1024);
tiling.SetOrgShape(1024, 1024, 1024);   
tiling.SetBias(true);
tiling.SetTraverse(MatrixTraverse::FIRSTM);  // 设置遍历方式
tiling.SetMadType(MatrixMadType::MXMODE);  // 使能MxMatmul场景
tiling.SetBufferSpace(-1, -1, -1);
optiling::TCubeTiling tilingData;   
int ret = tiling.GetTiling(tilingData);
```

