# SetBias<a name="ZH-CN_TOPIC_0000002100033042"></a>

## 产品支持情况<a name="section1586581915393"></a>

<a name="table169596713360"></a>
<table><thead align="left"><tr id="row129590715369"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p17959971362"><a name="p17959971362"></a><a name="p17959971362"></a><span id="ph895914718367"><a name="ph895914718367"></a><a name="ph895914718367"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p89594763612"><a name="p89594763612"></a><a name="p89594763612"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row18959157103612"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p13959117193618"><a name="p13959117193618"></a><a name="p13959117193618"></a><span id="ph9959117173614"><a name="ph9959117173614"></a><a name="ph9959117173614"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p1095914793613"><a name="p1095914793613"></a><a name="p1095914793613"></a>√</p>
</td>
</tr>
<tr id="row89591478362"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p7959157163619"><a name="p7959157163619"></a><a name="p7959157163619"></a><span id="ph1995997193619"><a name="ph1995997193619"></a><a name="ph1995997193619"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p149598793615"><a name="p149598793615"></a><a name="p149598793615"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section2935259242"></a>

设置偏置矩阵Bias。

## 函数原型<a name="section2012220138516"></a>

```
__aicore__ inline void SetBias(const AscendC::GlobalTensor<BiasT>& bias)
```

## 参数说明<a name="section2078510234519"></a>

<a name="table10876123516424"></a>
<table><thead align="left"><tr id="row16894435184216"><th class="cellrowborder" valign="top" width="16.541654165416542%" id="mcps1.1.4.1.1"><p id="p1789433584215"><a name="p1789433584215"></a><a name="p1789433584215"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="19.921992199219922%" id="mcps1.1.4.1.2"><p id="p19894143513427"><a name="p19894143513427"></a><a name="p19894143513427"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="63.53635363536353%" id="mcps1.1.4.1.3"><p id="p5894113516426"><a name="p5894113516426"></a><a name="p5894113516426"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row58944359427"><td class="cellrowborder" valign="top" width="16.541654165416542%" headers="mcps1.1.4.1.1 "><p id="p28941355422"><a name="p28941355422"></a><a name="p28941355422"></a>bias</p>
</td>
<td class="cellrowborder" valign="top" width="19.921992199219922%" headers="mcps1.1.4.1.2 "><p id="p9894535204216"><a name="p9894535204216"></a><a name="p9894535204216"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="63.53635363536353%" headers="mcps1.1.4.1.3 "><p id="p188941435124215"><a name="p188941435124215"></a><a name="p188941435124215"></a>Bias在Global Memory上的地址。<span id="ph15942199192220"><a name="ph15942199192220"></a><a name="ph15942199192220"></a><span id="ph1294215916225"><a name="ph1294215916225"></a><a name="ph1294215916225"></a><span id="ph894279182218"><a name="ph894279182218"></a><a name="ph894279182218"></a>类型为<a href="GlobalTensor.md">GlobalTensor</a>。</span></span></span>偏置矩阵Bias支持的数据类型为：half、bfloat16_t。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section112237312518"></a>

无

## 约束说明<a name="section414020501054"></a>

在卷积计算中，如果涉及偏置矩阵Bias，必须调用此接口；若卷积计算不涉及Bias，则不应调用此接口。

## 调用示例<a name="section09801457159"></a>

```
GlobalTensor<float> biasGm;
biasGm.SetGlobalBuffer(reinterpret_cast<__gm__ half *>(bias));
if (biasFlag) {
    conv3dApi.SetBias(biasGm);
}
```

