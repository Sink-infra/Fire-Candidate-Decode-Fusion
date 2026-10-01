# aclrtcGetBinDataSize<a name="ZH-CN_TOPIC_0000002466837745"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002433359052_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002433359052_p1883113061818"><a name="zh-cn_topic_0000002433359052_p1883113061818"></a><a name="zh-cn_topic_0000002433359052_p1883113061818"></a><span id="zh-cn_topic_0000002433359052_ph20833205312295"><a name="zh-cn_topic_0000002433359052_ph20833205312295"></a><a name="zh-cn_topic_0000002433359052_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002433359052_p783113012187"><a name="zh-cn_topic_0000002433359052_p783113012187"></a><a name="zh-cn_topic_0000002433359052_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002433359052_row1272474920205"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002433359052_p17301775812"><a name="zh-cn_topic_0000002433359052_p17301775812"></a><a name="zh-cn_topic_0000002433359052_p17301775812"></a><span id="zh-cn_topic_0000002433359052_ph2272194216543"><a name="zh-cn_topic_0000002433359052_ph2272194216543"></a><a name="zh-cn_topic_0000002433359052_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002433359052_p7378143825218"><a name="zh-cn_topic_0000002433359052_p7378143825218"></a><a name="zh-cn_topic_0000002433359052_p7378143825218"></a>x</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002433359052_row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002433359052_p48327011813"><a name="zh-cn_topic_0000002433359052_p48327011813"></a><a name="zh-cn_topic_0000002433359052_p48327011813"></a><span id="zh-cn_topic_0000002433359052_ph583230201815"><a name="zh-cn_topic_0000002433359052_ph583230201815"></a><a name="zh-cn_topic_0000002433359052_ph583230201815"></a><term id="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002433359052_p7948163910184"><a name="zh-cn_topic_0000002433359052_p7948163910184"></a><a name="zh-cn_topic_0000002433359052_p7948163910184"></a>√</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002433359052_row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002433359052_p14832120181815"><a name="zh-cn_topic_0000002433359052_p14832120181815"></a><a name="zh-cn_topic_0000002433359052_p14832120181815"></a><span id="zh-cn_topic_0000002433359052_ph1483216010188"><a name="zh-cn_topic_0000002433359052_ph1483216010188"></a><a name="zh-cn_topic_0000002433359052_ph1483216010188"></a><term id="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000002433359052_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002433359052_p19948143911820"><a name="zh-cn_topic_0000002433359052_p19948143911820"></a><a name="zh-cn_topic_0000002433359052_p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

获取编译的二进制数据大小。用于在[aclrtcGetBinData](aclrtcGetBinData.md)获取二进制数据时分配对应大小的内存空间。

## 函数原型<a name="section620mcpsimp"></a>

```
aclError aclrtcGetBinDataSize(aclrtcProg prog, size_t *binDataSizeRet)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  接口参数说明

<a name="table10560024131915"></a>
<table><thead align="left"><tr id="row35607245197"><th class="cellrowborder" valign="top" width="17.580000000000002%" id="mcps1.2.4.1.1"><p id="p1356016243193"><a name="p1356016243193"></a><a name="p1356016243193"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="15.559999999999999%" id="mcps1.2.4.1.2"><p id="p092775021313"><a name="p092775021313"></a><a name="p092775021313"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="66.86%" id="mcps1.2.4.1.3"><p id="p20560122410195"><a name="p20560122410195"></a><a name="p20560122410195"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row15560192415195"><td class="cellrowborder" valign="top" width="17.580000000000002%" headers="mcps1.2.4.1.1 "><p id="p12769163425119"><a name="p12769163425119"></a><a name="p12769163425119"></a>prog</p>
</td>
<td class="cellrowborder" valign="top" width="15.559999999999999%" headers="mcps1.2.4.1.2 "><p id="p0927135021312"><a name="p0927135021312"></a><a name="p0927135021312"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="66.86%" headers="mcps1.2.4.1.3 "><p id="p1717692211109"><a name="p1717692211109"></a><a name="p1717692211109"></a>运行时编译程序的句柄。</p>
</td>
</tr>
<tr id="row13879152135218"><td class="cellrowborder" valign="top" width="17.580000000000002%" headers="mcps1.2.4.1.1 "><p id="p19982811135218"><a name="p19982811135218"></a><a name="p19982811135218"></a>binDataSizeRet</p>
</td>
<td class="cellrowborder" valign="top" width="15.559999999999999%" headers="mcps1.2.4.1.2 "><p id="p1792775011320"><a name="p1792775011320"></a><a name="p1792775011320"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="66.86%" headers="mcps1.2.4.1.3 "><p id="p1642191295410"><a name="p1642191295410"></a><a name="p1642191295410"></a>编译得到的Device侧二进制数据大小。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

aclError为int类型变量，详细说明请参考[RTC错误码](RTC错误码.md)。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section837496171220"></a>

```
aclrtcProg prog;
size_t binDataSizeRet = 0;
aclError result = aclrtcGetBinDataSize(prog, &binDataSizeRet);
```

