# InitConstValue<a name="ZH-CN_TOPIC_0000001834660669"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="table38301303189"></a>
<table><thead align="left"><tr id="row20831180131817"><th class="cellrowborder" valign="top" width="57.96%" id="mcps1.1.3.1.1"><p id="p1883113061818"><a name="p1883113061818"></a><a name="p1883113061818"></a><span id="ph20833205312295"><a name="ph20833205312295"></a><a name="ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42.04%" id="mcps1.1.3.1.2"><p id="p783113012187"><a name="p783113012187"></a><a name="p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row220181016240"><td class="cellrowborder" valign="top" width="57.96%" headers="mcps1.1.3.1.1 "><p id="p48327011813"><a name="p48327011813"></a><a name="p48327011813"></a><span id="ph583230201815"><a name="ph583230201815"></a><a name="ph583230201815"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.04%" headers="mcps1.1.3.1.2 "><p id="p7948163910184"><a name="p7948163910184"></a><a name="p7948163910184"></a>√</p>
</td>
</tr>
<tr id="row173226882415"><td class="cellrowborder" valign="top" width="57.96%" headers="mcps1.1.3.1.1 "><p id="p14832120181815"><a name="p14832120181815"></a><a name="p14832120181815"></a><span id="ph1483216010188"><a name="ph1483216010188"></a><a name="ph1483216010188"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.04%" headers="mcps1.1.3.1.2 "><p id="p19948143911820"><a name="p19948143911820"></a><a name="p19948143911820"></a>√</p>
</td>
</tr>
<tr id="row8325205911538"><td class="cellrowborder" valign="top" width="57.96%" headers="mcps1.1.3.1.1 "><p id="p7743750164"><a name="p7743750164"></a><a name="p7743750164"></a><span id="ph171873262101"><a name="ph171873262101"></a><a name="ph171873262101"></a>Kirin X90</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.04%" headers="mcps1.1.3.1.2 "><p id="p167558406157"><a name="p167558406157"></a><a name="p167558406157"></a>√</p>
</td>
</tr>
<tr id="row97311222121319"><td class="cellrowborder" valign="top" width="57.96%" headers="mcps1.1.3.1.1 "><p id="p41641126141314"><a name="p41641126141314"></a><a name="p41641126141314"></a><span id="ph14164162661314"><a name="ph14164162661314"></a><a name="ph14164162661314"></a>Kirin 9030</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.04%" headers="mcps1.1.3.1.2 "><p id="p71641226151312"><a name="p71641226151312"></a><a name="p71641226151312"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

