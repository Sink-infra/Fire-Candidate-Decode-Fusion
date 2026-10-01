# AscendDequant<a name="ZH-CN_TOPIC_0000001863640817"></a>

## 产品支持情况<a name="section1586581915393"></a>

<a name="table169596713360"></a>
<table><thead align="left"><tr id="row129590715369"><th class="cellrowborder" valign="top" width="57.95%" id="mcps1.1.3.1.1"><p id="p17959971362"><a name="p17959971362"></a><a name="p17959971362"></a><span id="ph895914718367"><a name="ph895914718367"></a><a name="ph895914718367"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42.05%" id="mcps1.1.3.1.2"><p id="p89594763612"><a name="p89594763612"></a><a name="p89594763612"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row18959673369"><td class="cellrowborder" valign="top" width="57.95%" headers="mcps1.1.3.1.1 "><p id="p1595910763613"><a name="p1595910763613"></a><a name="p1595910763613"></a><span id="ph1595918753613"><a name="ph1595918753613"></a><a name="ph1595918753613"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.05%" headers="mcps1.1.3.1.2 "><p id="p1695957133611"><a name="p1695957133611"></a><a name="p1695957133611"></a>√</p>
</td>
</tr>
<tr id="row18959157103612"><td class="cellrowborder" valign="top" width="57.95%" headers="mcps1.1.3.1.1 "><p id="p13959117193618"><a name="p13959117193618"></a><a name="p13959117193618"></a><span id="ph9959117173614"><a name="ph9959117173614"></a><a name="ph9959117173614"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.05%" headers="mcps1.1.3.1.2 "><p id="p1095914793613"><a name="p1095914793613"></a><a name="p1095914793613"></a>√</p>
</td>
</tr>
<tr id="row89591478362"><td class="cellrowborder" valign="top" width="57.95%" headers="mcps1.1.3.1.1 "><p id="p7959157163619"><a name="p7959157163619"></a><a name="p7959157163619"></a><span id="ph1995997193619"><a name="ph1995997193619"></a><a name="ph1995997193619"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.05%" headers="mcps1.1.3.1.2 "><p id="p149598793615"><a name="p149598793615"></a><a name="p149598793615"></a>√</p>
</td>
</tr>
<tr id="row04294339114"><td class="cellrowborder" valign="top" width="57.95%" headers="mcps1.1.3.1.1 "><p id="p17430203315117"><a name="p17430203315117"></a><a name="p17430203315117"></a><span id="ph530409171210"><a name="ph530409171210"></a><a name="ph530409171210"></a>Kirin X90</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.05%" headers="mcps1.1.3.1.2 "><p id="p7162124721111"><a name="p7162124721111"></a><a name="p7162124721111"></a>√</p>
</td>
</tr>
<tr id="row102201535171119"><td class="cellrowborder" valign="top" width="57.95%" headers="mcps1.1.3.1.1 "><p id="p0220113513110"><a name="p0220113513110"></a><a name="p0220113513110"></a><span id="ph1110031213125"><a name="ph1110031213125"></a><a name="ph1110031213125"></a>Kirin 9030</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.05%" headers="mcps1.1.3.1.2 "><p id="p92202035131117"><a name="p92202035131117"></a><a name="p92202035131117"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

按元素做反量化计算，比如将int32\_t数据类型反量化为half/float等数据类型。**本接口最多支持输入为二维数据，不支持更高维度的输入。**

-   假设输入srcTensor的shape为**（m, n）**，每行数据（即n个输入数据）所占字节数要求**32字节对齐**，每行中进行反量化的元素个数为**calCount**；
-   反量化系数deqScale可以为标量或者向量，为向量的情况下，calCount <= deqScale的元素个数，只有前CalCount个反量化系数生效；
-   输出dstTensor的shape为**（m, n\_dst）**， n \* sizeof\(dstT\)不满足32字节对齐时，需要**向上补齐为32字节**，n\_dst为向上补齐后的列数。

下面通过两个具体的示例来解释参数的配置和计算逻辑（下文中DequantParams类型为存储shape信息的结构体\{m, n, calCount\}）：

-   如下图示例中，srcTensor的数据类型为int32\_t，m = 4，n = 8，calCount = 4，表明srcTensor中每行进行反量化的元素个数为4，deqScale中的前4个数生效，后12个数不参与反量化计算；dstTensor的数据类型为bfloat16\_t，m = 4，n\_dst = 16 \(16 \* sizeof\(bfloat16\_t\) % 32 = 0\)。计算逻辑是srcTensor的每n个数为一行，对于每行中的前calCount个元素，该行srcTensor的第i个元素与deqScale的第i个元素进行相乘写入dstTensor对应行的第i个元素，dstTensor对应行的第calCount + 1个元素\~第n\_dst个元素均为不确定的值。

    ![](figures/zh-cn_image_0000002155016964.png)

-   如下示例中，srcTensor的数据类型为int32\_t，m = 4，n = 8， calCount = 4，表明srcTensor中每行进行反量化的元素个数为4；dstTensor的数据类型为float，m = 4，n\_dst = 8 \(8 \* sizeof\(float\) % 32 = 0\)。对于srcTensor每行中的前4个元素都和标量deqScale相乘并写入dstTensor中每行的对应位置。

    ![](figures/zh-cn_image_0000001819864122.png)

当用户将模板参数中的mode配置为**DEQUANT\_WITH\_SINGLE\_ROW**时：

针对DequantParams \{m, n, calCount\}， 若同时满足以下3个条件：

1.  m = 1
2.  calCount为 32 / sizeof\(dstT\)的倍数
3.  n % calCount = 0

此时 \{1, n, calCount\}会被视作为** \{n / calCount, calCount, calCount\}**  进行反量化的计算。

具体效果可看下图所示，传入的DequantParams为 \{1, 16, 8\}。因为dstT为float，所以calCount满足为8的倍数，在**DEQUANT\_WITH\_SINGLE\_ROW**模式下会将\{1, 2 \* 8, 8\}转换为 \{2, 8, 8\}进行计算。

![](figures/zh-cn_image_0000001820178290.png)

![](figures/zh-cn_image_0000001866976705.png)

