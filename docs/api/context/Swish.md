# Swish<a name="ZH-CN_TOPIC_0000001728771777"></a>

## 产品支持情况<a name="section1586581915393"></a>

<a name="table169596713360"></a>
<table><thead align="left"><tr id="row129590715369"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p17959971362"><a name="p17959971362"></a><a name="p17959971362"></a><span id="ph895914718367"><a name="ph895914718367"></a><a name="ph895914718367"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p89594763612"><a name="p89594763612"></a><a name="p89594763612"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row18959673369"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p1595910763613"><a name="p1595910763613"></a><a name="p1595910763613"></a><span id="ph1595918753613"><a name="ph1595918753613"></a><a name="ph1595918753613"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p1695957133611"><a name="p1695957133611"></a><a name="p1695957133611"></a>√</p>
</td>
</tr>
<tr id="row18959157103612"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p13959117193618"><a name="p13959117193618"></a><a name="p13959117193618"></a><span id="ph9959117173614"><a name="ph9959117173614"></a><a name="ph9959117173614"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
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

## 功能说明<a name="section785018556590"></a>

在神经网络中，Swish是一个重要的激活函数。计算公式如下，其中β为常数：

![](figures/zh-cn_formulaimage_0000002316174174.png)

![](figures/zh-cn_formulaimage_0000001680848998.png)

## 函数原型<a name="section8850255125911"></a>

```
template <typename T, bool isReuseSource = false>
__aicore__ inline void Swish(const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, uint32_t dataSize, const T scalarValue)
```

## 参数说明<a name="section1085025505914"></a>

**表 1**  模板参数说明

<a name="table575571914269"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001538537601_row18755131942614"><th class="cellrowborder" valign="top" width="19.39%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001538537601_p675519193268"><a name="zh-cn_topic_0000001538537601_p675519193268"></a><a name="zh-cn_topic_0000001538537601_p675519193268"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.61%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001538537601_p375511918267"><a name="zh-cn_topic_0000001538537601_p375511918267"></a><a name="zh-cn_topic_0000001538537601_p375511918267"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001538537601_row14755141911264"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001538537601_p47551198266"><a name="zh-cn_topic_0000001538537601_p47551198266"></a><a name="zh-cn_topic_0000001538537601_p47551198266"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001538537601_p125969172719"><a name="zh-cn_topic_0000001538537601_p125969172719"></a><a name="zh-cn_topic_0000001538537601_p125969172719"></a>操作数的数据类型。</p>
<p id="p199333397155"><a name="p199333397155"></a><a name="p199333397155"></a><span id="ph183108134329"><a name="ph183108134329"></a><a name="ph183108134329"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、float。</p>
<p id="zh-cn_topic_0000001538537601_p44574117347"><a name="zh-cn_topic_0000001538537601_p44574117347"></a><a name="zh-cn_topic_0000001538537601_p44574117347"></a><span id="ph850781713275"><a name="ph850781713275"></a><a name="ph850781713275"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half、float。</p>
<p id="zh-cn_topic_0000001538537601_p10451541143416"><a name="zh-cn_topic_0000001538537601_p10451541143416"></a><a name="zh-cn_topic_0000001538537601_p10451541143416"></a><span id="ph25832792713"><a name="ph25832792713"></a><a name="ph25832792713"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half、float。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001538537601_row9756719122620"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001538537601_p1682112447268"><a name="zh-cn_topic_0000001538537601_p1682112447268"></a><a name="zh-cn_topic_0000001538537601_p1682112447268"></a>isReuseSource</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001538537601_p98212044172612"><a name="zh-cn_topic_0000001538537601_p98212044172612"></a><a name="zh-cn_topic_0000001538537601_p98212044172612"></a>是否允许修改源操作数。该参数预留，传入默认值false即可。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="table1485015517590"></a>
<table><thead align="left"><tr id="row885118552595"><th class="cellrowborder" valign="top" width="19.24%" id="mcps1.2.4.1.1"><p id="p1585195518592"><a name="p1585195518592"></a><a name="p1585195518592"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="12.23%" id="mcps1.2.4.1.2"><p id="p0851185511597"><a name="p0851185511597"></a><a name="p0851185511597"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="68.53%" id="mcps1.2.4.1.3"><p id="p1785175516591"><a name="p1785175516591"></a><a name="p1785175516591"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2851125520594"><td class="cellrowborder" valign="top" width="19.24%" headers="mcps1.2.4.1.1 "><p id="p9851165515593"><a name="p9851165515593"></a><a name="p9851165515593"></a>dstLocal</p>
</td>
<td class="cellrowborder" valign="top" width="12.23%" headers="mcps1.2.4.1.2 "><p id="p1785185514591"><a name="p1785185514591"></a><a name="p1785185514591"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="68.53%" headers="mcps1.2.4.1.3 "><p id="p9255193274511"><a name="p9255193274511"></a><a name="p9255193274511"></a>目的操作数。</p>
<p id="p12851115519599"><a name="p12851115519599"></a><a name="p12851115519599"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row6851155510593"><td class="cellrowborder" valign="top" width="19.24%" headers="mcps1.2.4.1.1 "><p id="p1385135518597"><a name="p1385135518597"></a><a name="p1385135518597"></a>srcLocal</p>
</td>
<td class="cellrowborder" valign="top" width="12.23%" headers="mcps1.2.4.1.2 "><p id="p585119553596"><a name="p585119553596"></a><a name="p585119553596"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="68.53%" headers="mcps1.2.4.1.3 "><p id="p963863814519"><a name="p963863814519"></a><a name="p963863814519"></a>源操作数。</p>
<p id="p493465115344"><a name="p493465115344"></a><a name="p493465115344"></a>源操作数的数据类型需要与目的操作数保持一致。</p>
<p id="p15450144034510"><a name="p15450144034510"></a><a name="p15450144034510"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row4852185535916"><td class="cellrowborder" valign="top" width="19.24%" headers="mcps1.2.4.1.1 "><p id="p14852105575915"><a name="p14852105575915"></a><a name="p14852105575915"></a>dataSize</p>
</td>
<td class="cellrowborder" valign="top" width="12.23%" headers="mcps1.2.4.1.2 "><p id="p168521855115913"><a name="p168521855115913"></a><a name="p168521855115913"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="68.53%" headers="mcps1.2.4.1.3 "><p id="p7852355195919"><a name="p7852355195919"></a><a name="p7852355195919"></a>实际计算数据元素个数。</p>
</td>
</tr>
<tr id="row204461978565"><td class="cellrowborder" valign="top" width="19.24%" headers="mcps1.2.4.1.1 "><p id="p1244747105613"><a name="p1244747105613"></a><a name="p1244747105613"></a>scalarValue</p>
</td>
<td class="cellrowborder" valign="top" width="12.23%" headers="mcps1.2.4.1.2 "><p id="p44478765615"><a name="p44478765615"></a><a name="p44478765615"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="68.53%" headers="mcps1.2.4.1.3 "><p id="p5692173212345"><a name="p5692173212345"></a><a name="p5692173212345"></a>激活函数中的β参数。支持的数据类型为：half、float。</p>
<p id="p14644132617358"><a name="p14644132617358"></a><a name="p14644132617358"></a>β参数的数据类型需要与源操作数和目的操作数保持一致。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section38228281712"></a>

