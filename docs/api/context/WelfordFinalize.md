# WelfordFinalize<a name="ZH-CN_TOPIC_0000002041690584"></a>

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

## 功能说明<a name="section618mcpsimp"></a>

Welford计算是一种在线计算均值和方差的方法。一方面，它可以在不存储所有样本的情况下，逐步计算所有样本的均值和方差，更适合处理海量数据；另一方面，它只需要对数据进行一次遍历，能减少访存次数，提高计算性能。本接口为Welford算法的后处理。

LayerNorm算法中Reduce轴较大的场景，可以通过切分Reduce轴，联合使用本接口与[WelfordUpdate](WelfordUpdate.md)，能够实现等效计算LayerNorm。根据Reduce轴切分后是否有尾块，本接口分为如下两种计算公式：

-   不带尾块/不带counts参数场景：

    ![](figures/zh-cn_formulaimage_0000002047308822.png)

    -   方差系数未修正场景：

        ![](figures/zh-cn_formulaimage_0000002188346134.png)

    -   方差系数修正场景：

        ![](figures/zh-cn_formulaimage_0000002223917377.png)

    其中，Mean为均值输出，Var为方差输出。

    Mean<sub>i</sub>代表输入的第i个均值，Var<sub>i</sub>代表输入的第i个方差。Ab代表Reduce轴切分后一次计算的大小，Rn代表Reduce轴按Ab拆分的次数，![](figures/zh-cn_formulaimage_0000002235200305.png)代表未修正的方差系数rRec，![](figures/zh-cn_image_0000002200279866.png)代表修正方差系数rRecWithCorrection。

-   带尾块/带counts参数场景：

    ![](figures/zh-cn_formulaimage_0000002047309002.png)

    -   方差系数未修正场景：

        ![](figures/zh-cn_formulaimage_0000002223802977.png)

    -   方差系数修正场景：

        ![](figures/zh-cn_formulaimage_0000002223917381.png)

    除上述参数含义外，counts<sub>i</sub>代表Mean<sub>i</sub>对应的系数，R代表未切分的原始Reduce轴长度，![](figures/zh-cn_formulaimage_0000002200133484.png)代表未修正的方差系数rRec，![](figures/zh-cn_image_0000002200279878.png)代表修正系数rRecWithCorrection。

## 函数原型<a name="section620mcpsimp"></a>

-   通过sharedTmpBuffer入参传入临时空间
    -   不带counts参数场景

        ```
        template <bool isReuseSource = false, const WelfordFinalizeConfig& config = WFFINALIZE_DEFAULT_CFG>
        __aicore__ inline void WelfordFinalize(const LocalTensor<float>& outputMean, const LocalTensor<float>& outputVariance, const LocalTensor<float>& inputMean, const LocalTensor<float>& inputVariance, const LocalTensor<uint8_t>& sharedTmpBuffer, WelfordFinalizePara& para)
        ```

    -   带counts参数场景

        ```
        template <bool isReuseSource = false, const WelfordFinalizeConfig& config = WFFINALIZE_DEFAULT_CFG>
        __aicore__ inline void WelfordFinalize(const LocalTensor<float>& outputMean, const LocalTensor<float>& outputVariance, const LocalTensor<float>& inputMean, const LocalTensor<float>& inputVariance, const LocalTensor<int32_t>& counts, const LocalTensor<uint8_t>& sharedTmpBuffer, WelfordFinalizePara& para)
        ```

-   接口框架申请临时空间
    -   不带counts参数场景

        ```
        template <bool isReuseSource = false, const WelfordFinalizeConfig& config = WFFINALIZE_DEFAULT_CFG>
        __aicore__ inline void WelfordFinalize(const LocalTensor<float>& outputMean, const LocalTensor<float>& outputVariance, const LocalTensor<float>& inputMean, const LocalTensor<float>& inputVariance, WelfordFinalizePara& para)
        ```

    -   带counts参数场景

        ```
        template <bool isReuseSource = false, const WelfordFinalizeConfig& config = WFFINALIZE_DEFAULT_CFG>
        __aicore__ inline void WelfordFinalize(const LocalTensor<float>& outputMean, const LocalTensor<float>& outputVariance, const LocalTensor<float>& inputMean, const LocalTensor<float>& inputVariance, const LocalTensor<int32_t>& counts, WelfordFinalizePara& para)
        ```

