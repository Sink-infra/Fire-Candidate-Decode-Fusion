# WholeReduceMax<a name="ZH-CN_TOPIC_0000001953491746"></a>

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
<tr id="row149855319106"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p07746191307"><a name="p07746191307"></a><a name="p07746191307"></a><span id="ph880814542020"><a name="ph880814542020"></a><a name="ph880814542020"></a>Kirin X90</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p18774619608"><a name="p18774619608"></a><a name="p18774619608"></a>√</p>
</td>
</tr>
<tr id="row586610538102"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p4336317115"><a name="p4336317115"></a><a name="p4336317115"></a><span id="ph115418716"><a name="ph115418716"></a><a name="ph115418716"></a>Kirin 9030</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p73313312117"><a name="p73313312117"></a><a name="p73313312117"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

每个repeat内所有数据求最大值以及其索引index，返回的索引值为每个repeat内部索引。

## 函数原型<a name="section620mcpsimp"></a>

-   mask逐bit模式

    ```
    template <typename T, bool isSetMask = true>
    __aicore__ inline void WholeReduceMax(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint64_t mask[], const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

-   mask连续模式

    ```
    template <typename T, bool isSetMask = true>
    __aicore__ inline void WholeReduceMax(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001429830437_row118356578583"><th class="cellrowborder" valign="top" width="13.969999999999999%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001429830437_p48354572582"><a name="zh-cn_topic_0000001429830437_p48354572582"></a><a name="zh-cn_topic_0000001429830437_p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="86.03%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001429830437_p583535795817"><a name="zh-cn_topic_0000001429830437_p583535795817"></a><a name="zh-cn_topic_0000001429830437_p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001429830437_row1835857145817"><td class="cellrowborder" valign="top" width="13.969999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001429830437_p5835457165816"><a name="zh-cn_topic_0000001429830437_p5835457165816"></a><a name="zh-cn_topic_0000001429830437_p5835457165816"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="86.03%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001429830437_p168351657155818"><a name="zh-cn_topic_0000001429830437_p168351657155818"></a><a name="zh-cn_topic_0000001429830437_p168351657155818"></a>操作数数据类型。</p>
<p id="p102651737121310"><a name="p102651737121310"></a><a name="p102651737121310"></a><span id="ph22655375134"><a name="ph22655375134"></a><a name="ph22655375134"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：uint16_t/int16_t/uint32_t/int32_t/half/float</p>
<p id="p11759459315"><a name="p11759459315"></a><a name="p11759459315"></a><span id="ph7759135915116"><a name="ph7759135915116"></a><a name="ph7759135915116"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half/float</p>
<p id="p209372313303"><a name="p209372313303"></a><a name="p209372313303"></a><span id="ph6937237308"><a name="ph6937237308"></a><a name="ph6937237308"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half/float</p>
<p id="p065555314"><a name="p065555314"></a><a name="p065555314"></a><span id="ph15441323310"><a name="ph15441323310"></a><a name="ph15441323310"></a>Kirin X90</span>，支持的数据类型为：half/float</p>
<p id="p082420411739"><a name="p082420411739"></a><a name="p082420411739"></a><span id="ph25359910414"><a name="ph25359910414"></a><a name="ph25359910414"></a>Kirin 9030</span>，支持的数据类型为：half/float</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001429830437_row18835145716587"><td class="cellrowborder" valign="top" width="13.969999999999999%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001429830437_p1383515717581"><a name="zh-cn_topic_0000001429830437_p1383515717581"></a><a name="zh-cn_topic_0000001429830437_p1383515717581"></a>isSetMask</p>
</td>
<td class="cellrowborder" valign="top" width="86.03%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001429830437_p77520541653"><a name="zh-cn_topic_0000001429830437_p77520541653"></a><a name="zh-cn_topic_0000001429830437_p77520541653"></a>是否在接口内部设置mask。</p>
<a name="zh-cn_topic_0000001429830437_ul1163765616511"></a><a name="zh-cn_topic_0000001429830437_ul1163765616511"></a><ul id="zh-cn_topic_0000001429830437_ul1163765616511"><li>true，表示在接口内部设置mask。</li><li>false，表示在接口外部设置mask，开发者需要使用<a href="SetVectorMask.md">SetVectorMask</a>接口设置mask值。这种模式下，本接口入参中的mask值必须设置为占位符MASK_PLACEHOLDER。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="table8955841508"></a>
<table><thead align="left"><tr id="row15956194105014"><th class="cellrowborder" valign="top" width="13.661366136613662%" id="mcps1.2.4.1.1"><p id="p7956144195014"><a name="p7956144195014"></a><a name="p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="9.12091209120912%" id="mcps1.2.4.1.2"><p id="p1295624145013"><a name="p1295624145013"></a><a name="p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="77.21772177217721%" id="mcps1.2.4.1.3"><p id="p16956144145011"><a name="p16956144145011"></a><a name="p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row5956546509"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p4428175618426"><a name="p4428175618426"></a><a name="p4428175618426"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p2428856174212"><a name="p2428856174212"></a><a name="p2428856174212"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="p15798114920385"><a name="p15798114920385"></a><a name="p15798114920385"></a>目的操作数。</p>
<p id="p16703131355116"><a name="p16703131355116"></a><a name="p16703131355116"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p1871313861718"><a name="p1871313861718"></a><a name="p1871313861718"></a>LocalTensor的起始地址需要保证4字节对齐（针对half数据类型），8字节对齐（针对float数据类型）。</p>
</td>
</tr>
<tr id="row4956154125018"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p10429155616425"><a name="p10429155616425"></a><a name="p10429155616425"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p164291756114215"><a name="p164291756114215"></a><a name="p164291756114215"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="p48001610153912"><a name="p48001610153912"></a><a name="p48001610153912"></a>源操作数。</p>
<p id="p434181318395"><a name="p434181318395"></a><a name="p434181318395"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p8419173193912"><a name="p8419173193912"></a><a name="p8419173193912"></a><span id="ph1479701815419"><a name="ph1479701815419"></a><a name="ph1479701815419"></a>LocalTensor的起始地址需要32字节对齐。</span></p>
<p id="p1942985674213"><a name="p1942985674213"></a><a name="p1942985674213"></a>源操作数的数据类型需要与目的操作数保持一致。</p>
</td>
</tr>
<tr id="row1495634115010"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p1728791441620"><a name="p1728791441620"></a><a name="p1728791441620"></a>mask/mask[]</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p159578209413"><a name="p159578209413"></a><a name="p159578209413"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000001530181537_p0554313181312"><a name="zh-cn_topic_0000001530181537_p0554313181312"></a><a name="zh-cn_topic_0000001530181537_p0554313181312"></a><span id="zh-cn_topic_0000001530181537_ph793119540147"><a name="zh-cn_topic_0000001530181537_ph793119540147"></a><a name="zh-cn_topic_0000001530181537_ph793119540147"></a>mask用于控制每次迭代内参与计算的元素。</span></p>
<a name="zh-cn_topic_0000001530181537_ul1255411133132"></a><a name="zh-cn_topic_0000001530181537_ul1255411133132"></a><ul id="zh-cn_topic_0000001530181537_ul1255411133132"><li>逐bit模式：可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。<p id="zh-cn_topic_0000001530181537_p121114581013"><a name="zh-cn_topic_0000001530181537_p121114581013"></a><a name="zh-cn_topic_0000001530181537_p121114581013"></a>mask为数组形式，数组长度和数组元素的取值范围和操作数的数据类型有关。当操作数为16位时，数组长度为2，mask[0]、mask[1]∈[0, 2<sup id="zh-cn_topic_0000001530181537_sup1411059101"><a name="zh-cn_topic_0000001530181537_sup1411059101"></a><a name="zh-cn_topic_0000001530181537_sup1411059101"></a>64</sup>-1]并且不同时为0；当操作数为32位时，数组长度为1，mask[0]∈(0, 2<sup id="zh-cn_topic_0000001530181537_sup1711155161017"><a name="zh-cn_topic_0000001530181537_sup1711155161017"></a><a name="zh-cn_topic_0000001530181537_sup1711155161017"></a>64</sup>-1]；当操作数为64位时，数组长度为1，mask[0]∈(0, 2<sup id="zh-cn_topic_0000001530181537_sup181195111019"><a name="zh-cn_topic_0000001530181537_sup181195111019"></a><a name="zh-cn_topic_0000001530181537_sup181195111019"></a>32</sup>-1]。</p>
<p id="zh-cn_topic_0000001530181537_p711354105"><a name="zh-cn_topic_0000001530181537_p711354105"></a><a name="zh-cn_topic_0000001530181537_p711354105"></a>例如，mask=[8, 0]，8=0b1000，表示仅第4个元素参与计算。</p>
</li></ul>
<a name="zh-cn_topic_0000001530181537_ul18554121313135"></a><a name="zh-cn_topic_0000001530181537_ul18554121313135"></a><ul id="zh-cn_topic_0000001530181537_ul18554121313135"><li>连续模式：表示前面连续的多少个元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask∈[1, 128]；当操作数为32位时，mask∈[1, 64]；当操作数为64位时，mask∈[1, 32]。</li></ul>
</td>
</tr>
<tr id="row103306116356"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p1756245258"><a name="p1756245258"></a><a name="p1756245258"></a>repeatTime</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p127561346255"><a name="p127561346255"></a><a name="p127561346255"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="p353564621520"><a name="p353564621520"></a><a name="p353564621520"></a>迭代次数。取值范围为[0, 255]。</p>
</td>
</tr>
<tr id="row6301859135119"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p229173384114"><a name="p229173384114"></a><a name="p229173384114"></a>dstRepStride</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p32933310418"><a name="p32933310418"></a><a name="p32933310418"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="p102993315413"><a name="p102993315413"></a><a name="p102993315413"></a>目的操作数相邻迭代间的地址步长。以一个repeat归约后的长度为单位。</p>
<p id="p1579221016158"><a name="p1579221016158"></a><a name="p1579221016158"></a>返回索引和最值时，单位为dst数据类型所占字节长度的两倍。比如当dst为half时，单位为4Bytes；</p>
<p id="p887145051513"><a name="p887145051513"></a><a name="p887145051513"></a>仅返回最值时，单位为dst数据类型所占字节长度；</p>
<p id="p33687399318"><a name="p33687399318"></a><a name="p33687399318"></a>仅返回索引时，单位为uint32_t类型所占字节长度。</p>
</td>
</tr>
<tr id="row0863135810539"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p15269101625610"><a name="p15269101625610"></a><a name="p15269101625610"></a>srcBlkStride</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p195761631163416"><a name="p195761631163416"></a><a name="p195761631163416"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="p14215346174119"><a name="p14215346174119"></a><a name="p14215346174119"></a>单次迭代内datablock的地址步长。</p>
</td>
</tr>
<tr id="row5250192917342"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p197977615560"><a name="p197977615560"></a><a name="p197977615560"></a>srcRepStride</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p1479756155613"><a name="p1479756155613"></a><a name="p1479756155613"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="p1624214011488"><a name="p1624214011488"></a><a name="p1624214011488"></a>源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的datablock数目。</p>
</td>
</tr>
<tr id="row350794117359"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p158803470265"><a name="p158803470265"></a><a name="p158803470265"></a>order</p>
</td>
<td class="cellrowborder" valign="top" width="9.12091209120912%" headers="mcps1.2.4.1.2 "><p id="p7880174717266"><a name="p7880174717266"></a><a name="p7880174717266"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="77.21772177217721%" headers="mcps1.2.4.1.3 "><p id="p1594215814570"><a name="p1594215814570"></a><a name="p1594215814570"></a>使用order参数指定dst中index与value的相对位置以及返回结果行为，ReduceOrder类型，默认值为ORDER_VALUE_INDEX。取值范围如下：</p>
<a name="ul19429814578"></a><a name="ul19429814578"></a><ul id="ul19429814578"><li>ORDER_VALUE_INDEX：表示value位于低半部，返回结果存储顺序为[value, index]。</li><li>ORDER_INDEX_VALUE：表示index位于低半部，返回结果存储顺序为[index, value]。</li><li>ORDER_ONLY_VALUE：表示只返回最值，返回结果存储顺序为[value]。</li><li>ORDER_ONLY_INDEX：表示只返回最值索引，返回结果存储顺序为[index]。</li></ul>
<p id="p8407122245016"><a name="p8407122245016"></a><a name="p8407122245016"></a><span id="ph647452053119"><a name="ph647452053119"></a><a name="ph647452053119"></a>Ascend 950PR/Ascend 950DT</span>，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE、ORDER_ONLY_VALUE、ORDER_ONLY_INDEX。</p>
<p id="p134246188318"><a name="p134246188318"></a><a name="p134246188318"></a><span id="ph191185131318"><a name="ph191185131318"></a><a name="ph191185131318"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE、ORDER_ONLY_VALUE、ORDER_ONLY_INDEX。</p>
<p id="p141148133319"><a name="p141148133319"></a><a name="p141148133319"></a><span id="ph1513611133112"><a name="ph1513611133112"></a><a name="ph1513611133112"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE、ORDER_ONLY_VALUE、ORDER_ONLY_INDEX。</p>
<p id="p993718942312"><a name="p993718942312"></a><a name="p993718942312"></a><span id="ph17937992232"><a name="ph17937992232"></a><a name="ph17937992232"></a>Kirin X90</span>，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE。</p>
<p id="p189373913239"><a name="p189373913239"></a><a name="p189373913239"></a><span id="ph1993716918235"><a name="ph1993716918235"></a><a name="ph1993716918235"></a>Kirin 9030</span>，支持ORDER_VALUE_INDEX、ORDER_INDEX_VALUE。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section5468191312484"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。
-   操作数地址重叠约束请参考[通用地址重叠约束](通用说明和约束.md#section668772811100)。

-   dst结果存储顺序由order决定，默认为最值、最值索引。返回结果中索引index数据按照dst的数据类型进行存储，比如dst使用half类型时，index按照half类型进行存储，读取时需要使用reinterpret\_cast方法转换到整数类型。若输入数据类型是half，需要使用reinterpret\_cast<uint16\_t\*\>，若输入是float，需要使用reinterpret\_cast<uint32\_t\*\>。比如[完整样例](#li16916756380)中，前两个计算结果为\[9.980e-01 5.364e-06\]，5.364e-06需要使用reinterpret\_cast方法转换得到索引值90。特别地，针对Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品，ORDER\_ONLY\_INDEX（仅返回最值索引）情况下，读取index时都需要使用reinterpret\_cast<uint32\_t\*\>。针对Ascend 950PR/Ascend 950DT，ORDER\_ONLY\_INDEX（仅返回最值索引）情况下，当操作数数据类型为uint16\_t/int16\_t/half时，读取index都需要使用reinterpret\_cast<uint32\_t\*\>。
-   针对不同场景合理使用归约指令可以带来性能提升，具体样例请参考[ReduceCustom](https://gitee.com/ascend/samples/tree/master/operator/ascendc/0_introduction/14_reduce_frameworklaunch/ReduceCustom)。

## 调用示例<a name="section642mcpsimp"></a>

-   tensor高维切分计算样例-mask连续模式

    ```
    // dstLocal,srcLocal均为half类型，srcLocal的计算数据量为512，连续排布，计算结果也需要连续排布，使用tensor高维切分计算接口，设定mask为最多的128个全部元素参与计算
    // 根据以上信息，推断出repeatTime为4，dstRepStride为1，srcBlkStride为1，srcRepStride为8
    // 若求最大值及索引，并且需要存储顺序为[value, index]的结果，可以使用默认order，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, 128, 4, 1, 1, 8);
    // 若求最大值及索引，并且需要存储顺序为[index, value]的结果，接口示例为：
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, 128, 4, 1, 1, 8, AscendC::ReduceOrder::ORDER_INDEX_VALUE);
    ```

-   tensor高维切分计算样例-mask逐bit模式

    ```
    // dstLocal,srcLocal均为half类型，srcLocal的计算数据量为512，连续排布，计算结果也需要连续排布，使用tensor高维切分计算接口，设定mask为最多的128个全部元素参与计算
    // 根据以上信息，推断出repeatTime为4，dstRepStride为1，srcBlkStride为1，srcRepStride为8
    // 若求最大值及索引，并且需要存储顺序为[value, index]的结果，使用默认order，接口示例为：
    uint64_t mask[2] = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };
    AscendC::WholeReduceMax<half>(dstLocal, srcLocal, mask, 4, 1, 1, 8);
    ```

-   <a name="li16916756380"></a>完整样例：

    ```
    #include "kernel_operator.h"
    class KernelReduce {
    public:
        __aicore__ inline KernelReduce() {}
        __aicore__ inline void Init(__gm__ uint8_t* src, __gm__ uint8_t* dstGm)
        {
            srcGlobal.SetGlobalBuffer((__gm__ half*)src);
            dstGlobal.SetGlobalBuffer((__gm__ half*)dstGm);
            repeat = srcDataSize / mask;
            pipe.InitBuffer(inQueueSrc, 1, srcDataSize * sizeof(half));
            pipe.InitBuffer(outQueueDst, 1, dstDataSize * sizeof(half));
        }
        __aicore__ inline void Process()
        {
            CopyIn();
            Compute();
            CopyOut();
        }
    private:
        __aicore__ inline void CopyIn()
        {
            AscendC::LocalTensor<half> srcLocal = inQueueSrc.AllocTensor<half>();
            AscendC::DataCopy(srcLocal, srcGlobal, srcDataSize);
            inQueueSrc.EnQue(srcLocal);
        }
        __aicore__ inline void Compute()
        {
            AscendC::LocalTensor<half> srcLocal = inQueueSrc.DeQue<half>();
            AscendC::LocalTensor<half> dstLocal = outQueueDst.AllocTensor<half>();
            AscendC::WholeReduceMax<half>(dstLocal, srcLocal, mask, repeat, 1, 1, 8); // 使用默认order, ReduceOrder::ORDER_VALUE_INDEX
            outQueueDst.EnQue<half>(dstLocal);
            inQueueSrc.FreeTensor(srcLocal);
        }
        __aicore__ inline void CopyOut()
        {
            AscendC::LocalTensor<half> dstLocal = outQueueDst.DeQue<half>();
            AscendC::DataCopy(dstGlobal, dstLocal, dstDataSize);
            outQueueDst.FreeTensor(dstLocal);
        }
    private:
        AscendC::TPipe pipe;
        AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueSrc;
        AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueDst;
        AscendC::GlobalTensor<half> srcGlobal, dstGlobal;
        int srcDataSize = 1024;
        int dstDataSize = 16;
        int mask = 128;
        int repeat = 0;
    };
    extern "C" __global__ __aicore__ void reduce_kernel(__gm__ uint8_t* src, __gm__ uint8_t* dstGm)
    {
        KernelReduce op;
        op.Init(src, dstGm);
        op.Process();
    }
    ```

    示例结果如下：

    ```
    输入数据src_gm：
    [0.00787  0.8516   0.01558  0.152    0.887    0.2532   0.2272   0.1295
     0.7207   0.628    0.5522   0.991    0.3164   0.961    0.526    0.5513
     0.03973  0.3293   0.809    0.562    0.915    0.56     0.3464   0.3438
     0.6094   0.1201   0.8384   0.848    0.004436 0.4263   0.01917  0.753
     0.9126   0.2307   0.1066   0.644    0.8657   0.7085   0.7915   0.1707
     0.3806   0.957    0.0483   0.858    0.10675  0.21     0.03345  0.55
     0.3757   0.3281   0.927    0.09406  0.6445   0.985    0.405    0.09393
     0.773    0.7227   0.03714  0.595    0.889    0.0948   0.4202   0.2747
     0.5894   0.3022   0.894    0.675    0.6016   0.938    0.585    0.5244
     0.8643   0.888    0.794    0.636    0.976    0.148    0.7427   0.1742
     0.32     0.0649   0.2954   0.2018   0.833    0.0976   0.4048   0.2861
     0.8765   0.722    0.998    0.03041  0.005512 0.9087   0.9873   0.1436
     0.4812   0.1901   0.78     0.6934   0.2317   0.3782   0.8613   0.808
     0.06885  0.3584   0.5684   0.541    0.5415   0.3096   0.5957   0.9043
     0.7964   0.501    0.4324   0.7544   0.687    0.8447   0.526    0.548
     0.926    0.9106   0.1616   0.183    0.6704   0.642    0.4783   0.1797
     0.2078   0.59     0.4866   0.4683   0.649    0.7266   0.4976   0.8364
     0.6245   0.07385  0.0786   0.586    0.7827   0.3298   0.9497   0.1617
     0.4375   0.3572   0.2896   0.6465   0.1156   0.4905   0.2617   0.8267
     0.2054   0.1415   0.2993   0.8374   0.754    0.942    0.6416   0.1222
     0.1465   0.3335   0.3577   0.6484   0.614    0.5825   0.6807   0.9297
     0.694    0.759    0.908    0.9126   0.4731   0.963    0.3271   0.724
     0.4077   0.335    0.672    0.4219   0.1818   0.843    0.2708   0.0816
     0.457    0.3481   0.67     0.6895   0.6924   0.191    0.2013   0.2484
     0.8833   0.9146   0.4102   0.1063   0.6685   0.804    0.6606   0.2491
     0.34     0.3281   0.823    0.603    0.521    0.6797   0.401    0.5
     0.03683  0.04758  0.507    0.667    0.9014   0.263    0.2477   0.0179
     0.8735   0.007023 0.545    0.758    0.3508   0.6333   0.9375   0.5903
     0.2732   0.0847   0.489    0.196    0.5557   0.403    0.9204   0.3655
     0.5083   0.7515   0.3347   0.6914   0.2185   0.2458   0.5537   0.3457
     0.4878   0.869    0.908    0.0877   0.295    0.9      0.9307   0.05545
     0.4639   0.4001   0.8433   0.4883   0.916    0.7026   0.5063   0.05164
     0.936    0.844    0.2086   0.625    0.0197   0.4312   0.3677   0.983
     0.625    0.004665 0.2479   0.3093   0.9214   0.003672 0.7915   0.921
     0.331    0.01127  0.703    0.6416   0.4053   0.53     0.9688   0.10297
     0.5547   0.07367  0.2305   0.02821  0.8115   0.4202   0.0561   0.0917
     0.04828  0.536    0.0905   0.328    0.8413   0.3696   0.982    0.3733
     0.436    0.753    0.1937   0.8706   0.991    0.273    0.763    0.418
     0.4446   0.513    0.6724   0.1179   0.921    0.756    0.7144   0.6196
     0.9634   0.562    0.3088   0.864    0.709    0.6797   0.2114   0.534
     0.5225   0.1852   0.038    0.5454   0.8823   0.849    0.608    0.7734
     0.7446   0.7236   0.1903   0.1031   0.497    0.57     0.172    0.1907
     0.6333   0.641    0.681    0.2323   0.1007   0.4094   0.3655   0.4248
     0.08044  0.1483   0.08716  0.354    0.128    0.3933   0.775    0.215
     0.728    0.909    0.4204   0.618    0.2517   0.9106   0.3647   0.5977
     0.3445   0.315    0.488    0.99     0.9443   0.6196   0.9287   0.088
     0.9946   0.796    0.7515   0.1912   0.4312   0.7974   0.735    0.01536
     0.7456   0.643    0.484    0.218    0.9272   0.1703   0.1885   0.1982
     0.754    0.902    0.848    0.05832  0.4138   0.6885   0.3853   0.3499
     0.639    0.5786   0.6353   0.5664   0.02621  0.56     0.532    0.08246
     0.733    0.1334   0.0728   0.7817   0.5273   0.126    0.179    0.7334
     0.1565   0.457    0.4807   0.6987   0.5845   0.6206   0.902    0.9277
     0.501    0.6763   0.3418   0.7925   0.07556  0.0929   0.9014   0.3145
     0.04907  0.7188   0.958    0.7275   0.1963   0.1742   0.785    0.518
     0.61     0.1112   0.481    0.10583  0.198    0.181    0.3271   0.2773
     0.2391   0.5625   0.621    0.173    0.05936  0.5654   0.838    0.865
     0.01523  0.6724   0.546    0.737    0.778    0.8613   0.7085   0.8213
     0.08826  0.818    0.4866   0.159    0.4143   0.1007   0.7773   0.487
     0.5225   0.8984   0.4907   0.525    0.4075   0.2632   0.2292   0.134
     0.4622   0.65     0.294    0.607    0.2725   0.2603   0.9326   0.787
     0.9478   0.941    0.3066   0.2944   0.3928   0.73     0.1797   0.2157
     0.609    0.4216   0.8984   0.8477   0.863    0.2478   0.993    0.6274
     0.724    0.03668  0.0991   0.5825   0.662    0.6904   0.7017   0.2379
     0.514    0.1646   0.3245   0.03072  0.3232   0.907    0.9966   0.6396
     0.2969   0.02539  0.66     0.764    0.7803   0.515    0.04074  0.2258
     0.08887  0.1782   0.875    0.1517   0.2351   0.3848   0.5933   0.6875
     0.1969   0.1283   0.06232  0.4348   0.168    0.6904   0.5464   0.12036
     0.885    0.007717 0.5967   0.2856   0.628    0.62     0.854    0.4297
     0.733    0.2274   0.9736   0.01622  0.456    0.4763   0.9707   0.874
     0.8794   0.511    0.1628   0.03458  0.506    0.1464   0.3674   0.1532
     0.786    0.3809   0.406    0.015434 0.901    0.951    0.3018   0.3584
     0.5337   0.4983   0.85     0.833    0.7324   0.492    0.39     0.09845
     0.8965   0.862    0.4033   0.181    0.2203   0.3738   0.2761   0.9653
     0.3577   0.289    0.3167   0.91     0.2688   0.3972   0.585    0.2178
     0.307    0.4966   0.513    0.5225   0.786    0.1888   0.9287   0.5093
     0.1193   0.3987   0.799    0.9995   0.611    0.9897   0.7515   0.4478
     0.3232   0.2426   0.3323   0.7134   0.77     0.7275   0.02043  0.3132
     0.3555   0.03122  0.8623   0.4705   0.6357   0.3157   0.5063   0.1711
     0.885    0.7554   0.815    0.0213   0.4346   0.049    0.905    0.525
     0.921    0.02411  0.771    0.7227   0.1786   0.278    0.03387  0.7744
     0.05875  0.8955   0.8374   0.715    0.3765   0.02075  0.675    0.9883
     0.63     0.7017   0.299    0.92     0.1644   0.3977   0.487    0.818
     0.636    0.3452   0.6406   0.783    0.3728   0.1619   0.7725   0.4673
     0.297    0.9375   0.083    0.0914   0.6704   0.08923  0.332    0.0973
     0.507    0.201    0.1658   0.2358   0.8706   0.6846   0.6396   0.289
     0.831    0.669    0.4683   0.2568   0.219    0.616    0.978    0.1564
     0.925    0.4265   0.6055   0.7246   0.235    0.5376   0.03668  0.2441
     0.7935   0.383    0.2996   0.3523   0.2544   0.6006   0.8896   0.757
     0.7134   0.3196   0.3657   0.249    0.2429   0.921    0.877    0.728
     0.8853   0.1635   0.546    0.9243   0.676    0.4749   0.3928   0.4187
     0.612    0.3953   0.2372   0.4092   0.1523   0.1599   0.03108  0.1602
     0.2474   0.3572   0.0643   0.9434   0.52     0.8574   0.959    0.7593
     0.2318   0.5444   0.2222   0.3884   0.8066   0.4573   0.664    0.335
     0.02025  0.1519   0.01386  0.989    0.852    0.695    0.01289  0.3433
     0.2148   0.9404   0.6753   0.704    0.11163  0.675    0.5264   0.1514
     0.5273   0.9785   0.2769   0.4846   0.2747   0.558    0.742    0.681
     0.835    0.9546   0.941    0.588    0.785    0.2095   0.07294  0.4343
     0.086    0.5825   0.513    0.6313   0.04236  0.4072   0.558    0.681
     0.4805   0.492    0.625    0.7744   0.002626 0.662    0.9043   0.4766
     0.6597   0.6934   0.3394   0.05453  0.9146   0.2222   0.7925   0.605
     0.812    0.671    0.4329   0.2118   0.363    0.1444   0.0955   0.692
     0.675    0.3      0.6846   0.535    0.9834   0.929    0.3582   0.964
     0.3835   0.1466   0.801    0.954    0.2554   0.01357  0.6636   0.8325
     0.6494   0.817    0.2268   0.00904  0.0487   0.08716  0.6753   0.3833
     0.663    0.396    0.6685   0.983    0.0728   0.694    0.02364  0.137
     0.1727   0.231    0.7896   0.8057   0.478    0.883    0.1785   0.5938
     0.11456  0.6997   0.1945   0.02365  0.7236   0.8623   0.2178   0.1295
     0.3867   0.7188   0.11475  0.6      0.419    0.2673   0.4404   0.0107
     0.4304   0.1364   0.3708   0.1158   0.1714   0.3123   0.3403   0.7163
     0.079    0.6245   0.719    0.558    0.4526   0.09924  0.512    0.2452
     0.519    0.999    0.7207   0.5605   0.7217   0.653    0.1164   0.789
     0.4724   0.2727   0.10315  0.9644   0.7573   0.06464  0.858    0.7847
     0.958    0.618    0.9536   0.46     0.9766   0.4263   0.4363   0.4434
     0.95     0.3032   0.4338   0.809    0.1642   0.0561   0.2668   0.1853
     0.356    0.934    0.968    0.327    0.913    0.434    0.6616   0.00502
     0.05066  0.5327   0.276    0.5176   0.0674   0.6143   0.8345   0.2976
     0.315    0.6646   0.527    0.791    0.0299   0.4558   0.8354   0.3115
     0.3735   0.3582   0.742    0.2637   0.8877   0.7603   0.4568   0.2045
     0.4746   0.392    0.65     0.391    0.972    0.6973   0.2297   0.568
     0.49     0.1895   0.547    0.79     0.747    0.5205   0.313    0.3809
     0.7817   0.32     0.1012   0.339    0.716    0.8955   0.8564   0.126
     0.6597   0.228    0.1194   0.4775   0.173    0.0265   0.7456   0.859
     0.4841   0.595    0.4553   0.1351   0.2246   0.3564   0.1832   0.8535
     0.703    0.2423   0.04187  0.145    0.997    0.1919   0.571    0.8555
     0.1578   0.2688   0.405    0.3909   0.1428   0.863    0.7295   0.3267
     0.1294   0.5986   0.677    0.7065   0.8853   0.923    0.9385   0.935
     0.1747   0.32     0.2292   0.2676   0.1161   0.4666   0.3826   0.2588
     0.1863   0.7993   0.3984   0.2961   0.2952   0.3247   0.923    0.05746 ]
    输出数据dst_gm：
    [9.980e-01 5.364e-06 9.629e-01 2.682e-06 9.946e-01 6.676e-06 9.966e-01
     7.510e-06 9.995e-01 5.424e-06 9.888e-01 6.378e-06 9.990e-01 6.735e-06
     9.971e-01 5.484e-06]
    ```