将特定TPosition的LocalTensor初始化为某一具体数值。

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T, typename U = PrimT<T>, typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
__aicore__ inline void InitConstValue(const LocalTensor<T>& dst, const InitConstValueParams<U>& initConstValueParams)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001429830437_row118356578583"><th class="cellrowborder" valign="top" width="17.43%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001429830437_p48354572582"><a name="zh-cn_topic_0000001429830437_p48354572582"></a><a name="zh-cn_topic_0000001429830437_p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="82.57%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001429830437_p583535795817"><a name="zh-cn_topic_0000001429830437_p583535795817"></a><a name="zh-cn_topic_0000001429830437_p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001429830437_row1835857145817"><td class="cellrowborder" valign="top" width="17.43%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001429830437_p5835457165816"><a name="zh-cn_topic_0000001429830437_p5835457165816"></a><a name="zh-cn_topic_0000001429830437_p5835457165816"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="82.57%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001429830437_p168351657155818"><a name="zh-cn_topic_0000001429830437_p168351657155818"></a><a name="zh-cn_topic_0000001429830437_p168351657155818"></a>dst的数据类型。</p>
<p id="p990614114434"><a name="p990614114434"></a><a name="p990614114434"></a><span id="ph126684217571"><a name="ph126684217571"></a><a name="ph126684217571"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t</p>
<p id="p523904010149"><a name="p523904010149"></a><a name="p523904010149"></a><span id="ph16239174011416"><a name="ph16239174011416"></a><a name="ph16239174011416"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t</p>
<p id="p1916932318559"><a name="p1916932318559"></a><a name="p1916932318559"></a><span id="ph10169122365515"><a name="ph10169122365515"></a><a name="ph10169122365515"></a>Kirin X90</span>，支持的数据类型为：half/int8_t</p>
<p id="p18181249132017"><a name="p18181249132017"></a><a name="p18181249132017"></a><span id="ph16103105042019"><a name="ph16103105042019"></a><a name="ph16103105042019"></a>Kirin 9030</span>，支持的数据类型为：half</p>
</td>
</tr>
<tr id="row118213273213"><td class="cellrowborder" valign="top" width="17.43%" headers="mcps1.2.3.1.1 "><p id="p161827233218"><a name="p161827233218"></a><a name="p161827233218"></a>U</p>
</td>
<td class="cellrowborder" valign="top" width="82.57%" headers="mcps1.2.3.1.2 "><p id="p15182229327"><a name="p15182229327"></a><a name="p15182229327"></a>初始化值的数据类型。</p>
<a name="ul17416131483320"></a><a name="ul17416131483320"></a><ul id="ul17416131483320"><li>当dst使用基础数据类型时， U和dst的数据类型T需保持一致，否则编译失败。</li><li>当dst使用<a href="TensorTrait.md">TensorTrait</a>类型时，U和dst的数据类型T的LiteType需保持一致，否则编译失败。</li></ul>
<p id="p472714219338"><a name="p472714219338"></a><a name="p472714219338"></a>最后一个模板参数仅用于上述数据类型检查，用户无需关注。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="table8955841508"></a>
<table><thead align="left"><tr id="row15956194105014"><th class="cellrowborder" valign="top" width="17.661766176617665%" id="mcps1.2.4.1.1"><p id="p7956144195014"><a name="p7956144195014"></a><a name="p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="8.670867086708672%" id="mcps1.2.4.1.2"><p id="p1295624145013"><a name="p1295624145013"></a><a name="p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="73.66736673667367%" id="mcps1.2.4.1.3"><p id="p16956144145011"><a name="p16956144145011"></a><a name="p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row5956546509"><td class="cellrowborder" valign="top" width="17.661766176617665%" headers="mcps1.2.4.1.1 "><p id="p19287714181617"><a name="p19287714181617"></a><a name="p19287714181617"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.670867086708672%" headers="mcps1.2.4.1.2 "><p id="p192871614151615"><a name="p192871614151615"></a><a name="p192871614151615"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="73.66736673667367%" headers="mcps1.2.4.1.3 "><p id="p232011551739"><a name="p232011551739"></a><a name="p232011551739"></a>目的操作数，结果矩阵，类型为LocalTensor。</p>
<p id="p17644116247"><a name="p17644116247"></a><a name="p17644116247"></a><span id="ph26443161442"><a name="ph26443161442"></a><a name="ph26443161442"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>，支持的TPosition为A1/A2/B1/B2。</p>
<p id="p56447161944"><a name="p56447161944"></a><a name="p56447161944"></a><span id="ph3644016948"><a name="ph3644016948"></a><a name="ph3644016948"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>，支持的TPosition为A1/A2/B1/B2。</p>
<p id="p1951017519562"><a name="p1951017519562"></a><a name="p1951017519562"></a><span id="ph458106105616"><a name="ph458106105616"></a><a name="ph458106105616"></a>Kirin X90</span>，支持的TPosition为A1/A2/B1/B2。</p>
<p id="p5899141520216"><a name="p5899141520216"></a><a name="p5899141520216"></a><span id="ph15899201514212"><a name="ph15899201514212"></a><a name="ph15899201514212"></a>Kirin 9030</span>，支持的TPosition为A1/A2/B1/B2。</p>
<p id="p16287121461618"><a name="p16287121461618"></a><a name="p16287121461618"></a>如果TPosition为A1/B1，起始地址需要满足32B对齐；如果TPosition为A2/B2，起始地址需要满足512B对齐。</p>
</td>
</tr>
<tr id="row1075785651510"><td class="cellrowborder" valign="top" width="17.661766176617665%" headers="mcps1.2.4.1.1 "><p id="p486615557145"><a name="p486615557145"></a><a name="p486615557145"></a>InitConstValueParams</p>
</td>
<td class="cellrowborder" valign="top" width="8.670867086708672%" headers="mcps1.2.4.1.2 "><p id="p11287151451610"><a name="p11287151451610"></a><a name="p11287151451610"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.66736673667367%" headers="mcps1.2.4.1.3 "><p id="p17376814155615"><a name="p17376814155615"></a><a name="p17376814155615"></a>初始化相关参数，类型为InitConstValueParams。</p>
<p id="p595519531047"><a name="p595519531047"></a><a name="p595519531047"></a>具体定义请参考<span id="ph10562197165916"><a name="ph10562197165916"></a><a name="ph10562197165916"></a>${INSTALL_DIR}</span>/include/ascendc/basic_api/interface/kernel_struct_mm.h，<span id="ph14322531015"><a name="ph14322531015"></a><a name="ph14322531015"></a>${INSTALL_DIR}</span>请替换为CANN软件安装后文件存储路径。</p>
<p id="p12287014111614"><a name="p12287014111614"></a><a name="p12287014111614"></a>参数说明请参考<a href="#table15780447181917">表3</a>。</p>
<p id="p179517269366"><a name="p179517269366"></a><a name="p179517269366"></a><span id="ph779572693610"><a name="ph779572693610"></a><a name="ph779572693610"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_3"><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_3"><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a>Atlas A2 推理系列产品</term></span>，支持配置所有参数。</p>
<p id="p6822956165313"><a name="p6822956165313"></a><a name="p6822956165313"></a><span id="ph6822456165317"><a name="ph6822456165317"></a><a name="ph6822456165317"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_3"><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_3"><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a>Atlas A3 推理系列产品</term></span>，支持配置所有参数。</p>
<p id="p022812305710"><a name="p022812305710"></a><a name="p022812305710"></a><span id="ph142294312571"><a name="ph142294312571"></a><a name="ph142294312571"></a>Kirin X90</span>，支持配置所有参数。</p>
<p id="p85079382219"><a name="p85079382219"></a><a name="p85079382219"></a><span id="ph116661313221"><a name="ph116661313221"></a><a name="ph116661313221"></a>Kirin 9030</span>，支持配置所有参数。</p>
<a name="ul1670213162021"></a><a name="ul1670213162021"></a><ul id="ul1670213162021"><li><span>仅支持配置迭代次数（repeatTimes）和初始化值（initValue）场景下，其他参数配置无效。每次迭代处理固定数据量（512字节），迭代间无间隔。</span></li><li>支持配置所有参数场景下，支持配置迭代次数（repeatTimes）、初始化值（initValue）、每个迭代处理的数据块个数（blockNum）和迭代间间隔（dstGap）。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 3**  InitConstValueParams结构体参数说明