由于该接口的内部实现中涉及复杂的计算，需要额外的临时空间来存储计算过程中的中间变量。临时空间支持**接口框架申请**和开发者**通过sharedTmpBuffer入参传入**两种方式。

-   接口框架申请临时空间，开发者无需申请，但是需要预留临时空间的大小。

-   通过sharedTmpBuffer入参传入，使用该tensor作为临时空间进行处理，接口框架不再申请。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。

接口框架申请的方式，开发者需要预留临时空间；通过sharedTmpBuffer传入的情况，开发者需要为tensor申请空间。临时空间大小BufferSize的获取方式如下：通过[WelfordFinalize Tiling](WelfordFinalize-Tiling.md)中提供的GetWelfordFinalizeMaxMinTmpSize接口获取所需最大和最小临时空间大小，最小空间可以保证功能正确，最大空间用于提升性能。

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table575571914269"></a>
<table><thead align="left"><tr id="row18755131942614"><th class="cellrowborder" valign="top" width="19.39%" id="mcps1.2.3.1.1"><p id="p675519193268"><a name="p675519193268"></a><a name="p675519193268"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.61%" id="mcps1.2.3.1.2"><p id="p375511918267"><a name="p375511918267"></a><a name="p375511918267"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row9756719122620"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p1682112447268"><a name="p1682112447268"></a><a name="p1682112447268"></a>isReuseSource</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p175786163713"><a name="p175786163713"></a><a name="p175786163713"></a>该参数预留，传入默认值false即可。</p>
</td>
</tr>
<tr id="row726932710267"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p92691827112616"><a name="p92691827112616"></a><a name="p92691827112616"></a>config</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p145341387272"><a name="p145341387272"></a><a name="p145341387272"></a>结构体模板参数，用于配置相关信息，WelfordFinalizeConfig类型，具体定义如下：</p>
<a name="screen55453174426"></a><a name="screen55453174426"></a><pre class="screen" codetype="Cpp" id="screen55453174426">struct WelfordFinalizeConfig {
     bool isCorrection = false;
}</pre>
<a name="ul13824145153916"></a><a name="ul13824145153916"></a><ul id="ul13824145153916"><li>isCorrection：计算方差时，是否使用修正系数，取值如下：<a name="ul58395710396"></a><a name="ul58395710396"></a><ul id="ul58395710396"><li>false：不使用修正系数，即方差系数为rRec。</li><li>true：使用修正系数rRecWithCorrection。</li></ul>
</li></ul>
<p id="p75171231133714"><a name="p75171231133714"></a><a name="p75171231133714"></a>配置示例如下。</p>
<a name="screen14584144314375"></a><a name="screen14584144314375"></a><pre class="screen" codetype="Cpp" id="screen14584144314375">constexpr WelfordFinalizeConfig WFFINALIZE_DEFAULT_CFG = { false };</pre>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="17.77%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="9.65%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.58%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row42461942101815"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p112283556514"><a name="p112283556514"></a><a name="p112283556514"></a>outputMean</p>
</td>
<td class="cellrowborder" valign="top" width="9.65%" headers="mcps1.2.4.1.2 "><p id="p186293346150"><a name="p186293346150"></a><a name="p186293346150"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.58%" headers="mcps1.2.4.1.3 "><p id="p242825624218"><a name="p242825624218"></a><a name="p242825624218"></a>均值目的操作数，数据类型为float。<span>输出的均值为1个数，需要sizeof(float)大小的空间进行保存，根据</span><a href="通用说明和约束.md#table16278354141117">存储单元的对齐要求</a><span>，开发者实际需要为outputMean分配32字节对齐的内存空间。</span></p>
<p id="p16911647191712"><a name="p16911647191712"></a><a name="p16911647191712"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row588411475110"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p123181843195412"><a name="p123181843195412"></a><a name="p123181843195412"></a>outputVariance</p>
</td>
<td class="cellrowborder" valign="top" width="9.65%" headers="mcps1.2.4.1.2 "><p id="p1286018337576"><a name="p1286018337576"></a><a name="p1286018337576"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.58%" headers="mcps1.2.4.1.3 "><p id="p1984317542572"><a name="p1984317542572"></a><a name="p1984317542572"></a>方差目的操作数，数据类型为float。<span>输出的方差为1个数，需要sizeof(float)大小的空间进行保存，根据</span><a href="通用说明和约束.md#table16278354141117">存储单元的对齐要求</a><span>，开发者实际需要为outputVariance分配32字节对齐的内存空间。</span></p>
<p id="p1384305465711"><a name="p1384305465711"></a><a name="p1384305465711"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row250163914199"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p493856135417"><a name="p493856135417"></a><a name="p493856135417"></a>inputMean</p>
</td>
<td class="cellrowborder" valign="top" width="9.65%" headers="mcps1.2.4.1.2 "><p id="p1662903414157"><a name="p1662903414157"></a><a name="p1662903414157"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.58%" headers="mcps1.2.4.1.3 "><p id="p19916922165715"><a name="p19916922165715"></a><a name="p19916922165715"></a>均值源操作数，数据类型为float。shape为[abLength]。</p>
<p id="p15450144034510"><a name="p15450144034510"></a><a name="p15450144034510"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_2"><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_2"><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_2"><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row81221355101812"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p732415106555"><a name="p732415106555"></a><a name="p732415106555"></a>inputVariance</p>
</td>
<td class="cellrowborder" valign="top" width="9.65%" headers="mcps1.2.4.1.2 "><p id="p121236558181"><a name="p121236558181"></a><a name="p121236558181"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.58%" headers="mcps1.2.4.1.3 "><p id="p1690619370586"><a name="p1690619370586"></a><a name="p1690619370586"></a>方差源操作数，数据类型为float。shape为[abLength]。</p>
<p id="p12238353135818"><a name="p12238353135818"></a><a name="p12238353135818"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_3"><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_3"><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_3"><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row15733201320551"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p4942133420395"><a name="p4942133420395"></a><a name="p4942133420395"></a>counts</p>
</td>
<td class="cellrowborder" valign="top" width="9.65%" headers="mcps1.2.4.1.2 "><p id="p157331713195513"><a name="p157331713195513"></a><a name="p157331713195513"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.58%" headers="mcps1.2.4.1.3 "><p id="p0129134165817"><a name="p0129134165817"></a><a name="p0129134165817"></a>源操作数，数据类型为int32_t。shape为[abLength]。</p>
<p id="p12243135545816"><a name="p12243135545816"></a><a name="p12243135545816"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_4"><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_4"><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_4"><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row20749938181910"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p1575033814199"><a name="p1575033814199"></a><a name="p1575033814199"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="9.65%" headers="mcps1.2.4.1.2 "><p id="p77501738191912"><a name="p77501738191912"></a><a name="p77501738191912"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.58%" headers="mcps1.2.4.1.3 "><p id="p1323018409190"><a name="p1323018409190"></a><a name="p1323018409190"></a>临时空间，数据类型为uint8_t。</p>
<p id="p14203184218188"><a name="p14203184218188"></a><a name="p14203184218188"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_5"><a name="zh-cn_topic_0000001530181537_ph173308471594_5"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_5"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_5"><a name="zh-cn_topic_0000001530181537_ph9902231466_5"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_5"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_5"><a name="zh-cn_topic_0000001530181537_ph1782115034816_5"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_5"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p104071111204211"><a name="p104071111204211"></a><a name="p104071111204211"></a>接口内部复杂计算时用于存储中间变量，由开发者提供。</p>
<p id="p2230154015198"><a name="p2230154015198"></a><a name="p2230154015198"></a>临时空间大小BufferSize的获取方式请参考<a href="WelfordFinalize-Tiling.md">WelfordFinalize Tiling</a>。</p>
</td>
</tr>
<tr id="row041413392198"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p054917245550"><a name="p054917245550"></a><a name="p054917245550"></a>para</p>
</td>
<td class="cellrowborder" valign="top" width="9.65%" headers="mcps1.2.4.1.2 "><p id="p263018345154"><a name="p263018345154"></a><a name="p263018345154"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.58%" headers="mcps1.2.4.1.3 "><p id="p0534135117111"><a name="p0534135117111"></a><a name="p0534135117111"></a>计算所需的参数信息。WelfordFinalizePara类型，定义如下。</p>
<a name="screen1411934411516"></a><a name="screen1411934411516"></a><pre class="screen" codetype="Cpp" id="screen1411934411516">struct WelfordFinalizePara {
    uint32_t rnLength;
    uint32_t abLength;
    uint32_t headCount;
    uint32_t headCountLength;
    uint32_t tailCount;
    uint32_t tailCountLength;
    float abRec;
    float rRec;
    float rRecWithCorrection;
};</pre>
<a name="ul20925141115211"></a><a name="ul20925141115211"></a><ul id="ul20925141115211"><li>rnLength：输入的Reduce轴，按abLength为一次计算的大小，拆分的次数。如果拆分后有尾块，则次数向上取整。</li><li>abLength：Reduce轴拆分的大小。在不带counts参数的接口中，abLength=headCountLength+tailCountLength。</li><li>headCount：在不带counts参数的接口中使能该参数，作为公式中非尾块的counts系数，headCount值。</li><li>headCountLength：在不带counts参数的接口中使能该参数，headCount值对应的长度。</li><li>tailCount：在不带counts参数的接口中使能该参数，作为公式中尾块的counts系数，tailCount值。</li><li>tailCountLength：在不带counts参数的接口中使能该参数，tailCount值对应的长度。</li><li>abRec：abLength的倒数，即为1/abLength的值。</li><li>rRec：输入的Reduce轴拆分后，若没有尾块，表示1/(rnLength*abLength)的值，若有尾块，表示1/R的值。</li><li>rRecWithCorrection：输入的方差修正系数，当模板参数config中的isCorrection为true时生效。</li></ul>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   接口参数para.abLength的取值必须为32/sizeof\(float\)的整数倍。
-   接口参数para.headCountLength与para.tailCountLength的和必须等于参数para.abLength。
-   接口处理逻辑以参数para中设置的具体参数值为准，不依赖源操作数的shape信息。
-   接口参数para.tailCount为0时，禁止配置para.tailCountLength为非0值。
-   不支持源操作数与目的操作数地址重叠。
-   不支持sharedTmpBuffer与源操作数和目的操作数地址重叠。

