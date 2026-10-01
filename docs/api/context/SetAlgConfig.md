# SetAlgConfig<a name="ZH-CN_TOPIC_0000002120978194"></a>

## 功能说明<a name="section618mcpsimp"></a>

设置通信算法。

## 函数原型<a name="section620mcpsimp"></a>

```
uint32_t SetAlgConfig(const std::string &algConfig)
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
<tbody><tr id="row106481443135617"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p167361341213"><a name="p167361341213"></a><a name="p167361341213"></a>algConfig</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p137362417119"><a name="p137362417119"></a><a name="p137362417119"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p915191791319"><a name="p915191791319"></a><a name="p915191791319"></a>通信算法配置。string类型，支持的最大长度为128字节。</p>
<p id="p13299449161211"><a name="p13299449161211"></a><a name="p13299449161211"></a>针对<span id="ph192681844162618"><a name="ph192681844162618"></a><a name="ph192681844162618"></a>Ascend 950PR/Ascend 950DT</span>，该参数为预留字段，配置后不生效，默认仅支持FullMesh算法。FullMesh算法即NPU之间的全连接，任意两个NPU之间可以直接进行数据收发。</p>
<p id="p641311811133"><a name="p641311811133"></a><a name="p641311811133"></a>针对<span id="ph690123416432"><a name="ph690123416432"></a><a name="ph690123416432"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span>，当前支持的取值为：</p>
<a name="ul13413418111314"></a><a name="ul13413418111314"></a><ul id="ul13413418111314"><li>"AllReduce=level0:doublering"：AllReduce通信任务。</li><li>"AllGather=level0:doublering"：AllGather通信任务。</li><li>"ReduceScatter=level0:doublering"：ReduceScatter通信任务。</li><li>"AlltoAll=level0:fullmesh;level1:pairwise"：AlltoAllV和AlltoAll通信任务。</li><li>"BatchWrite=level0:fullmesh"：BatchWrite通信任务。</li></ul>
<p id="p2090494125016"><a name="p2090494125016"></a><a name="p2090494125016"></a>针对<span id="ph1292674871116"><a name="ph1292674871116"></a><a name="ph1292674871116"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span>，该参数为预留字段，配置后不生效，默认仅支持FullMesh算法。FullMesh算法即NPU之间的全连接，任意两个NPU之间可以直接进行数据收发。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

-   0表示设置成功。
-   非0表示设置失败。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section1665082013318"></a>

本接口的调用示例请见[调用示例](SetOpType.md#section1665082013318)。

