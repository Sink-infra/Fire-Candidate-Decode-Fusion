# IsFinite<a name="ZH-CN_TOPIC_0000002139196693"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="table1334714391211"></a>
<table><thead align="left"><tr id="row1334743121213"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p834713321216"><a name="p834713321216"></a><a name="p834713321216"></a><span id="ph834783101215"><a name="ph834783101215"></a><a name="ph834783101215"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p2347234127"><a name="p2347234127"></a><a name="p2347234127"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row113472312122"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p234710320128"><a name="p234710320128"></a><a name="p234710320128"></a><span id="ph103471336127"><a name="ph103471336127"></a><a name="ph103471336127"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p4751940181211"><a name="p4751940181211"></a><a name="p4751940181211"></a>√</p>
</td>
</tr>
<tr id="row1834733191219"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p1234716311218"><a name="p1234716311218"></a><a name="p1234716311218"></a><span id="ph434819391213"><a name="ph434819391213"></a><a name="ph434819391213"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7751240111217"><a name="p7751240111217"></a><a name="p7751240111217"></a>x</p>
</td>
</tr>
<tr id="row33481333123"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p2034813321217"><a name="p2034813321217"></a><a name="p2034813321217"></a><span id="ph334833191213"><a name="ph334833191213"></a><a name="ph334833191213"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p20751740131216"><a name="p20751740131216"></a><a name="p20751740131216"></a>x</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

按元素判断输入的浮点数是否非NAN、非±INF，输出结果为浮点数或者布尔值。对于非NAN或者非±INF的输入数据，当输出为浮点类型时，对应位置的结果为该浮点类型的1，反之为0；当输出为bool类型时，对应位置的结果为true，反之为false。计算公式如下：

![](figures/zh-cn_formulaimage_0000002218496437.png)

-   当输出为浮点类型时：

    ![](figures/zh-cn_formulaimage_0000002224017961.png)

-   当输出为bool类型时：

    ![](figures/zh-cn_formulaimage_0000002224061861.png)

## 函数原型<a name="section620mcpsimp"></a>

