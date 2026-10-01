# Transpose<a name="ZH-CN_TOPIC_0000002473885166"></a>

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

## 功能说明<a name="section279215775714"></a>

对输入数据进行数据排布及Reshape操作，具体功能如下：

【场景1：NZ2ND，1、2轴互换】

输入Tensor \{ shape:\[B, N, H/N/16, S/16, 16, 16\], origin\_shape:\[B, N, S, H/N\], format:"NZ", origin\_format:"ND"\}

输出Tensor \{ shape:\[B, S, N, H/N\], origin\_shape:\[B, S, N, H/N\], format:"ND", origin\_format:"ND"\}

**图 1**  场景1数据排布变换<a name="fig108312301510"></a>  
![](figures/场景1数据排布变换.png "场景1数据排布变换")

【场景2：NZ2NZ，1、2轴互换】

输入Tensor \{ shape:\[B, N, H/N/16, S/16, 16, 16\], origin\_shape:\[B, N, S, H/N\], format:"NZ", origin\_format:"ND"\}

输出Tensor \{ shape:\[B, S, H/N/16, N/16, 16, 16\], origin\_shape:\[B, S, N, H/N\], format:"NZ", origin\_format:"ND"\}

**图 2**  场景2数据排布变换<a name="fig235913115156"></a>  
![](figures/场景2数据排布变换.png "场景2数据排布变换")

【场景3：NZ2NZ，尾轴切分】

输入Tensor \{ shape:\[B, H / 16, S / 16, 16, 16\], origin\_shape:\[B, S, H\], format:"NZ", origin\_format:"ND"\}

输出Tensor \{ shape:\[B, N, H/N/16, S / 16, 16, 16\], origin\_shape:\[B, N, S, H/N\], format:"NZ", origin\_format:"ND"\}

**图 3**  场景3数据排布变换<a name="fig1677921913157"></a>  
![](figures/场景3数据排布变换.png "场景3数据排布变换")

【场景4：NZ2ND，尾轴切分】

输入Tensor \{ shape:\[B, H / 16, S / 16, 16, 16\], origin\_shape:\[B, S, H\], format:"NZ", origin\_format:"ND"\}

输出Tensor \{ shape:\[B, N, S, H/N\], origin\_shape:\[B, N, S, H/N\], format:"ND", origin\_format:"ND"\}

**图 4**  场景4数据排布变换<a name="fig29891726191517"></a>  
![](figures/场景4数据排布变换.png "场景4数据排布变换")

【场景5：NZ2ND，尾轴合并】

输入Tensor \{ shape:\[B, N, H/N/16, S/16, 16, 16\], origin\_shape:\[B, N, S, H/N\], format:"NZ", origin\_format:"ND"\}

输出Tensor \{ shape:\[B, S, H\], origin\_shape:\[B, S, H\], format:"ND", origin\_format:"ND"\}

**图 5**  场景5数据排布变换<a name="fig3495163311159"></a>  
![](figures/场景5数据排布变换.png "场景5数据排布变换")

【场景6：NZ2NZ，尾轴合并】

输入Tensor \{ shape:\[B, N, H/N/16, S/16, 16, 16\], origin\_shape:\[B, N, S, H/N\], format:"NZ", origin\_format:"ND"\}

输出Tensor \{ shape:\[B, H/16, S/16, 16, 16\], origin\_shape:\[B, S, H\], format:"NZ", origin\_format:"ND"\}

**图 6**  场景6数据排布变换<a name="fig4917340141512"></a>  
![](figures/场景6数据排布变换.png "场景6数据排布变换")

【场景7：二维转置】

支持在UB上对二维Tensor进行转置，其中srcShape中的H、W均是16的整倍。

**图 7**  场景7数据排布变换<a name="fig13513134791511"></a>  
![](figures/场景7数据排布变换.png "场景7数据排布变换")

## 实现原理<a name="section13229175017585"></a>

对应Transpose的7种功能场景，每种功能场景的算法框图如图所示。

**图 8**  场景1：NZ2ND，1、2轴互换<a name="fig11375014172116"></a>  
![](figures/场景1-NZ2ND-1-2轴互换.png "场景1-NZ2ND-1-2轴互换")

计算过程分为如下几步：

先后沿H/N方向，N方向，B方向循环处理：

