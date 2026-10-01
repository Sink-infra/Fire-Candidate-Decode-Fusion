# SetFixpipeNz2ndFlag<a name="ZH-CN_TOPIC_0000001788021086"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="table38301303189"></a>
<table><thead align="left"><tr id="row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p1883113061818"><a name="p1883113061818"></a><a name="p1883113061818"></a><span id="ph20833205312295"><a name="ph20833205312295"></a><a name="ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p783113012187"><a name="p783113012187"></a><a name="p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row1272474920205"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p17301775812"><a name="p17301775812"></a><a name="p17301775812"></a><span id="ph2272194216543"><a name="ph2272194216543"></a><a name="ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p37256491200"><a name="p37256491200"></a><a name="p37256491200"></a>√</p>
</td>
</tr>
<tr id="row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p48327011813"><a name="p48327011813"></a><a name="p48327011813"></a><span id="ph583230201815"><a name="ph583230201815"></a><a name="ph583230201815"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7948163910184"><a name="p7948163910184"></a><a name="p7948163910184"></a>√</p>
</td>
</tr>
<tr id="row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p14832120181815"><a name="p14832120181815"></a><a name="p14832120181815"></a><span id="ph1483216010188"><a name="ph1483216010188"></a><a name="ph1483216010188"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p19948143911820"><a name="p19948143911820"></a><a name="p19948143911820"></a>√</p>
</td>
</tr>
<tr id="row1576562114215"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p7743750164"><a name="p7743750164"></a><a name="p7743750164"></a><span id="ph171873262101"><a name="ph171873262101"></a><a name="ph171873262101"></a>Kirin X90</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p35541780589"><a name="p35541780589"></a><a name="p35541780589"></a>√</p>
</td>
</tr>
<tr id="row6928141917428"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p277014435153"><a name="p277014435153"></a><a name="p277014435153"></a><span id="ph081875221011"><a name="ph081875221011"></a><a name="ph081875221011"></a>Kirin  9030</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p1992311175812"><a name="p1992311175812"></a><a name="p1992311175812"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

[DataCopy](随路量化激活搬运.md)（CO1-\>GM、CO1-\>A1）过程中进行随路格式转换（NZ格式转换为ND格式）时，通过调用该接口设置格式转换的相关配置。

## 函数原型<a name="section620mcpsimp"></a>

