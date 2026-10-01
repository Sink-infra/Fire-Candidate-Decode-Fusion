# ReinterpretCast<a name="ZH-CN_TOPIC_0000002330767554"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="zh-cn_topic_0000002330927338_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002330927338_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002330927338_p1883113061818"><a name="zh-cn_topic_0000002330927338_p1883113061818"></a><a name="zh-cn_topic_0000002330927338_p1883113061818"></a><span id="zh-cn_topic_0000002330927338_ph20833205312295"><a name="zh-cn_topic_0000002330927338_ph20833205312295"></a><a name="zh-cn_topic_0000002330927338_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002330927338_p783113012187"><a name="zh-cn_topic_0000002330927338_p783113012187"></a><a name="zh-cn_topic_0000002330927338_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002330927338_row1272474920205"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002330927338_p17301775812"><a name="zh-cn_topic_0000002330927338_p17301775812"></a><a name="zh-cn_topic_0000002330927338_p17301775812"></a><span id="zh-cn_topic_0000002330927338_ph2272194216543"><a name="zh-cn_topic_0000002330927338_ph2272194216543"></a><a name="zh-cn_topic_0000002330927338_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002330927338_p37256491200"><a name="zh-cn_topic_0000002330927338_p37256491200"></a><a name="zh-cn_topic_0000002330927338_p37256491200"></a>√</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002330927338_row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002330927338_p48327011813"><a name="zh-cn_topic_0000002330927338_p48327011813"></a><a name="zh-cn_topic_0000002330927338_p48327011813"></a><span id="zh-cn_topic_0000002330927338_ph583230201815"><a name="zh-cn_topic_0000002330927338_ph583230201815"></a><a name="zh-cn_topic_0000002330927338_ph583230201815"></a><term id="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002330927338_p7948163910184"><a name="zh-cn_topic_0000002330927338_p7948163910184"></a><a name="zh-cn_topic_0000002330927338_p7948163910184"></a>√</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002330927338_row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002330927338_p14832120181815"><a name="zh-cn_topic_0000002330927338_p14832120181815"></a><a name="zh-cn_topic_0000002330927338_p14832120181815"></a><span id="zh-cn_topic_0000002330927338_ph1483216010188"><a name="zh-cn_topic_0000002330927338_ph1483216010188"></a><a name="zh-cn_topic_0000002330927338_ph1483216010188"></a><term id="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000002330927338_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002330927338_p19948143911820"><a name="zh-cn_topic_0000002330927338_p19948143911820"></a><a name="zh-cn_topic_0000002330927338_p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

将当前Tensor重解释为用户指定的新类型，转换后的Tensor与原Tensor地址及内容完全相同，Tensor的大小（字节数）保持不变。

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename CAST_T> 
__aicore__ inline LocalTensor<CAST_T> ReinterpretCast() const
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001429830437_row118356578583"><th class="cellrowborder" valign="top" width="16.28%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001429830437_p48354572582"><a name="zh-cn_topic_0000001429830437_p48354572582"></a><a name="zh-cn_topic_0000001429830437_p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="83.72%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001429830437_p583535795817"><a name="zh-cn_topic_0000001429830437_p583535795817"></a><a name="zh-cn_topic_0000001429830437_p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001429830437_row1835857145817"><td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.2.3.1.1 "><p id="p2177206172414"><a name="p2177206172414"></a><a name="p2177206172414"></a>CAST_T</p>
</td>
<td class="cellrowborder" valign="top" width="83.72%" headers="mcps1.2.3.1.2 "><p id="p106801314162417"><a name="p106801314162417"></a><a name="p106801314162417"></a>用户指定的新类型。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

重解释转换后的Tensor。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section17531157161314"></a>

参考[调用示例](LocalTensor构造函数.md#section17531157161314)。