## 调用示例<a name="section94691236101419"></a>

```
pipe.InitBuffer(sharedTmpBuffer, stackBufferSize);        
AscendC::LocalTensor<uint8_t> tmpLocalTensor = sharedTmpBuffer.Get<uint8_t>();         
struct AscendC::WelfordFinalizePara para = {rnLength, abLength, head, headLength, tail, tailLength, abRec, rRec};
AscendC::WelfordFinalize<false>(meanLocal, varianceLocal, inputMeanLocal, inputVarianceLocal, inputCountsLocal, tmpLocalTensor, para); 
```

```
#include "kernel_operator.h"

template <typename dataType, bool isCounts = false> class KernelWelfordFinalize {
public:
    __aicore__ inline KernelWelfordFinalize() {}
    __aicore__ inline void Init(GM_ADDR inputX_gm, GM_ADDR inputmean_gm, GM_ADDR inputvar_gm, GM_ADDR outputMean_gm,
        GM_ADDR outputVariance_gm, uint32_t rnLength, uint32_t abLength, uint32_t rLength, uint32_t head,
        uint32_t headLength, uint32_t tail, uint32_t tailLength, float rRecWithCorrection)
    {
        this->rnLength = rnLength;
        this->abLength = abLength;
        this->head = head;
        this->headLength = headLength;
        this->tail = tail;
        this->tailLength = tailLength;

        totalLength = rnLength * abLength;
        this->rLength = rLength;
        this->abRec = 1 / (float)abLength;
        this->rRec = 1 / (float)rLength;
        this->rRecWithCorrection = rRecWithCorrection;
        this->outLength = (rnLength + 8 - 1) / 8 * 8;

        inputX_global.SetGlobalBuffer(reinterpret_cast<__gm__ dataType *>(inputX_gm), totalLength);
        inputmean_global.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(inputmean_gm), totalLength);
        inputvar_global.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(inputvar_gm), totalLength);

        outputMean_global.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(outputMean_gm), totalLength);
        outputVariance_global.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(outputVariance_gm), totalLength);

        pipe.InitBuffer(inQueueX, 1, sizeof(dataType) * totalLength);
        pipe.InitBuffer(inQueueMean, 1, sizeof(float) * totalLength);
        pipe.InitBuffer(inQueueVar, 1, sizeof(float) * totalLength);
        pipe.InitBuffer(outQueueMean, 1, sizeof(float) * outLength);
        pipe.InitBuffer(outQueueVariance, 1, sizeof(float) * outLength);
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
        AscendC::LocalTensor<dataType> inputXLocal = inQueueX.AllocTensor<dataType>();
        AscendC::LocalTensor<float> inmeanLocal = inQueueMean.AllocTensor<float>();
        AscendC::LocalTensor<float> invarLocal = inQueueVar.AllocTensor<float>();

        AscendC::DataCopy(inputXLocal, inputX_global, totalLength);
        AscendC::DataCopy(inmeanLocal, inputmean_global, totalLength);
        AscendC::DataCopy(invarLocal, inputvar_global, totalLength);

        inQueueX.EnQue(inputXLocal);
        inQueueMean.EnQue(inmeanLocal);
        inQueueVar.EnQue(invarLocal);
    }
    __aicore__ inline void Compute()
    {
        AscendC::LocalTensor<dataType> inputXLocal = inQueueX.DeQue<dataType>();
        AscendC::LocalTensor<float> inmeanLocal = inQueueMean.DeQue<float>();
        AscendC::LocalTensor<float> invarLocal = inQueueVar.DeQue<float>();

        AscendC::LocalTensor<float> meanLocal = outQueueMean.AllocTensor<float>();
        AscendC::LocalTensor<float> varianceLocal = outQueueVariance.AllocTensor<float>();
        AscendC::Duplicate(meanLocal, float(0), outLength);
        AscendC::Duplicate(varianceLocal, float(0), outLength);

        static constexpr AscendC::WelfordFinalizeConfig CONFIG = {false};
        AscendC::WelfordFinalizePara para = { rnLength, abLength, head, headLength, tail, tailLength, abRec, rRec, rRecWithCorrection};
        if constexpr (isCounts) {
            AscendC::WelfordFinalize<false, CONFIG>(meanLocal, varianceLocal, inmeanLocal, invarLocal, inputXLocal, para);
        } else {
            AscendC::WelfordFinalize<false, CONFIG>(meanLocal, varianceLocal, inmeanLocal, invarLocal, para);
        }

        outQueueMean.EnQue<float>(meanLocal);
        outQueueVariance.EnQue<float>(varianceLocal);

        inQueueX.FreeTensor(inputXLocal);
        inQueueMean.FreeTensor(inmeanLocal);
        inQueueVar.FreeTensor(invarLocal);
    }
    __aicore__ inline void CopyOut()
    {
        AscendC::LocalTensor<float> meanLocal = outQueueMean.DeQue<float>();
        AscendC::LocalTensor<float> varianceLocal = outQueueVariance.DeQue<float>();

        AscendC::DataCopy(outputMean_global, meanLocal, outLength);
        AscendC::DataCopy(outputVariance_global, varianceLocal, outLength);

        outQueueMean.FreeTensor(meanLocal);
        outQueueVariance.FreeTensor(varianceLocal);
    }

private:
    AscendC::GlobalTensor<dataType> inputX_global;
    AscendC::GlobalTensor<float> inputmean_global;
    AscendC::GlobalTensor<float> inputvar_global;
    AscendC::GlobalTensor<float> outputMean_global;
    AscendC::GlobalTensor<float> outputVariance_global;

    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueX;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueMean;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueVar;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueMean;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueVariance;

    uint32_t rnLength;
    uint32_t abLength;
    uint32_t rLength;
    uint32_t head;
    uint32_t headLength;
    uint32_t tail;
    uint32_t tailLength;
    uint32_t totalLength;
    uint32_t outLength;
    float abRec;
    float rRec;
    float rRecWithCorrection;
};
```