-   PER\_TOKEN反量化：srcTensor的每组token（token为n方向，共有m组token）中的元素共享一组deqscale参数，srcTensor为\[m, n\]时，deqscale为\[m, 1\]。

    ![](figures/zh-cn_formulaimage_0000002105578933.png)

-   PER\_GROUP反量化：这里定义group的计算方向为k方向，srcTensor在k方向上每groupSize个元素共享一组deqscale参数。srcTensor为\[m, n\]时，如果kDim=0，表示k是m方向，deqscale为\[\(m + groupSize - 1\) / groupSize, n\]；如果kDim=1，表示k是n方向，deqscale的shape为\[m，\(n + groupSize - 1\) / groupSize\]。
    -   kDim=0：

        ![](figures/zh-cn_formulaimage_0000002069815092.png)

    -   kDim=1：

        ![](figures/zh-cn_formulaimage_0000002105655005.png)

## 实现原理<a name="section13229175017585"></a>

以数据类型int32\_t，shape为\[m, n\]的输入srcTensor，数据类型scaleT，shape为\[n\]的输入deqScale和数据类型dstT，shape为\[m, n\]的输出dstTensor为例，描述AscendDequant高阶API内部算法框图，如下图所示。

**图 1**  AscendDequant内部算法框图<a name="fig15964941122"></a>  
![](figures/AscendDequant内部算法框图.png "AscendDequant内部算法框图")

计算过程分为如下几步，均在Vector上进行：

1.  精度转换：将srcTensor和deqScale都转换成FP32精度的tensor，分别得到srcFP32和deqScaleFP32；
2.  Mul计算：srcFP32一共有m行，每行长度为n；通过m次循环，将srcFP32的每行与deqScaleFP32相乘，通过mask控制仅对前dequantParams.calcount个数进行mul计算，图中index的取值范围为 \[0, m\)，对应srcFP32的每一行；计算所得结果为mulRes，shape为\[m, n\]；
3.  结果数据精度转换：mulRes从FP32转换成dstT类型的tensor，所得结果为dstTensor，shape为\[m, n\]。

PER\_TOKEN/PER\_GROUP场景下，输入srcTensor数据类型是int32\_t/float，此时内部算法框图如下所示。

**图 2**  AscendDequant PER\_TOKEN/PER\_GROUP内部算法框图<a name="fig575484943219"></a>  
![](figures/AscendDequant-PER_TOKEN-PER_GROUP内部算法框图.png "AscendDequant-PER_TOKEN-PER_GROUP内部算法框图")

PER\_TOKEN/PER\_GROUP场景的计算逻辑如下：

1.  读取数据：连续读取输入srcTensor；根据不同的场景，对输入deqscale，采用不同的读取方式；例如，PER\_TOKEN场景做Broadcast处理，PER\_GROUP场景做Gather处理；
2.  精度转换：根据不同输入的数据类型组合，对srcTensor/deqscale进行相应的数据类型转换；
3.  计算：对类型转换后的srcTensor和deqscale数据做乘法；
4.  精度转换：将上述计算得到的结果转换成dstT类型，得到最终输出。

## 函数原型<a name="section620mcpsimp"></a>

-   反量化参数deqScale为矢量
    -   通过sharedTmpBuffer入参传入临时空间

        ```
        template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
        __aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const LocalTensor<scaleT>& deqScale, const LocalTensor<uint8_t>& sharedTmpBuffer, DequantParams params)
        ```

    -   接口框架申请临时空间

        ```
        template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
        __aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const LocalTensor<scaleT>& deqScale, DequantParams params)
        ```

    -   PER\_TOKEN/PER\_GROUP量化
        -   通过sharedTmpBuffer入参传入临时空间

            ```
            template <typename dstT, typename srcT, typename scaleT, const AscendDeQuantConfig& config, const AscendDeQuantPolicy& policy>
            __aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<srcT>& srcTensor, const LocalTensor<scaleT>& scaleTensor, const LocalTensor<scaleT>& offsetTensor, const LocalTensor<uint8_t>& sharedTmpBuffer, const AscendDeQuantParam& para)
            ```

        -   接口框架申请临时空间

            ```
            template <typename dstT, typename srcT, typename scaleT, const AscendDeQuantConfig& config, const AscendDeQuantPolicy& policy>
            __aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<srcT>& srcTensor, const LocalTensor<scaleT>& scaleTensor, const LocalTensor<scaleT>& offsetTensor, const AscendDeQuantParam& para)
            ```

-   反量化参数deqScale为标量
    -   通过sharedTmpBuffer入参传入临时空间

        ```
        template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
        __aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const scaleT deqScale, const LocalTensor<uint8_t>& sharedTmpBuffer, DequantParams params)
        ```

    -   接口框架申请临时空间

        ```
        template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
        __aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const scaleT deqScale, DequantParams params)
        ```

由于该接口的内部实现中涉及复杂的数学计算，需要额外的临时空间来存储计算过程中的中间变量。临时空间支持**接口框架申请**和开发者**通过sharedTmpBuffer入参传入**两种方式。

-   接口框架申请临时空间，开发者无需申请，但是需要预留临时空间的大小。

-   通过sharedTmpBuffer入参传入，使用该tensor作为临时空间进行处理，接口框架不再申请。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。

接口框架申请的方式，开发者需要预留临时空间；通过sharedTmpBuffer传入的情况，开发者需要为sharedTmpBuffer申请空间。临时空间大小BufferSize的获取方式如下：通过[GetAscendDequantMaxMinTmpSize](GetAscendDequantMaxMinTmpSize.md)中提供的GetAscendDequantMaxMinTmpSize接口获取需要预留空间的范围大小。

以下接口不推荐使用，新开发内容不要使用如下接口：

```
template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
__aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const LocalTensor<scaleT>& deqScale, const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
```

```
template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
__aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const LocalTensor<scaleT>& deqScale, const LocalTensor<uint8_t>& sharedTmpBuffer)
```

```
template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
__aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const LocalTensor<scaleT>& deqScale, const uint32_t calCount)
```