1.  第1次TransDataTo5HD步骤：沿S方向转置S/16个连续的16\*16的方形到temp中，在temp中每个方形与方形之间连续存储；
2.  第2次TransDataTo5HD步骤：将temp中S/16个16\*16的方形转置到dst中，在dst中是ND格式，来自同一个方形的连续2行数据在目的操作数上的地址偏移\(H/N\)\*N个元素，沿H方向的每2个方形的同一行数据在目的操作数上的地址偏移16个元素。

**图 9**  场景2：NZ2NZ，1、2轴互换<a name="fig161551928122110"></a>  
![](figures/场景2-NZ2NZ-1-2轴互换.png "场景2-NZ2NZ-1-2轴互换")

计算过程分为如下几步：

先后沿H/N方向，N方向，B方向循环处理：

1.  第1次TransDataTo5HD步骤：沿S方向分别取S/16个连续的16\*16的方形到temp中，在temp中每个方形与方形之间连续存储；
2.  第2次TransDataTo5HD步骤：将temp中S/16个16\*16的方形转置到dst中，在dst中是NZ格式，来自同一个方形的连续2行数据在目的操作数上的地址偏移\(H/N\)\*N个元素，沿H方向的每2个方形的同一行数据在目的操作数上的地址偏移N\*16个元素。

**图 10**  场景3：NZ2NZ，尾轴切分<a name="fig1212114436213"></a>  
![](figures/场景3-NZ2NZ-尾轴切分.png "场景3-NZ2NZ-尾轴切分")

计算过程分为如下几步：

先后沿H方向，B方向循环处理：

1.  第1次TransDataTo5HD步骤：每次转置S/16个连续的16\*16的方形到temp1中；
2.  DataCopy步骤：当H/N<=16时，每次搬运H/N\*S个元素到temp2中；当H/N\>16时，前H/N/16次搬运16\*S个元素到temp2中，最后一次搬运H/N%16\*S个元素到temp2中；
3.  第2次TransDataTo5HD步骤：将temp2中的16\*S的方形转置到dst中，在dst中是NZ格式，来自同一个方形的连续2行数据在目的操作数上的地址偏移16个元素，沿H方向的每2个方形的同一行数据在目的操作数上的地址偏移S\*16个元素。

**图 11**  场景4：NZ2ND，尾轴切分<a name="fig42511571217"></a>  
![](figures/场景4-NZ2ND-尾轴切分.png "场景4-NZ2ND-尾轴切分")

计算过程分为如下几步：

先后沿H方向，B方向循环处理：

1.  第1次TransDataTo5HD步骤：每次转置S/16个连续的16\*16的方形到temp1中；
2.  DataCopy步骤：当H/N<=16时，每次搬运H/N\*S个元素到temp2中；当H/N\>16时，前H/N/16次搬运16\*S个元素到temp2中，最后一次搬运H/N%16\*S个元素到tmp2中；
3.  第2次TransDataTo5HD步骤：将temp2中的数据转置到dst中，在dst中是ND格式，来自同一个方形的连续2行数据在目的操作数上的地址偏移\(H/N+16-1\)/16\*16个元素，沿H方向的每2个方形的同一行数据在目的操作数上的地址偏移\(H/N+16-1\)/16\*16\*S个元素。

**图 12**  场景5：NZ2ND，尾轴合并<a name="fig594615918226"></a>  
![](figures/场景5-NZ2ND-尾轴合并.png "场景5-NZ2ND-尾轴合并")

计算过程分为如下几步：

先后沿H方向，B方向循环处理：

1.  第1次TransDataTo5HD步骤：每次转置一个S\*16的方形到temp1中；
2.  DataCopy步骤：当H/N<=16时，每次搬运H/N\*S个元素到temp2中；当H/N\>16时，前H/N/16次搬运16\*S个元素到temp2中，最后一次搬运H/N%16\*S个元素到tmp2中；
3.  第2次TransDataTo5HD步骤：将temp2中的16\*S的方形转置到dst中，在dst中是ND格式，来自同一个方形的连续2行数据在目的操作数上的地址偏移\(H+16-1\)/16\*16个元素，沿H方向的每2个方形的同一行数据在目的操作数上的地址偏移H/N\*S个元素。

**图 13**  场景6：NZ2NZ，尾轴合并<a name="fig20321112722210"></a>  
![](figures/场景6-NZ2NZ-尾轴合并.png "场景6-NZ2NZ-尾轴合并")

计算过程分为如下几步：

先后沿H方向，B方向循环处理：