```
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)

__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

<a name="table18368155193919"></a>
<table><thead align="left"><tr id="row1036805543911"><th class="cellrowborder" valign="top" width="16.371637163716375%" id="mcps1.2.4.1.1"><p id="p1836835511393"><a name="p1836835511393"></a><a name="p1836835511393"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="11.341134113411341%" id="mcps1.2.4.1.2"><p id="p10368255163915"><a name="p10368255163915"></a><a name="p10368255163915"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.28722872287229%" id="mcps1.2.4.1.3"><p id="p436875573911"><a name="p436875573911"></a><a name="p436875573911"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row1436825518395"><td class="cellrowborder" valign="top" width="16.371637163716375%" headers="mcps1.2.4.1.1 "><p id="p9649151061720"><a name="p9649151061720"></a><a name="p9649151061720"></a>ndNum</p>
</td>
<td class="cellrowborder" valign="top" width="11.341134113411341%" headers="mcps1.2.4.1.2 "><p id="p1649121041718"><a name="p1649121041718"></a><a name="p1649121041718"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.28722872287229%" headers="mcps1.2.4.1.3 "><p id="p17387131015120"><a name="p17387131015120"></a><a name="p17387131015120"></a>nd的数量，类型是uint16_t，取值范围：ndNum∈[1, 65535]。</p>
</td>
</tr>
<tr id="row1836875519393"><td class="cellrowborder" valign="top" width="16.371637163716375%" headers="mcps1.2.4.1.1 "><p id="p7650141019171"><a name="p7650141019171"></a><a name="p7650141019171"></a>srcNdStride</p>
</td>
<td class="cellrowborder" valign="top" width="11.341134113411341%" headers="mcps1.2.4.1.2 "><p id="p4650610141715"><a name="p4650610141715"></a><a name="p4650610141715"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.28722872287229%" headers="mcps1.2.4.1.3 "><p id="p10585131383420"><a name="p10585131383420"></a><a name="p10585131383420"></a>以分形大小为单位的源步长，源相邻nz矩阵的偏移（头与头）。</p>
<p id="p79581856916"><a name="p79581856916"></a><a name="p79581856916"></a><span id="ph1958656219"><a name="ph1958656219"></a><a name="ph1958656219"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，<span>srcNdStride∈</span>[1, 512]，单位：fractal_size 1024B。</p>
<p id="p13958135618116"><a name="p13958135618116"></a><a name="p13958135618116"></a><span id="ph095816561116"><a name="ph095816561116"></a><a name="ph095816561116"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，<span>srcNdStride∈</span>[1, 512]，单位：fractal_size 1024B。</p>
<p id="p14959205612119"><a name="p14959205612119"></a><a name="p14959205612119"></a><span id="ph195955610116"><a name="ph195955610116"></a><a name="ph195955610116"></a>Ascend 950PR/Ascend 950DT</span>，<span>srcNdStride∈</span>[0, 65535]，单位：C0_SIZE。</p>
<p id="p8541115114910"><a name="p8541115114910"></a><a name="p8541115114910"></a><span id="ph1733213734914"><a name="ph1733213734914"></a><a name="ph1733213734914"></a>Kirin X90</span>，<span>srcNdStride∈</span>[0, 65535]，单位：C0_SIZE。</p>
<p id="p237021654914"><a name="p237021654914"></a><a name="p237021654914"></a><span id="ph122164011499"><a name="ph122164011499"></a><a name="ph122164011499"></a>Kirin 9030</span>，<span>srcNdStride∈</span>[0, 65535]，单位：C0_SIZE。</p>
</td>
</tr>
<tr id="row1767431631917"><td class="cellrowborder" valign="top" width="16.371637163716375%" headers="mcps1.2.4.1.1 "><p id="p667418162198"><a name="p667418162198"></a><a name="p667418162198"></a>dstNdStride</p>
</td>
<td class="cellrowborder" valign="top" width="11.341134113411341%" headers="mcps1.2.4.1.2 "><p id="p11675191610195"><a name="p11675191610195"></a><a name="p11675191610195"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.28722872287229%" headers="mcps1.2.4.1.3 "><p id="p358320412029"><a name="p358320412029"></a><a name="p358320412029"></a>目的相邻nd矩阵的偏移（头与头）。单位为元素。</p>
<p id="p203221626139"><a name="p203221626139"></a><a name="p203221626139"></a><span id="ph83222261313"><a name="ph83222261313"></a><a name="ph83222261313"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>，dstNdStride∈[1, 65535]。</p>
<p id="p11322122616318"><a name="p11322122616318"></a><a name="p11322122616318"></a><span id="ph10322132617317"><a name="ph10322132617317"></a><a name="ph10322132617317"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>，dstNdStride∈[1, 65535]。</p>
<p id="p133229262318"><a name="p133229262318"></a><a name="p133229262318"></a><span id="ph19322172615318"><a name="ph19322172615318"></a><a name="ph19322172615318"></a>Ascend 950PR/Ascend 950DT</span>，dstNdStride∈[1, 2<sup id="sup13486142919415"><a name="sup13486142919415"></a><a name="sup13486142919415"></a>32</sup> -1]。</p>
<p id="p1352212185012"><a name="p1352212185012"></a><a name="p1352212185012"></a><span id="ph1852312114501"><a name="ph1852312114501"></a><a name="ph1852312114501"></a>Kirin X90</span>，<span>dstNdStride∈</span>[0, 65535]。</p>
<p id="p1552351175014"><a name="p1552351175014"></a><a name="p1552351175014"></a><span id="ph8523141155010"><a name="ph8523141155010"></a><a name="ph8523141155010"></a>Kirin 9030</span>，<span>dstNdStride∈</span>[0, 65535]。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section6461234123118"></a>

完整示例可参考[完整示例](随路量化激活搬运.md#li178441955134010)。

```
uint16_t ndNum = 2;
uint16_t srcNdStride = 2;
uint16_t dstNdStride = 1;
AscendC::SetFixpipeNz2ndFlag(ndNum, srcNdStride, dstNdStride); // 设置FIX搬运NZ格式到ND格式转换的参数
```