```
template<typename T, typename U>
__aicore__ inline void IsFinite(const LocalTensor<U>& dst, const LocalTensor<T>& src, uint32_t calCount)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table575571914269"></a>
<table><thead align="left"><tr id="row18755131942614"><th class="cellrowborder" valign="top" width="19.39%" id="mcps1.2.3.1.1"><p id="p675519193268"><a name="p675519193268"></a><a name="p675519193268"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.61%" id="mcps1.2.3.1.2"><p id="p375511918267"><a name="p375511918267"></a><a name="p375511918267"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row471717528218"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p125301542515"><a name="p125301542515"></a><a name="p125301542515"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p135303419517"><a name="p135303419517"></a><a name="p135303419517"></a>源操作数的数据类型。</p>
<p id="p1329111342610"><a name="p1329111342610"></a><a name="p1329111342610"></a><span id="ph49572819710"><a name="ph49572819710"></a><a name="ph49572819710"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、bfloat16_t、float。</p>
</td>
</tr>
<tr id="row14755141911264"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p47551198266"><a name="p47551198266"></a><a name="p47551198266"></a>U</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p125969172719"><a name="p125969172719"></a><a name="p125969172719"></a>目的操作数的数据类型。</p>
<p id="p1965414481962"><a name="p1965414481962"></a><a name="p1965414481962"></a><span id="ph114693403570"><a name="ph114693403570"></a><a name="ph114693403570"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：bool、half、bfloat16_t、float。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="table62161631132810"></a>
<table><thead align="left"><tr id="row12216103118284"><th class="cellrowborder" valign="top" width="13.661366136613662%" id="mcps1.2.4.1.1"><p id="p1421643114288"><a name="p1421643114288"></a><a name="p1421643114288"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="12.591259125912593%" id="mcps1.2.4.1.2"><p id="p82165310285"><a name="p82165310285"></a><a name="p82165310285"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="73.74737473747375%" id="mcps1.2.4.1.3"><p id="p1121663111288"><a name="p1121663111288"></a><a name="p1121663111288"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row82161131182810"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p248574352318"><a name="p248574352318"></a><a name="p248574352318"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="p144841743122315"><a name="p144841743122315"></a><a name="p144841743122315"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="p1565464814612"><a name="p1565464814612"></a><a name="p1565464814612"></a>目的操作数。</p>
<p id="p16703131355116"><a name="p16703131355116"></a><a name="p16703131355116"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p650124211103"><a name="p650124211103"></a><a name="p650124211103"></a>目的操作数的数据类型和源操作数保持一致，或者目的操作数的数据类型为bool类型。当前支持的数据类型组合请见<a href="#table158181847102411">表3</a>。</p>
</td>
</tr>
<tr id="row5216163192815"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p164821543202310"><a name="p164821543202310"></a><a name="p164821543202310"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="p64811843112313"><a name="p64811843112313"></a><a name="p64811843112313"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="p1329113341066"><a name="p1329113341066"></a><a name="p1329113341066"></a>源操作数。</p>
<p id="p3290735103018"><a name="p3290735103018"></a><a name="p3290735103018"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1212625414239"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p312613545238"><a name="p312613545238"></a><a name="p312613545238"></a>calCount</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="p10126254182313"><a name="p10126254182313"></a><a name="p10126254182313"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="p15110132519616"><a name="p15110132519616"></a><a name="p15110132519616"></a>参与计算的元素个数。</p>
</td>
</tr>
</tbody>
</table>

**表 3**  输入输出支持的数据类型组合

<a name="table158181847102411"></a>
<table><thead align="left"><tr id="row381964718248"><th class="cellrowborder" valign="top" width="45.910000000000004%" id="mcps1.2.3.1.1"><p id="p1681934711240"><a name="p1681934711240"></a><a name="p1681934711240"></a>srcDtype</p>
</th>
<th class="cellrowborder" valign="top" width="54.09%" id="mcps1.2.3.1.2"><p id="p48194471241"><a name="p48194471241"></a><a name="p48194471241"></a>dstDtype</p>
</th>
</tr>
</thead>
<tbody><tr id="row285104217615"><td class="cellrowborder" valign="top" width="45.910000000000004%" headers="mcps1.2.3.1.1 "><p id="p1681914742410"><a name="p1681914742410"></a><a name="p1681914742410"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="54.09%" headers="mcps1.2.3.1.2 "><p id="p1385120421762"><a name="p1385120421762"></a><a name="p1385120421762"></a>half</p>
</td>
</tr>
<tr id="row2085274214619"><td class="cellrowborder" valign="top" width="45.910000000000004%" headers="mcps1.2.3.1.1 "><p id="p5904192132920"><a name="p5904192132920"></a><a name="p5904192132920"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="54.09%" headers="mcps1.2.3.1.2 "><p id="p1385216421761"><a name="p1385216421761"></a><a name="p1385216421761"></a>bool</p>
</td>
</tr>
<tr id="row1185215421164"><td class="cellrowborder" valign="top" width="45.910000000000004%" headers="mcps1.2.3.1.1 "><p id="p511313117714"><a name="p511313117714"></a><a name="p511313117714"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="54.09%" headers="mcps1.2.3.1.2 "><p id="p985215424614"><a name="p985215424614"></a><a name="p985215424614"></a>float</p>
</td>
</tr>
<tr id="row1881954718248"><td class="cellrowborder" valign="top" width="45.910000000000004%" headers="mcps1.2.3.1.1 "><p id="p5901638112916"><a name="p5901638112916"></a><a name="p5901638112916"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="54.09%" headers="mcps1.2.3.1.2 "><p id="p019942995719"><a name="p019942995719"></a><a name="p019942995719"></a>bool</p>
</td>
</tr>
<tr id="row2819184711242"><td class="cellrowborder" valign="top" width="45.910000000000004%" headers="mcps1.2.3.1.1 "><p id="p8137120105714"><a name="p8137120105714"></a><a name="p8137120105714"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="54.09%" headers="mcps1.2.3.1.2 "><p id="p94751112115810"><a name="p94751112115810"></a><a name="p94751112115810"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row12819174742414"><td class="cellrowborder" valign="top" width="45.910000000000004%" headers="mcps1.2.3.1.1 "><p id="p12176124511299"><a name="p12176124511299"></a><a name="p12176124511299"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="54.09%" headers="mcps1.2.3.1.2 "><p id="p144751123587"><a name="p144751123587"></a><a name="p144751123587"></a>bool</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section91032023123812"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   **不支持源操作数与目的操作数地址重叠。**
-   操作数地址偏移对齐要求请参见[通用说明和约束](通用说明和约束.md)。

## 调用示例<a name="section642mcpsimp"></a>

```
#include "kernel_operator.h"
 
AscendC::LocalTensor<SrcT> xLocal = inQueueX.DeQue<SrcT>();
if constexpr (std::is_same_v<DstT, bool>) {
    AscendC::LocalTensor<int8_t> yLocal = outQueueY.AllocTensor<int8_t>();
    AscendC::Duplicate(yLocal, (int8_t)0, dataSize);
    AscendC::IsFinite(yLocal, xLocal, calCount);
    outQueueY.EnQue(yLocal);
} else {
    AscendC::LocalTensor<DstT> yLocal = outQueueY.AllocTensor<DstT>();
    AscendC::Duplicate(yLocal, (DstT)0, dataSize);
    AscendC::IsFinite(yLocal, xLocal, calCount);
    outQueueY.EnQue(yLocal);
}
inQueueX.FreeTensor(xLocal);
```

结果示例如下：

```
输入的数据类型为float，输出的数据类型为bool
输入数据(src):
[1.0,+inf,3.0,4.0,nan,6.0,-inf,8.0]
输出数据(dst):
[true,false,true,true,false,true,false,true]
```