1.  第1次TransDataTo5HD步骤：每次转置一个S\*16的方形到temp1中；
2.  DataCopy步骤：当H/N<=16时，每次搬运H/N\*S个元素到temp2中；当H/N\>16时，前H/N/16次搬运16\*S个元素到temp2中，最后一次搬运H/N%16\*S个元素到tmp2中；
3.  第2次TransDataTo5HD步骤：将temp2中的16\*S的方形转置到dst中，在dst中是NZ格式，来自同一个方形的连续2行数据在目的操作数上的地址偏移16个元素，沿H方向的每2个方形的同一行数据在目的操作数上的地址偏移S\*16个元素。

**图 14**  场景7：二维转置<a name="fig1685617381223"></a>  
![](figures/场景7-二维转置.png "场景7-二维转置")

计算过程如下：

1.  调用TransDataTo5HD，通过设置不同的源操作数地址序列和目的操作数地址序列，将\[H, W\]转置为\[W, H\]，src和dst均是ND格式。

## 函数原型<a name="section620mcpsimp"></a>

由于该接口的内部实现中涉及复杂的计算，需要额外的临时空间来存储计算过程中的中间变量。临时空间大小BufferSize的获取方法：通过[Transpose Tiling](Transpose-Tiling.md)中提供的**GetTransposeMaxMinTmpSize**接口获取所需最大和最小临时空间大小，最小空间可以保证功能正确，最大空间用于提升性能。

临时空间支持**接口框架申请**和开发者**通过sharedTmpBuffer入参传入**两种方式，因此Transpose接口的函数原型有两种：

-   通过sharedTmpBuffer入参传入临时空间

    ```
    template <typename T>
    __aicore__ inline void Transpose(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint8_t> &sharedTmpBuffer, TransposeType transposeType, ConfusionTransposeTiling& tiling)
    ```

    该方式下开发者需自行申请并管理临时内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。

-   接口框架申请临时空间

    ```
    template <typename T>
    __aicore__ inline void Transpose(const LocalTensor<T>& dst, const LocalTensor<T>& src, TransposeType transposeType, ConfusionTransposeTiling& tiling)
    ```

    该方式下开发者无需申请，但是需要预留临时空间的大小。

## 参数说明<a name="section8434813191918"></a>

**表 1**  模板参数说明