无

## 约束说明<a name="section11852175575912"></a>

-   操作数地址偏移对齐要求请参见[通用说明和约束](通用说明和约束.md)。
-   **不支持源操作数与目的操作数地址重叠。**
-   当前仅支持ND格式的输入，不支持其他格式。

## 调用示例<a name="section208521655195918"></a>

```
#include "kernel_operator.h"

AscendC::LocalTensor<T> dstLocal = outQueue.AllocTensor<T>();
AscendC::LocalTensor<T> srcLocal = inQueueX.DeQue<T>();
AscendC::Swish(dstLocal, srcLocal, dataSize, scalarValue);
outQueue.EnQue<T>(dstLocal);
inQueueX.FreeTensor(srcLocal);
```

结果示例如下：

```
输入数据(srcLocal): 
[ 0.5312  -3.654   -2.92     3.787   -3.059    3.77     0.571   -0.668
 -0.09534  0.5454  -1.801   -1.791    1.563    0.878    3.973    1.799
  2.023    1.018    3.082   -3.814    2.254   -3.717    0.4675  -0.4631
 -2.47     0.9814  -0.854    3.31     3.256    3.764    1.867   -1.773]
输出数据(dstLocal): 
[ 0.3784   -0.007263 -0.02016   3.78     -0.01666   3.762     0.414
 -0.1622   -0.04382   0.3909   -0.0803   -0.08105   1.461     0.717
  3.969     1.719     1.96      0.8647    3.066    -0.00577   2.207
 -0.006626  0.3223   -0.1448   -0.03622   0.8257   -0.1617    3.297
  3.244     3.756     1.792    -0.0825]
```

