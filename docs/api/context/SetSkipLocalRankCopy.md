# SetSkipLocalRankCopy<a name="ZH-CN_TOPIC_0000002156218309"></a>

## 功能说明<a name="section618mcpsimp"></a>

设置本卡的通信算法的计算结果是否输出到目的数据buffer地址。

## 函数原型<a name="section620mcpsimp"></a>

```
uint32_t SetSkipLocalRankCopy(uint8_t skipLocalRankCopy)
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
<tbody><tr id="row106481443135617"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p167361341213"><a name="p167361341213"></a><a name="p167361341213"></a>skipLocalRankCopy</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p137362417119"><a name="p137362417119"></a><a name="p137362417119"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p3298848112513"><a name="p3298848112513"></a><a name="p3298848112513"></a>本卡的通信算法的计算结果是否输出到recvBuf（目的数据buffer地址）。</p>
<p id="p32561243597"><a name="p32561243597"></a><a name="p32561243597"></a>针对<span id="ph1292674871116"><a name="ph1292674871116"></a><a name="ph1292674871116"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span>，仅AllGather算法与AlltoAll算法支持配置该参数。uint8_t类型，参数取值如下：</p>
<a name="ul398318266399"></a><a name="ul398318266399"></a><ul id="ul398318266399"><li>0：输出本卡通信算法的计算结果（未调用本接口时的默认行为）。</li><li>1：不输出本卡通信算法的计算结果。在无需输出通信结果时，配置参数值为1，此时不会拷贝本卡的通信结果数据，可提升算子性能。例如，在8卡场景下，本卡只取其他卡的部分数据，这时可配置该参数为1。</li></ul>
<p id="p17413844192612"><a name="p17413844192612"></a><a name="p17413844192612"></a>针对<span id="ph690123416432"><a name="ph690123416432"></a><a name="ph690123416432"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span>，该参数为预留字段，配置后不生效。</p>
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