<a name="table15780447181917"></a>
<table><thead align="left"><tr id="row0780947111915"><th class="cellrowborder" valign="top" width="15.25%" id="mcps1.2.3.1.1"><p id="p1780124771913"><a name="p1780124771913"></a><a name="p1780124771913"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="84.75%" id="mcps1.2.3.1.2"><p id="p1578014718198"><a name="p1578014718198"></a><a name="p1578014718198"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row10780647151919"><td class="cellrowborder" valign="top" width="15.25%" headers="mcps1.2.3.1.1 "><p id="p7662195112120"><a name="p7662195112120"></a><a name="p7662195112120"></a>repeatTimes</p>
</td>
<td class="cellrowborder" valign="top" width="84.75%" headers="mcps1.2.3.1.2 "><p id="p1140916508219"><a name="p1140916508219"></a><a name="p1140916508219"></a>迭代次数。默认值为0。</p>
<a name="ul98581524450"></a><a name="ul98581524450"></a><ul id="ul98581524450"><li><span>仅支持配置迭代次数（repeatTimes）和初始化值（initValue）场景下，repeatTimes∈[0, 255]。</span></li><li>支持配置所有参数场景下，repeatTimes∈[0, 32767] 。</li></ul>
</td>
</tr>
<tr id="row6780947191919"><td class="cellrowborder" valign="top" width="15.25%" headers="mcps1.2.3.1.1 "><p id="p1778310122113"><a name="p1778310122113"></a><a name="p1778310122113"></a>blockNum</p>
</td>
<td class="cellrowborder" valign="top" width="84.75%" headers="mcps1.2.3.1.2 "><p id="p1634012352218"><a name="p1634012352218"></a><a name="p1634012352218"></a>每次迭代初始化的数据块个数，取值范围：blockNum∈[0, 32767] 。默认值为0。</p>
<a name="ul16932118103412"></a><a name="ul16932118103412"></a><ul id="ul16932118103412"><li>dst的位置为A1/B1时，每一个block（数据块）大小是32B；</li><li>dst的位置为A2/B2时，每一个block（数据块）大小是512B。</li></ul>
</td>
</tr>
<tr id="row1078074711194"><td class="cellrowborder" valign="top" width="15.25%" headers="mcps1.2.3.1.1 "><p id="p012671315216"><a name="p012671315216"></a><a name="p012671315216"></a>dstGap</p>
</td>
<td class="cellrowborder" valign="top" width="84.75%" headers="mcps1.2.3.1.2 "><p id="p71261136859"><a name="p71261136859"></a><a name="p71261136859"></a>目的操作数前一个迭代结束地址到后一个迭代起始地址之间的距离。</p>
<a name="ul1196025153512"></a><a name="ul1196025153512"></a><ul id="ul1196025153512"><li>dst的位置为A1/B1时，单位是32B；</li><li>dst的位置为A2/B2时，单位是512B。</li></ul>
<p id="p1734053517219"><a name="p1734053517219"></a><a name="p1734053517219"></a>取值范围：dstGap∈[0, 32767] 。默认值为0。</p>
</td>
</tr>
<tr id="row1761285762117"><td class="cellrowborder" valign="top" width="15.25%" headers="mcps1.2.3.1.1 "><p id="p4579316202119"><a name="p4579316202119"></a><a name="p4579316202119"></a>initValue</p>
</td>
<td class="cellrowborder" valign="top" width="84.75%" headers="mcps1.2.3.1.2 "><p id="p193068319220"><a name="p193068319220"></a><a name="p193068319220"></a>初始化的value值，支持的数据类型与dst保持一致。</p>
</td>
</tr>
</tbody>
</table>

