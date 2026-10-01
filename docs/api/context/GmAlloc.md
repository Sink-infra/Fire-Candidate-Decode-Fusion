# GmAlloc<a name="ZH-CN_TOPIC_0000002044684566"></a>

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
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p10932155111300"><a name="p10932155111300"></a><a name="p10932155111300"></a>√</p>
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
<tr id="row164131535248"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p741411312247"><a name="p741411312247"></a><a name="p741411312247"></a><span id="ph1541318345248"><a name="ph1541318345248"></a><a name="ph1541318345248"></a>Kirin X90</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p241417316249"><a name="p241417316249"></a><a name="p241417316249"></a>√</p>
</td>
</tr>
<tr id="row5587359243"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p115874532412"><a name="p115874532412"></a><a name="p115874532412"></a><span id="ph137970361244"><a name="ph137970361244"></a><a name="ph137970361244"></a>Kirin 9030</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p65871511240"><a name="p65871511240"></a><a name="p65871511240"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_section259105813316"></a>

进行核函数的CPU侧运行验证时，用于创建共享内存：在/tmp目录下创建一个共享文件，并返回该文件的映射指针。

## 函数原型<a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_section2067518173415"></a>

```
void *GmAlloc(size_t size)
```

## 参数说明<a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_section158061867342"></a>

<a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="16.49%" id="mcps1.1.4.1.1"><p id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="11.93%" id="mcps1.1.4.1.2"><p id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="71.58%" id="mcps1.1.4.1.3"><p id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_row42461942101815"><td class="cellrowborder" valign="top" width="16.49%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p284425844311"><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p284425844311"></a><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p284425844311"></a>size</p>
</td>
<td class="cellrowborder" valign="top" width="11.93%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p158449584436"><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p158449584436"></a><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p158449584436"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="71.58%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p297233812230"><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p297233812230"></a><a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_p297233812230"></a>用户想要申请的共享内存大小</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_section640mcpsimp"></a>

返回该共享内存空间的首地址。

## 约束说明<a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_section794123819592"></a>

该接口在系统的/tmp目录下生成临时文件，故需要磁盘空间足够才可以正常生成共享内存。

## 调用示例<a name="zh-cn_topic_0000001963639306_zh-cn_topic_0000001541764188_section82241477610"></a>

```
constexpr size_t len = 8 * 32 * 1024 * 8;
half* x = (half*) GmAlloc(len*sizeof(half));
```