<a name="table729818506422"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001692058420_row11299950204217"><th class="cellrowborder" valign="top" width="19.18%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001692058420_p1029955044218"><a name="zh-cn_topic_0000001692058420_p1029955044218"></a><a name="zh-cn_topic_0000001692058420_p1029955044218"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.82000000000001%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001692058420_p1629911506421"><a name="zh-cn_topic_0000001692058420_p1629911506421"></a><a name="zh-cn_topic_0000001692058420_p1629911506421"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001692058420_row12299165018421"><td class="cellrowborder" valign="top" width="19.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001692058420_p1329915004219"><a name="zh-cn_topic_0000001692058420_p1329915004219"></a><a name="zh-cn_topic_0000001692058420_p1329915004219"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="80.82000000000001%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001692058420_p8299155010420"><a name="zh-cn_topic_0000001692058420_p8299155010420"></a><a name="zh-cn_topic_0000001692058420_p8299155010420"></a>操作数的数据类型。</p>
<p id="p871914594558"><a name="p871914594558"></a><a name="p871914594558"></a><span id="ph137191759185511"><a name="ph137191759185511"></a><a name="ph137191759185511"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：int16_t、uint16_t、half、int32_t、uint32_t、float。</p>
<p id="p142889819517"><a name="p142889819517"></a><a name="p142889819517"></a><span id="ph747465446"><a name="ph747465446"></a><a name="ph747465446"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：int16_t、uint16_t、half、int32_t、uint32_t、float。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="17.77%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="8.07%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="74.16%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row42461942101815"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p462911347151"><a name="p462911347151"></a><a name="p462911347151"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="8.07%" headers="mcps1.2.4.1.2 "><p id="p186293346150"><a name="p186293346150"></a><a name="p186293346150"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="74.16%" headers="mcps1.2.4.1.3 "><p id="p1762913411151"><a name="p1762913411151"></a><a name="p1762913411151"></a>目的操作数，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。</p>
<p id="p11763733516"><a name="p11763733516"></a><a name="p11763733516"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row250163914199"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p1762920347151"><a name="p1762920347151"></a><a name="p1762920347151"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="8.07%" headers="mcps1.2.4.1.2 "><p id="p1662903414157"><a name="p1662903414157"></a><a name="p1662903414157"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="74.16%" headers="mcps1.2.4.1.3 "><p id="p1663083415154"><a name="p1663083415154"></a><a name="p1663083415154"></a>源操作数，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。</p>
<p id="p95501751455"><a name="p95501751455"></a><a name="p95501751455"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1315719181049"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p215910345451"><a name="p215910345451"></a><a name="p215910345451"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="8.07%" headers="mcps1.2.4.1.2 "><p id="p20159183474511"><a name="p20159183474511"></a><a name="p20159183474511"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="74.16%" headers="mcps1.2.4.1.3 "><p id="p47801866195"><a name="p47801866195"></a><a name="p47801866195"></a>共享缓冲区，用于存放API内部计算产生的临时数据。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。共享缓冲区大小的获取方式请参考<a href="Transpose-Tiling.md">Transpose Tiling</a>。</p>
<p id="p14203184218188"><a name="p14203184218188"></a><a name="p14203184218188"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_2"><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_2"><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_2"><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row26611527105912"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p13661162716595"><a name="p13661162716595"></a><a name="p13661162716595"></a>transposeType</p>
</td>
<td class="cellrowborder" valign="top" width="8.07%" headers="mcps1.2.4.1.2 "><p id="p196611227125920"><a name="p196611227125920"></a><a name="p196611227125920"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="74.16%" headers="mcps1.2.4.1.3 "><p id="p66619278598"><a name="p66619278598"></a><a name="p66619278598"></a>数据排布及reshape的类型，类型为TransposeType枚举类。</p>
<a name="screen570192952518"></a><a name="screen570192952518"></a><pre class="screen" codetype="Cpp" id="screen570192952518">enum class TransposeType : uint8_t {
    TRANSPOSE_TYPE_NONE,            // default value
    TRANSPOSE_NZ2ND_0213,           // 场景1：NZ2ND，1、2轴互换
    TRANSPOSE_NZ2NZ_0213,           // 场景2：NZ2NZ，1、2轴互换
    TRANSPOSE_NZ2NZ_012_WITH_N,     // 场景3：NZ2NZ，尾轴切分
    TRANSPOSE_NZ2ND_012_WITH_N,     // 场景4：NZ2ND，尾轴切分
    TRANSPOSE_NZ2ND_012_WITHOUT_N,  // 场景5：NZ2ND，尾轴合并
    TRANSPOSE_NZ2NZ_012_WITHOUT_N,  // 场景6：NZ2NZ，尾轴合并
    TRANSPOSE_ND2ND_ONLY,           // 场景7：二维转置 
    TRANSPOSE_ND_UB_GM,             // 当前不支持
    TRANSPOSE_GRAD_ND_UB_GM,        // 当前不支持
    TRANSPOSE_ND2ND_B16,            // 当前不支持
    TRANSPOSE_NCHW2NHWC,            // 当前不支持
    TRANSPOSE_NHWC2NCHW            // 当前不支持
    
    
     
             
    };</pre>
</td>
</tr>
<tr id="row041413392198"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p4630634141515"><a name="p4630634141515"></a><a name="p4630634141515"></a>tiling</p>
</td>
<td class="cellrowborder" valign="top" width="8.07%" headers="mcps1.2.4.1.2 "><p id="p263018345154"><a name="p263018345154"></a><a name="p263018345154"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="74.16%" headers="mcps1.2.4.1.3 "><p id="p13630123491515"><a name="p13630123491515"></a><a name="p13630123491515"></a>计算所需tiling信息，Tiling信息的获取请参考<a href="Transpose-Tiling.md">Transpose Tiling</a>。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。

## 调用示例<a name="section94691236101419"></a>

本示例为场景1（NZ2ND，1、2轴互换）示例：

输入Tensor \{ shape:\[B, N, H/N/16, S/16, 16, 16\], origin\_shape：\[B, N, S, H/N\], format:"NZ", origin\_format:"ND"\}

输出Tensor \{ shape:\[B, S, N, H/N\], origin\_shape:\[B, S, N, H/N\], format:"ND", origin\_format:"ND"\}

B=1，N=2, S=64, H/N=32，输入数据类型均为half。

```
AscendC::TPipe *pipe = pipeIn;
AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueSrcVecIn;
AscendC::TQue<AscendC::TPosition::VECOUT, 1> inQueueSrcVecOut;
pipe->InitBuffer(inQueueSrcVecIn, 1, b * n * s * hnDiv * sizeof(T));
pipe->InitBuffer(inQueueSrcVecOut, 1, b * n * s * hnDiv * sizeof(T));
AscendC::Transpose(dst, src, AscendC::TransposeType::TRANSPOSE_NZ2ND_0213, this->tiling);
```