## 约束说明<a name="section633mcpsimp"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。

## 调用示例<a name="section642mcpsimp"></a>

```
#include "kernel_operator.h"

template <typename dst_T, typename fmap_T, typename weight_T, typename dstCO1_T> class KernelCubeMmad {
public:
    __aicore__ inline KernelCubeMmad()
    {
        C0 = 32 / sizeof(fmap_T);
        C1 = channelSize / C0;
        coutBlocks = (Cout + 16 - 1) / 16;
        ho = H - dilationH * (Kh - 1);
        wo = W - dilationW * (Kw - 1);
        howo = ho * wo;
        howoRound = ((howo + 16 - 1) / 16) * 16;
        featureMapA1Size = C1 * H * W * C0;      // shape: [C1, H, W, C0]
        weightA1Size = C1 * Kh * Kw * Cout * C0; // shape: [C1, Kh, Kw, Cout, C0]
        featureMapA2Size = howoRound * (C1 * Kh * Kw * C0);
        weightB2Size = (C1 * Kh * Kw * C0) * coutBlocks * 16;
        m = howo;
        k = C1 * Kh * Kw * C0;
        n = Cout;
        biasSize = Cout;                  // shape: [Cout]
        dstSize = coutBlocks * howo * 16; // shape: [coutBlocks, howo, 16]
        dstCO1Size = coutBlocks * howoRound * 16;
        fmRepeat = featureMapA2Size / (16 * C0);
        weRepeat = weightB2Size / (16 * C0);
    }
    __aicore__ inline void Init(__gm__ uint8_t* fmGm, __gm__ uint8_t* weGm, __gm__ uint8_t* biasGm,
        __gm__ uint8_t* dstGm)
    {
        fmGlobal.SetGlobalBuffer((__gm__ fmap_T*)fmGm);
        weGlobal.SetGlobalBuffer((__gm__ weight_T*)weGm);
        biasGlobal.SetGlobalBuffer((__gm__ dstCO1_T*)biasGm);
        dstGlobal.SetGlobalBuffer((__gm__ dst_T*)dstGm);
        pipe.InitBuffer(inQueueFmA1, 1, featureMapA1Size * sizeof(fmap_T));
        pipe.InitBuffer(inQueueFmA2, 1, featureMapA2Size * sizeof(fmap_T));
        pipe.InitBuffer(inQueueWeB1, 1, weightA1Size * sizeof(weight_T));
        pipe.InitBuffer(inQueueWeB2, 1, weightB2Size * sizeof(weight_T));
        pipe.InitBuffer(inQueueBiasA1, 1, biasSize * sizeof(dstCO1_T));
        pipe.InitBuffer(outQueueCO1, 1, dstCO1Size * sizeof(dstCO1_T));
    }
    __aicore__ inline void Process()
    {
        CopyIn();
        Split();
        Compute();
        CopyOut();
    }

private:
    __aicore__ inline void CopyIn()
    {
        AscendC::LocalTensor<fmap_T> featureMapA1 = inQueueFmA1.AllocTensor<fmap_T>();
        AscendC::LocalTensor<weight_T> weightB1 = inQueueWeB1.AllocTensor<weight_T>();
        AscendC::LocalTensor<dstCO1_T> biasA1 = inQueueBiasA1.AllocTensor<dstCO1_T>();

        AscendC::InitConstValue(featureMapA1, {1, static_cast<uint16_t>(featureMapA1Size * sizeof(fmap_T) / 32), 0, 1});
        AscendC::InitConstValue(weightB1, {1, static_cast<uint16_t>(weightA1Size * sizeof(weight_T) / 32), 0, 2});
        AscendC::DataCopy(biasA1, biasGlobal, { 1, static_cast<uint16_t>(biasSize * sizeof(dstCO1_T) / 32), 0, 0 });

        inQueueFmA1.EnQue(featureMapA1);
        inQueueWeB1.EnQue(weightB1);
        inQueueBiasA1.EnQue(biasA1);
    }
    __aicore__ inline void Split()
    {
        AscendC::LocalTensor<fmap_T> featureMapA1 = inQueueFmA1.DeQue<fmap_T>();
        AscendC::LocalTensor<weight_T> weightB1 = inQueueWeB1.DeQue<weight_T>();
        AscendC::LocalTensor<fmap_T> featureMapA2 = inQueueFmA2.AllocTensor<fmap_T>();
        AscendC::LocalTensor<weight_T> weightB2 = inQueueWeB2.AllocTensor<weight_T>();

        AscendC::InitConstValue(featureMapA2, {1, static_cast<uint16_t>(featureMapA2Size * sizeof(fmap_T) / 512), 0, 1});
        AscendC::InitConstValue(weightB2, { 1, static_cast<uint16_t>(weightB2Size * sizeof(weight_T) / 512), 0, 2});

        inQueueFmA2.EnQue<fmap_T>(featureMapA2);
        inQueueWeB2.EnQue<weight_T>(weightB2);
        inQueueFmA1.FreeTensor(featureMapA1);
        inQueueWeB1.FreeTensor(weightB1);
    }
    __aicore__ inline void Compute()
    {
        AscendC::LocalTensor<fmap_T> featureMapA2 = inQueueFmA2.DeQue<fmap_T>();
        AscendC::LocalTensor<weight_T> weightB2 = inQueueWeB2.DeQue<weight_T>();
        AscendC::LocalTensor<dstCO1_T> dstCO1 = outQueueCO1.AllocTensor<dstCO1_T>();
        AscendC::LocalTensor<dstCO1_T> biasA1 = inQueueBiasA1.DeQue<dstCO1_T>();
        AscendC::Mmad(dstCO1, featureMapA2, weightB2, biasA1, { m, n, k, true, 0, false, false, false });

        outQueueCO1.EnQue<dstCO1_T>(dstCO1);
        inQueueFmA2.FreeTensor(featureMapA2);
        inQueueWeB2.FreeTensor(weightB2);
        inQueueBiasA1.FreeTensor(biasA1);
    }
    __aicore__ inline void CopyOut()
    {
        AscendC::LocalTensor<dstCO1_T> dstCO1 = outQueueCO1.DeQue<dstCO1_T>();
        AscendC::FixpipeParamsV220 fixpipeParams;
        fixpipeParams.nSize = coutBlocks * 16;
        fixpipeParams.mSize = howo;
        fixpipeParams.srcStride = howo;
        fixpipeParams.dstStride = howo * AscendC::BLOCK_CUBE * sizeof(dst_T) / AscendC::ONE_BLK_SIZE;
        fixpipeParams.quantPre = deqMode;
        AscendC::Fixpipe<dst_T, dstCO1_T, AscendC::CFG_NZ>(dstGlobal, dstCO1, fixpipeParams);
        outQueueCO1.FreeTensor(dstCO1);
    }

private:
    AscendC::TPipe pipe;
    // feature map queue
    AscendC::TQue<AscendC::TPosition::A1, 1> inQueueFmA1;
    AscendC::TQue<AscendC::TPosition::A2, 1> inQueueFmA2;
    // weight queue
    AscendC::TQue<AscendC::TPosition::B1, 1> inQueueWeB1;
    AscendC::TQue<AscendC::TPosition::B2, 1> inQueueWeB2;
    // bias queue
    AscendC::TQue<AscendC::TPosition::A1, 1> inQueueBiasA1;
    // dst queue
    AscendC::TQue<AscendC::TPosition::CO1, 1> outQueueCO1;

    AscendC::GlobalTensor<fmap_T> fmGlobal;
    AscendC::GlobalTensor<weight_T> weGlobal;
    AscendC::GlobalTensor<dst_T> dstGlobal;
    AscendC::GlobalTensor<dstCO1_T> biasGlobal;

    uint16_t channelSize = 32;
    uint16_t H = 4, W = 4;
    uint8_t Kh = 2, Kw = 2;
    uint16_t Cout = 16;
    uint16_t C0, C1;
    uint8_t dilationH = 2, dilationW = 2;
    uint16_t coutBlocks, ho, wo, howo, howoRound;
    uint32_t featureMapA1Size, weightA1Size, featureMapA2Size, weightB2Size, biasSize, dstSize, dstCO1Size;
    uint16_t m, k, n;
    uint8_t fmRepeat, weRepeat;
    AscendC::QuantMode_t deqMode = AscendC::QuantMode_t::F322F16;
};

extern "C" __global__ __aicore__ void cube_mmad_simple_kernel(__gm__ uint8_t *fmGm, __gm__ uint8_t *weGm,
    __gm__ uint8_t *biasGm, __gm__ uint8_t *dstGm)
{
    KernelCubeMmad<half, half, half, half> op;
    op.Init(fmGm, weGm, biasGm, dstGm);
    op.Process();
}
```

