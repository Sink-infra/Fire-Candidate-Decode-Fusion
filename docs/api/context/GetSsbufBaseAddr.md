# GetSsbufBaseAddr<a name="ZH-CN_TOPIC_0000002542906667"></a>

## 产品支持情况<a name="section11658125112438"></a>

<a name="table1165815116436"></a>
<table><thead align="left"><tr id="row165885144318"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p5658175124313"><a name="p5658175124313"></a><a name="p5658175124313"></a><span id="ph1865816515435"><a name="ph1865816515435"></a><a name="ph1865816515435"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p2065805124318"><a name="p2065805124318"></a><a name="p2065805124318"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row6658115154311"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p2658175111436"><a name="p2658175111436"></a><a name="p2658175111436"></a><span id="ph96581951124313"><a name="ph96581951124313"></a><a name="ph96581951124313"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p5658351144316"><a name="p5658351144316"></a><a name="p5658351144316"></a>√</p>
</td>
</tr>
<tr id="row1165815515434"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p8658145110431"><a name="p8658145110431"></a><a name="p8658145110431"></a><span id="ph13658185110434"><a name="ph13658185110434"></a><a name="ph13658185110434"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p66591051144318"><a name="p66591051144318"></a><a name="p66591051144318"></a>x</p>
</td>
</tr>
<tr id="row865918513433"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p10659051114310"><a name="p10659051114310"></a><a name="p10659051114310"></a><span id="ph36591351154313"><a name="ph36591351154313"></a><a name="ph36591351154313"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7659175114439"><a name="p7659175114439"></a><a name="p7659175114439"></a>x</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section1966075164312"></a>

该接口用于获取SSBuffer的基地址。在AIC和AIV核间通信的场景下使用该接口可能产生不确定问题。

## 函数原型<a name="section266175134310"></a>

```
__aicore__ inline __ssbuf__ void*  GetSsbufBaseAddr()
```

## 参数说明<a name="section766115164313"></a>

无

## 返回值说明<a name="section166616514437"></a>

返回指向SSBuffer基地址的指针。

## 约束说明<a name="section156613513435"></a>

无

## 调用示例<a name="section11661185118437"></a>

```
 __ssbuf__ void* ssbuf = GetSsbufBaseAddr();
```