```
template <typename dstT, typename scaleT, DeQuantMode mode = DeQuantMode::DEQUANT_WITH_SINGLE_ROW>
__aicore__ inline void AscendDequant(const LocalTensor<dstT>& dstTensor, const LocalTensor<int32_t>& srcTensor, const LocalTensor<scaleT>& deqScale)
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
<tbody><tr id="row14755141911264"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p47551198266"><a name="p47551198266"></a><a name="p47551198266"></a>dstT</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p125969172719"><a name="p125969172719"></a><a name="p125969172719"></a>目的操作数的数据类型。</p>
</td>
</tr>
<tr id="row6356241194912"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p143561041144915"><a name="p143561041144915"></a><a name="p143561041144915"></a>scaleT</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p4299155115268"><a name="p4299155115268"></a><a name="p4299155115268"></a>deqScale的数据类型。</p>
</td>
</tr>
<tr id="row9756719122620"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p1682112447268"><a name="p1682112447268"></a><a name="p1682112447268"></a>mode</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><div class="p" id="p7851923173111"><a name="p7851923173111"></a><a name="p7851923173111"></a>决定当DequantParams为{1, n, calCount}时的计算逻辑，传入enum DeQuantMode，支持以下 2 种配置：<a name="ul168513231318"></a><a name="ul168513231318"></a><ul id="ul168513231318"><li><strong id="b14851162310311"><a name="b14851162310311"></a><a name="b14851162310311"></a>DEQUANT_WITH_SINGLE_ROW</strong>：当DequantParams {m, n, calCount} 同时满足以下条件：1、m = 1；2、calCount为 32 / sizeof(dstT)的倍数；3、n % calCount = 0时，即 {1, n, calCount} 会当作 {n / calCount, calCount, calCount} 进行计算。</li><li><strong id="b1385182313116"><a name="b1385182313116"></a><a name="b1385182313116"></a>DEQUANT_WITH_MULTI_ROW</strong>：即使满足上述所有条件，{1, n, calCount} 依然只会当作 {1, n, calCount} 进行计算， 即总共n个数，前calCount个数进行反量化的计算。</li></ul>
</div>
</td>
</tr>
</tbody>
</table>

**表 2**  PER\_TOKEN/PER\_GROUP场景模板参数说明

<a name="table589125485010"></a>
<table><thead align="left"><tr id="row489125425015"><th class="cellrowborder" valign="top" width="19.259999999999998%" id="mcps1.2.3.1.1"><p id="p6891105420508"><a name="p6891105420508"></a><a name="p6891105420508"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.74%" id="mcps1.2.3.1.2"><p id="p17891145445014"><a name="p17891145445014"></a><a name="p17891145445014"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1891165475020"><td class="cellrowborder" valign="top" width="19.259999999999998%" headers="mcps1.2.3.1.1 "><p id="p68921754135010"><a name="p68921754135010"></a><a name="p68921754135010"></a>srcT</p>
</td>
<td class="cellrowborder" valign="top" width="80.74%" headers="mcps1.2.3.1.2 "><p id="p5182154805113"><a name="p5182154805113"></a><a name="p5182154805113"></a>源操作数的数据类型。</p>
</td>
</tr>
<tr id="row089212543508"><td class="cellrowborder" valign="top" width="19.259999999999998%" headers="mcps1.2.3.1.1 "><p id="p15892165425015"><a name="p15892165425015"></a><a name="p15892165425015"></a>config</p>
</td>
<td class="cellrowborder" valign="top" width="80.74%" headers="mcps1.2.3.1.2 "><p id="p13892754135017"><a name="p13892754135017"></a><a name="p13892754135017"></a>量化接口配置参数，AscendDeQuantConfig类型，具体定义如下。</p>
<a name="screen1434141315715"></a><a name="screen1434141315715"></a><pre class="screen" codetype="Cpp" id="screen1434141315715">struct AscendDeQuantConfig {
        bool hasOffset;
        int32_t kDim = 1;
}</pre>
<a name="ul558513317417"></a><a name="ul558513317417"></a><ul id="ul558513317417"><li>hasOffset：量化参数offset是否参与计算。<a name="ul186211135517"></a><a name="ul186211135517"></a><ul id="ul186211135517"><li>True：表示offset参数参与计算。</li><li>False：表示offset参数不参与计算。</li></ul>
</li><li>kDim：group的计算方向，即k方向。仅在PER_GROUP场景有效，支持的取值如下。<a name="ul980613301459"></a><a name="ul980613301459"></a><ul id="ul980613301459"><li>0：k轴是第0轴，即m方向为group的计算方向；</li><li>1：k轴是第1轴，即n方向为group的计算方向。</li></ul>
</li></ul>
</td>
</tr>
<tr id="row13892205418504"><td class="cellrowborder" valign="top" width="19.259999999999998%" headers="mcps1.2.3.1.1 "><p id="p10892554165011"><a name="p10892554165011"></a><a name="p10892554165011"></a>policy</p>
</td>
<td class="cellrowborder" valign="top" width="80.74%" headers="mcps1.2.3.1.2 "><p id="p10892185420508"><a name="p10892185420508"></a><a name="p10892185420508"></a>量化策略配置参数，枚举类型，可取值如下：</p>
<a name="screen10546253131615"></a><a name="screen10546253131615"></a><pre class="screen" codetype="Cpp" id="screen10546253131615">enum class AscendDeQuantPolicy : int32_t {
        PER_TOKEN,  // 配置为PER_TOKEN模式
        PER_GROUP, // 配置为PER_GROUP模式
        PER_CHANNEL_PER_GROUP, // 预留参数，暂不支持
        PER_TOEKN_PER_GROUP // 预留参数，暂不支持
}</pre>
</td>
</tr>
</tbody>
</table>

**表 3**  接口参数说明

<a name="table44731299481"></a>
<table><thead align="left"><tr id="row247482914489"><th class="cellrowborder" valign="top" width="15.55%" id="mcps1.2.4.1.1"><p id="p147413295483"><a name="p147413295483"></a><a name="p147413295483"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="9.24%" id="mcps1.2.4.1.2"><p id="p1147432994819"><a name="p1147432994819"></a><a name="p1147432994819"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="75.21%" id="mcps1.2.4.1.3"><p id="p74749297483"><a name="p74749297483"></a><a name="p74749297483"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row12474329104814"><td class="cellrowborder" valign="top" width="15.55%" headers="mcps1.2.4.1.1 "><p id="p1047411294482"><a name="p1047411294482"></a><a name="p1047411294482"></a>dstTensor</p>
</td>
<td class="cellrowborder" valign="top" width="9.24%" headers="mcps1.2.4.1.2 "><p id="p047412984813"><a name="p047412984813"></a><a name="p047412984813"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="75.21%" headers="mcps1.2.4.1.3 "><p id="p3989161814016"><a name="p3989161814016"></a><a name="p3989161814016"></a>目的操作数。类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="p12247148298"><a name="p12247148298"></a><a name="p12247148298"></a><span id="ph152477481396"><a name="ph152477481396"></a><a name="ph152477481396"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、bfloat16_t、float。</p>
<p id="p3343929193720"><a name="p3343929193720"></a><a name="p3343929193720"></a><span id="ph1634312291372"><a name="ph1634312291372"></a><a name="ph1634312291372"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half、bfloat16_t、float。</p>
<p id="p1647418294485"><a name="p1647418294485"></a><a name="p1647418294485"></a><span id="ph74741329164816"><a name="ph74741329164816"></a><a name="ph74741329164816"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half、bfloat16_t、float。</p>
<p id="p152497566102"><a name="p152497566102"></a><a name="p152497566102"></a><span id="ph184955111218"><a name="ph184955111218"></a><a name="ph184955111218"></a>Kirin X90</span>，支持的数据类型为：half、float。</p>
<p id="p174578443119"><a name="p174578443119"></a><a name="p174578443119"></a><span id="ph69300917129"><a name="ph69300917129"></a><a name="ph69300917129"></a>Kirin 9030</span>，支持的数据类型为：half、float。</p>
<a name="ul66981895213"></a><a name="ul66981895213"></a><ul id="ul66981895213"><li>dstTensor的行数和srcTensor的行数保持一致。</li><li>n * sizeof(dstT)不满足32字节对齐时，需要<strong id="b1782911052214"><a name="b1782911052214"></a><a name="b1782911052214"></a>向上补齐为32字节</strong>，n_dst为向上补齐后的列数。如srcTensor数据类型为int32_t，shape为 (4, 8)，dstTensor为bfloat16_t，则n_dst应从8补齐为16，dstTensor shape为(4, 16)。补齐的计算过程为：n_dst = (8 * sizeof(bfloat16_t) + 32 - 1) / 32 * 32 / sizeof(bfloat16_t)。</li></ul>
</td>
</tr>
<tr id="row18474729124817"><td class="cellrowborder" valign="top" width="15.55%" headers="mcps1.2.4.1.1 "><p id="p54741029164810"><a name="p54741029164810"></a><a name="p54741029164810"></a>srcTensor</p>
</td>
<td class="cellrowborder" valign="top" width="9.24%" headers="mcps1.2.4.1.2 "><p id="p144741829194814"><a name="p144741829194814"></a><a name="p144741829194814"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="75.21%" headers="mcps1.2.4.1.3 "><p id="p143185337247"><a name="p143185337247"></a><a name="p143185337247"></a>源操作数。类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="p1797213717147"><a name="p1797213717147"></a><a name="p1797213717147"></a><span id="ph797220791415"><a name="ph797220791415"></a><a name="ph797220791415"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：int32_t。</p>
<p id="p149295884015"><a name="p149295884015"></a><a name="p149295884015"></a><span id="ph8929208184020"><a name="ph8929208184020"></a><a name="ph8929208184020"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：int32_t。</p>
<p id="p14496195212317"><a name="p14496195212317"></a><a name="p14496195212317"></a><span id="ph104968521634"><a name="ph104968521634"></a><a name="ph104968521634"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：int32_t。</p>
<p id="p1550603911125"><a name="p1550603911125"></a><a name="p1550603911125"></a><span id="ph850653981211"><a name="ph850653981211"></a><a name="ph850653981211"></a>Kirin X90</span>，支持的数据类型为：int32_t。</p>
<p id="p155063394127"><a name="p155063394127"></a><a name="p155063394127"></a><span id="ph1050619395120"><a name="ph1050619395120"></a><a name="ph1050619395120"></a>Kirin 9030</span>，支持的数据类型为：int32_t。</p>
<p id="p2836135311718"><a name="p2836135311718"></a><a name="p2836135311718"></a>shape为 [m, n]，n个输入数据所占字节数要求<strong id="b733515211256"><a name="b733515211256"></a><a name="b733515211256"></a>32字节对齐</strong>。</p>
</td>
</tr>
<tr id="row617218172310"><td class="cellrowborder" valign="top" width="15.55%" headers="mcps1.2.4.1.1 "><p id="p16660132211315"><a name="p16660132211315"></a><a name="p16660132211315"></a>deqScale</p>
</td>
<td class="cellrowborder" valign="top" width="9.24%" headers="mcps1.2.4.1.2 "><p id="p156601822153115"><a name="p156601822153115"></a><a name="p156601822153115"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="75.21%" headers="mcps1.2.4.1.3 "><p id="p12593175910168"><a name="p12593175910168"></a><a name="p12593175910168"></a>源操作数。类型为标量或者<a href="LocalTensor.md">LocalTensor</a>。类型为LocalTensor时，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="p71351262148"><a name="p71351262148"></a><a name="p71351262148"></a><span id="ph7135162661418"><a name="ph7135162661418"></a><a name="ph7135162661418"></a>Ascend 950PR/Ascend 950DT</span>，当deqScale为矢量时，支持的数据类型为：uint64_t、float、bfloat16_t；当deqScale为标量时，支持的数据类型为bfloat16_t、float。</p>
<p id="p887385104019"><a name="p887385104019"></a><a name="p887385104019"></a><span id="ph4873125184020"><a name="ph4873125184020"></a><a name="ph4873125184020"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_3"><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_3"><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a>Atlas A3 推理系列产品</term></span>，当deqScale为矢量时，支持的数据类型为：uint64_t、float、bfloat16_t；当deqScale为标量时，支持的数据类型为bfloat16_t、float。</p>
<p id="p814261713282"><a name="p814261713282"></a><a name="p814261713282"></a><span id="ph76609228311"><a name="ph76609228311"></a><a name="ph76609228311"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_3"><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_3"><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a>Atlas A2 推理系列产品</term></span>，当deqScale为矢量时，支持的数据类型为：uint64_t、float、bfloat16_t；当deqScale为标量时，支持的数据类型为bfloat16_t、float。</p>
<p id="p11825155515364"><a name="p11825155515364"></a><a name="p11825155515364"></a>dstTensor、srcTensor、deqScale支持的数据类型组合请参考<a href="#table1963437121712">表5</a>和<a href="#table16300356102013">表6</a>。</p>
<p id="p580011124132"><a name="p580011124132"></a><a name="p580011124132"></a><span id="ph1080031215134"><a name="ph1080031215134"></a><a name="ph1080031215134"></a>Kirin X90</span>，当deqScale为矢量时，支持的数据类型为：uint64_t、float；当deqScale为标量时，支持的数据类型为float。</p>
<p id="p180021291314"><a name="p180021291314"></a><a name="p180021291314"></a><span id="ph2800151219137"><a name="ph2800151219137"></a><a name="ph2800151219137"></a>Kirin 9030</span>，当deqScale为矢量时，支持的数据类型为：uint64_t、float；当deqScale为标量时，支持的数据类型为float。</p>
</td>
</tr>
<tr id="row1747412296483"><td class="cellrowborder" valign="top" width="15.55%" headers="mcps1.2.4.1.1 "><p id="p74741029204817"><a name="p74741029204817"></a><a name="p74741029204817"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="9.24%" headers="mcps1.2.4.1.2 "><p id="p1747452954810"><a name="p1747452954810"></a><a name="p1747452954810"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="75.21%" headers="mcps1.2.4.1.3 "><p id="p191160465422"><a name="p191160465422"></a><a name="p191160465422"></a>临时缓存。类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="p5881016172817"><a name="p5881016172817"></a><a name="p5881016172817"></a>临时空间大小BufferSize的获取方式请参考<a href="GetAscendDequantMaxMinTmpSize.md">GetAscendDequantMaxMinTmpSize</a>。</p>
<p id="p117351349203919"><a name="p117351349203919"></a><a name="p117351349203919"></a><span id="ph127351049183914"><a name="ph127351049183914"></a><a name="ph127351049183914"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：uint8_t。</p>
<p id="p3990133464118"><a name="p3990133464118"></a><a name="p3990133464118"></a><span id="ph4990133415414"><a name="ph4990133415414"></a><a name="ph4990133415414"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_4"><a name="zh-cn_topic_0000001312391781_term1253731311225_4"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_4"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_4"><a name="zh-cn_topic_0000001312391781_term131434243115_4"></a><a name="zh-cn_topic_0000001312391781_term131434243115_4"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：uint8_t。</p>
<p id="p372173815911"><a name="p372173815911"></a><a name="p372173815911"></a><span id="ph173638145913"><a name="ph173638145913"></a><a name="ph173638145913"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_4"><a name="zh-cn_topic_0000001312391781_term11962195213215_4"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_4"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_4"><a name="zh-cn_topic_0000001312391781_term184716139811_4"></a><a name="zh-cn_topic_0000001312391781_term184716139811_4"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：uint8_t。</p>
<p id="p8269388141"><a name="p8269388141"></a><a name="p8269388141"></a><span id="ph1226917801416"><a name="ph1226917801416"></a><a name="ph1226917801416"></a>Kirin X90</span>，支持的数据类型为：uint8_t。</p>
<p id="p4269182143"><a name="p4269182143"></a><a name="p4269182143"></a><span id="ph726913816144"><a name="ph726913816144"></a><a name="ph726913816144"></a>Kirin 9030</span>，支持的数据类型为：uint8_t。</p>
</td>
</tr>
<tr id="row850382835820"><td class="cellrowborder" valign="top" width="15.55%" headers="mcps1.2.4.1.1 "><p id="p714563310589"><a name="p714563310589"></a><a name="p714563310589"></a>params</p>
</td>
<td class="cellrowborder" valign="top" width="9.24%" headers="mcps1.2.4.1.2 "><p id="p210914395588"><a name="p210914395588"></a><a name="p210914395588"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="75.21%" headers="mcps1.2.4.1.3 "><p id="p49691232086"><a name="p49691232086"></a><a name="p49691232086"></a>srcTensor的shape信息。DequantParams类型，具体定义如下：</p>
<a name="screen641172125916"></a><a name="screen641172125916"></a><pre class="screen" codetype="Cpp" id="screen641172125916">struct DequantParams
{
    uint32_t m;             // srcTensor的行数
    uint32_t n;             // srcTensor的列数
    uint32_t calCount;      // 针对srcTensor每一行，前calCount个数为有效数据，与deqScale的前calCount个数或者deqScale标量进行乘法计算
};</pre>
<a name="ul19603810111110"></a><a name="ul19603810111110"></a><ul id="ul19603810111110"><li>DequantParams.n * sizeof(T)必须是32字节的整数倍，T为srcTensor中元素的数据类型。</li><li>因为是每n个数中的前calCount个数进行乘法运算，因此DequantParams.n和calCount需要满足以下关系<p id="p10782229968"><a name="p10782229968"></a><a name="p10782229968"></a>1 &lt;= DequantParams.calCount &lt;= DequantParams.n。</p>
</li><li>deqScale为矢量时，DequantParams.calCount &lt;= deqScale的元素个数。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 4**  PER\_TOKEN/PER\_GROUP场景接口参数说明

<a name="table1735215113117"></a>
<table><thead align="left"><tr id="row435295114110"><th class="cellrowborder" valign="top" width="16.661666166616662%" id="mcps1.2.4.1.1"><p id="p1735225111115"><a name="p1735225111115"></a><a name="p1735225111115"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="10.98109810981098%" id="mcps1.2.4.1.2"><p id="p18352115112119"><a name="p18352115112119"></a><a name="p18352115112119"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.35723572357236%" id="mcps1.2.4.1.3"><p id="p1135235110113"><a name="p1135235110113"></a><a name="p1135235110113"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row193524511719"><td class="cellrowborder" valign="top" width="16.661666166616662%" headers="mcps1.2.4.1.1 "><p id="p15352951716"><a name="p15352951716"></a><a name="p15352951716"></a>dstTensor</p>
</td>
<td class="cellrowborder" valign="top" width="10.98109810981098%" headers="mcps1.2.4.1.2 "><p id="p1635214511319"><a name="p1635214511319"></a><a name="p1635214511319"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.35723572357236%" headers="mcps1.2.4.1.3 "><p id="p18204795410"><a name="p18204795410"></a><a name="p18204795410"></a>目的操作数。</p>
<p id="p320416920413"><a name="p320416920413"></a><a name="p320416920413"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p112044915418"><a name="p112044915418"></a><a name="p112044915418"></a><span id="ph520420914410"><a name="ph520420914410"></a><a name="ph520420914410"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、bfloat16_t、float。</p>
</td>
</tr>
<tr id="row123522518116"><td class="cellrowborder" valign="top" width="16.661666166616662%" headers="mcps1.2.4.1.1 "><p id="p63525511816"><a name="p63525511816"></a><a name="p63525511816"></a>srcTensor</p>
</td>
<td class="cellrowborder" valign="top" width="10.98109810981098%" headers="mcps1.2.4.1.2 "><p id="p14352105115117"><a name="p14352105115117"></a><a name="p14352105115117"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.35723572357236%" headers="mcps1.2.4.1.3 "><p id="p1320459948"><a name="p1320459948"></a><a name="p1320459948"></a>源操作数。</p>
<p id="p82041993416"><a name="p82041993416"></a><a name="p82041993416"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p1204791548"><a name="p1204791548"></a><a name="p1204791548"></a><span id="ph920413915412"><a name="ph920413915412"></a><a name="ph920413915412"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：int32_t、float。</p>
</td>
</tr>
<tr id="row735217510112"><td class="cellrowborder" valign="top" width="16.661666166616662%" headers="mcps1.2.4.1.1 "><p id="p135215512117"><a name="p135215512117"></a><a name="p135215512117"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="10.98109810981098%" headers="mcps1.2.4.1.2 "><p id="p1635245114114"><a name="p1635245114114"></a><a name="p1635245114114"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.35723572357236%" headers="mcps1.2.4.1.3 "><p id="p6205129543"><a name="p6205129543"></a><a name="p6205129543"></a>临时缓存。</p>
<p id="p1120549049"><a name="p1120549049"></a><a name="p1120549049"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_2"><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_2"><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_2"><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p62051192418"><a name="p62051192418"></a><a name="p62051192418"></a>临时空间大小BufferSize的获取方式请参考<a href="GetAscendQuantMaxMinTmpSize.md">GetAscendQuantMaxMinTmpSize</a>。</p>
<p id="p152052091048"><a name="p152052091048"></a><a name="p152052091048"></a><span id="ph142051991410"><a name="ph142051991410"></a><a name="ph142051991410"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：uint8_t。</p>
</td>
</tr>
<tr id="row1235211516111"><td class="cellrowborder" valign="top" width="16.661666166616662%" headers="mcps1.2.4.1.1 "><p id="p5352185119112"><a name="p5352185119112"></a><a name="p5352185119112"></a>scaleTensor</p>
</td>
<td class="cellrowborder" valign="top" width="10.98109810981098%" headers="mcps1.2.4.1.2 "><p id="p153521051117"><a name="p153521051117"></a><a name="p153521051117"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.35723572357236%" headers="mcps1.2.4.1.3 "><p id="p6205796411"><a name="p6205796411"></a><a name="p6205796411"></a>量化参数scale。</p>
<p id="p1205119748"><a name="p1205119748"></a><a name="p1205119748"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_3"><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_3"><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_3"><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p162051891948"><a name="p162051891948"></a><a name="p162051891948"></a><span id="ph142052912417"><a name="ph142052912417"></a><a name="ph142052912417"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、bfloat16_t、float。</p>
</td>
</tr>
<tr id="row135285114116"><td class="cellrowborder" valign="top" width="16.661666166616662%" headers="mcps1.2.4.1.1 "><p id="p1352175111110"><a name="p1352175111110"></a><a name="p1352175111110"></a>offsetTensor</p>
</td>
<td class="cellrowborder" valign="top" width="10.98109810981098%" headers="mcps1.2.4.1.2 "><p id="p33537511216"><a name="p33537511216"></a><a name="p33537511216"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.35723572357236%" headers="mcps1.2.4.1.3 "><p id="p132061398413"><a name="p132061398413"></a><a name="p132061398413"></a>量化参数offset。预留参数，当前暂不支持。</p>
<p id="p020689945"><a name="p020689945"></a><a name="p020689945"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_4"><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_4"><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_4"><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p122061191946"><a name="p122061191946"></a><a name="p122061191946"></a><span id="ph14206896416"><a name="ph14206896416"></a><a name="ph14206896416"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型和scaleTensor保持一致。</p>
</td>
</tr>
<tr id="row1235316511515"><td class="cellrowborder" valign="top" width="16.661666166616662%" headers="mcps1.2.4.1.1 "><p id="p13533516110"><a name="p13533516110"></a><a name="p13533516110"></a>para</p>
</td>
<td class="cellrowborder" valign="top" width="10.98109810981098%" headers="mcps1.2.4.1.2 "><p id="p17353115110118"><a name="p17353115110118"></a><a name="p17353115110118"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.35723572357236%" headers="mcps1.2.4.1.3 "><p id="p535310512112"><a name="p535310512112"></a><a name="p535310512112"></a>反量化接口的参数，定义如下：</p>
<a name="screen18982315404"></a><a name="screen18982315404"></a><pre class="screen" codetype="Cpp" id="screen18982315404">struct AscendDeQuantParam {
        uint32_t m;
        uint32_t n;
        uint32_t calCount;   
        uint32_t groupSize = 0;
}</pre>
<a name="ul244013277403"></a><a name="ul244013277403"></a><ul id="ul244013277403"><li>m：m方向元素个数。</li><li>n：n方向元素个数。n值对应的数据大小需满足32B对齐的要求，即shape最后一维为n的输入输出均需要满足该维度上32B对齐的要求。</li><li>calCount:参与计算的元素个数。calCount必须是n的整数倍。</li><li>groupSize ：PER_GROUP场景有效，表示groupSize行/列数据共用一个scale/offset。groupSize的取值必须大于0且是32的整倍数。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 5**  支持的数据类型组合（deqScale为LocalTensor）

<a name="table1963437121712"></a>
<table><thead align="left"><tr id="row16963183711175"><th class="cellrowborder" valign="top" width="30.05300530053005%" id="mcps1.2.4.1.1"><p id="p1296310378174"><a name="p1296310378174"></a><a name="p1296310378174"></a>dstTensor</p>
</th>
<th class="cellrowborder" valign="top" width="31.773177317731772%" id="mcps1.2.4.1.2"><p id="p3963237121719"><a name="p3963237121719"></a><a name="p3963237121719"></a>srcTensor</p>
</th>
<th class="cellrowborder" valign="top" width="38.173817381738175%" id="mcps1.2.4.1.3"><p id="p596323771711"><a name="p596323771711"></a><a name="p596323771711"></a>deqScale</p>
</th>
</tr>
</thead>
<tbody><tr id="row19963203771713"><td class="cellrowborder" valign="top" width="30.05300530053005%" headers="mcps1.2.4.1.1 "><p id="p8964203741720"><a name="p8964203741720"></a><a name="p8964203741720"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="31.773177317731772%" headers="mcps1.2.4.1.2 "><p id="p896463713171"><a name="p896463713171"></a><a name="p896463713171"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.173817381738175%" headers="mcps1.2.4.1.3 "><p id="p496411375175"><a name="p496411375175"></a><a name="p496411375175"></a>uint64_t</p>
<p id="p0792388399"><a name="p0792388399"></a><a name="p0792388399"></a>注意：当deqScale的数据类型是uint64_t时，数值低32位是参与计算的数据，数据类型是float，数值高32位是一些控制参数，本接口不使用。</p>
</td>
</tr>
<tr id="row14964537111719"><td class="cellrowborder" valign="top" width="30.05300530053005%" headers="mcps1.2.4.1.1 "><p id="p16964143711712"><a name="p16964143711712"></a><a name="p16964143711712"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="31.773177317731772%" headers="mcps1.2.4.1.2 "><p id="p125026399197"><a name="p125026399197"></a><a name="p125026399197"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.173817381738175%" headers="mcps1.2.4.1.3 "><p id="p8964153701717"><a name="p8964153701717"></a><a name="p8964153701717"></a>float</p>
</td>
</tr>
<tr id="row996423751717"><td class="cellrowborder" valign="top" width="30.05300530053005%" headers="mcps1.2.4.1.1 "><p id="p1396463781710"><a name="p1396463781710"></a><a name="p1396463781710"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="31.773177317731772%" headers="mcps1.2.4.1.2 "><p id="p16204019195"><a name="p16204019195"></a><a name="p16204019195"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.173817381738175%" headers="mcps1.2.4.1.3 "><p id="p10964163715171"><a name="p10964163715171"></a><a name="p10964163715171"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row1196413375174"><td class="cellrowborder" valign="top" width="30.05300530053005%" headers="mcps1.2.4.1.1 "><p id="p7964133731719"><a name="p7964133731719"></a><a name="p7964133731719"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="31.773177317731772%" headers="mcps1.2.4.1.2 "><p id="p253912402195"><a name="p253912402195"></a><a name="p253912402195"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.173817381738175%" headers="mcps1.2.4.1.3 "><p id="p596414373170"><a name="p596414373170"></a><a name="p596414373170"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row361702025017"><td class="cellrowborder" valign="top" width="30.05300530053005%" headers="mcps1.2.4.1.1 "><p id="p16178207509"><a name="p16178207509"></a><a name="p16178207509"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="31.773177317731772%" headers="mcps1.2.4.1.2 "><p id="p4617202014509"><a name="p4617202014509"></a><a name="p4617202014509"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.173817381738175%" headers="mcps1.2.4.1.3 "><p id="p1161712085012"><a name="p1161712085012"></a><a name="p1161712085012"></a>float</p>
</td>
</tr>
</tbody>
</table>

**表 6**  支持的数据类型组合（deqScale为标量）

<a name="table16300356102013"></a>
<table><thead align="left"><tr id="row930015616207"><th class="cellrowborder" valign="top" width="29.982998299829983%" id="mcps1.2.4.1.1"><p id="p12300135632011"><a name="p12300135632011"></a><a name="p12300135632011"></a>dstTensor</p>
</th>
<th class="cellrowborder" valign="top" width="31.623162316231625%" id="mcps1.2.4.1.2"><p id="p430065632012"><a name="p430065632012"></a><a name="p430065632012"></a>srcTensor</p>
</th>
<th class="cellrowborder" valign="top" width="38.39383938393839%" id="mcps1.2.4.1.3"><p id="p53001569204"><a name="p53001569204"></a><a name="p53001569204"></a>deqScale</p>
</th>
</tr>
</thead>
<tbody><tr id="row9300145642013"><td class="cellrowborder" valign="top" width="29.982998299829983%" headers="mcps1.2.4.1.1 "><p id="p15300135615203"><a name="p15300135615203"></a><a name="p15300135615203"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="31.623162316231625%" headers="mcps1.2.4.1.2 "><p id="p10300356132010"><a name="p10300356132010"></a><a name="p10300356132010"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.39383938393839%" headers="mcps1.2.4.1.3 "><p id="p3300155662012"><a name="p3300155662012"></a><a name="p3300155662012"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row114049495503"><td class="cellrowborder" valign="top" width="29.982998299829983%" headers="mcps1.2.4.1.1 "><p id="p204049499508"><a name="p204049499508"></a><a name="p204049499508"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="31.623162316231625%" headers="mcps1.2.4.1.2 "><p id="p74041049145010"><a name="p74041049145010"></a><a name="p74041049145010"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.39383938393839%" headers="mcps1.2.4.1.3 "><p id="p12404149165018"><a name="p12404149165018"></a><a name="p12404149165018"></a>float</p>
</td>
</tr>
<tr id="row1216518171715"><td class="cellrowborder" valign="top" width="29.982998299829983%" headers="mcps1.2.4.1.1 "><p id="p236646111720"><a name="p236646111720"></a><a name="p236646111720"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="31.623162316231625%" headers="mcps1.2.4.1.2 "><p id="p193661968173"><a name="p193661968173"></a><a name="p193661968173"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.39383938393839%" headers="mcps1.2.4.1.3 "><p id="p43669631716"><a name="p43669631716"></a><a name="p43669631716"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row335551062319"><td class="cellrowborder" valign="top" width="29.982998299829983%" headers="mcps1.2.4.1.1 "><p id="p03551010192316"><a name="p03551010192316"></a><a name="p03551010192316"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="31.623162316231625%" headers="mcps1.2.4.1.2 "><p id="p635510107236"><a name="p635510107236"></a><a name="p635510107236"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="38.39383938393839%" headers="mcps1.2.4.1.3 "><p id="p418282714233"><a name="p418282714233"></a><a name="p418282714233"></a>float</p>
</td>
</tr>
</tbody>
</table>

**表 7**  PER\_TOKEN/PER\_GROUP场景支持的数据类型组合

<a name="table158181847102411"></a>
<table><thead align="left"><tr id="row381964718248"><th class="cellrowborder" valign="top" width="30.683068306830684%" id="mcps1.2.4.1.1"><p id="p1681934711240"><a name="p1681934711240"></a><a name="p1681934711240"></a>srcDtype</p>
</th>
<th class="cellrowborder" valign="top" width="33.17331733173317%" id="mcps1.2.4.1.2"><p id="p4819184792415"><a name="p4819184792415"></a><a name="p4819184792415"></a>scaleDtype</p>
</th>
<th class="cellrowborder" valign="top" width="36.14361436143614%" id="mcps1.2.4.1.3"><p id="p48194471241"><a name="p48194471241"></a><a name="p48194471241"></a>dstDtype</p>
</th>
</tr>
</thead>
<tbody><tr id="row1881954718248"><td class="cellrowborder" rowspan="5" valign="top" width="30.683068306830684%" headers="mcps1.2.4.1.1 "><p id="p122550193211"><a name="p122550193211"></a><a name="p122550193211"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.17331733173317%" headers="mcps1.2.4.1.2 "><p id="p1881984712245"><a name="p1881984712245"></a><a name="p1881984712245"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="36.14361436143614%" headers="mcps1.2.4.1.3 "><p id="p14725638123210"><a name="p14725638123210"></a><a name="p14725638123210"></a>half</p>
</td>
</tr>
<tr id="row2819164722415"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p2819104711244"><a name="p2819104711244"></a><a name="p2819104711244"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p87255387324"><a name="p87255387324"></a><a name="p87255387324"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row2819184711242"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p178191447122415"><a name="p178191447122415"></a><a name="p178191447122415"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p11725638203211"><a name="p11725638203211"></a><a name="p11725638203211"></a>float</p>
</td>
</tr>
<tr id="row1481954718242"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p98190478243"><a name="p98190478243"></a><a name="p98190478243"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p4725123873211"><a name="p4725123873211"></a><a name="p4725123873211"></a>half</p>
</td>
</tr>
<tr id="row9819747132418"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p148191847132412"><a name="p148191847132412"></a><a name="p148191847132412"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p4725143873214"><a name="p4725143873214"></a><a name="p4725143873214"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row1181904702416"><td class="cellrowborder" rowspan="5" valign="top" width="30.683068306830684%" headers="mcps1.2.4.1.1 "><p id="p797413291277"><a name="p797413291277"></a><a name="p797413291277"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="33.17331733173317%" headers="mcps1.2.4.1.2 "><p id="p2974122902715"><a name="p2974122902715"></a><a name="p2974122902715"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="36.14361436143614%" headers="mcps1.2.4.1.3 "><p id="p4725173803215"><a name="p4725173803215"></a><a name="p4725173803215"></a>half</p>
</td>
</tr>
<tr id="row12819174742414"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p12974329192720"><a name="p12974329192720"></a><a name="p12974329192720"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p7725538143210"><a name="p7725538143210"></a><a name="p7725538143210"></a>bfloat16_t</p>
</td>
</tr>
<tr id="row12820134710244"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p19974132972710"><a name="p19974132972710"></a><a name="p19974132972710"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p7725123893218"><a name="p7725123893218"></a><a name="p7725123893218"></a>float</p>
</td>
</tr>
<tr id="row1382034714246"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p119742029112718"><a name="p119742029112718"></a><a name="p119742029112718"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p187251138153214"><a name="p187251138153214"></a><a name="p187251138153214"></a>half</p>
</td>
</tr>
<tr id="row16820147142414"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p2097432917271"><a name="p2097432917271"></a><a name="p2097432917271"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p172563812323"><a name="p172563812323"></a><a name="p172563812323"></a>bfloat16_t</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section38228281712"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   **不支持源操作数与目的操作数地址重叠。**
-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。
-   PER\_TOKEN/PER\_GROUP场景，连续计算方向（即n方向）的数据量要求32B对齐。

## 调用示例<a name="section642mcpsimp"></a>

```
rowLen = m;                 // m = 4
colLen = n;                 // n = 8
//输入srcLocal的shape为4*8，类型为int32_t，deqScaleLocal的shape为8，类型为float，预留临时空间
AscendC::AscendDequant(dstLocal, srcLocal, deqScaleLocal, {rowLen, colLen, deqScaleLocal.GetSize()});
```

结果示例如下：

```
输入数据(srcLocal) int32_t数据类型:
[ -8  5 -5 -7 -3 -8  3  6
   9  2 -5  0  0 -5 -7  0 
  -6  0 -2  3 -2 8   5  2 
   2  2 -4  5 -4  4 -8  3 ]

反量化参数deqScale float数据类型:  
[ 10.433567  10.765296   -30.694275   -65.47741    8.386527    -89.646194   65.11153    42.213394]

输出数据(dstLocal) float数据类型:  
[-83.46854      53.82648    153.47137    458.34186    -25.15958   717.16956    195.33458   253.28036 
 93.9021        21.530592   153.47137    -0.          0.          448.23096    -455.7807   0.    
 -62.601402     0.          61.38855     -196.43222   -16.773054  -717.16956   325.55762   84.42679 
 20.867134      21.530592   122.7771     -327.38705   -33.54611   -358.58478   -520.8922   126.64018 ]
```

PER\_TOKEN/PER\_GROUP场景调用示例如下。

```
// 注意m,n需从外部传入
constexpr static bool isReuseSource = false;
constexpr static AscendDeQuantConfig config = {has_offset, -1};
constexpr static AscendDeQuantPolicy policy = AscendDeQuantPolicy::PER_TOKEN; // 可修改枚举值以使能PER_GROUP
AscendDeQuantParam para;
para.m = m;
para.n = n;
para.calCount = calCount;
AscendDequant<dstType, srcType, scaleType, config, policy>(dstLocal, srcLocal, scaleLocal, offsetLocal, para);
```

