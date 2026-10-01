# Normalize<a name="ZH-CN_TOPIC_0000002041690580"></a>

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

[LayerNorm](LayerNorm.md)中，已知均值和方差，计算shape为\[A，R\]的输入数据的标准差的倒数rstd和y，其计算公式如下：

![](figures/zh-cn_formulaimage_0000002046262966.png)

![](figures/zh-cn_formulaimage_0000002082381949.png)

其中，E和Var分别代表输入在R轴的均值，方差，γ为缩放系数，β为平移系数，ε为防除零的权重系数。

## 函数原型<a name="section620mcpsimp"></a>

-   通过sharedTmpBuffer入参传入临时空间

    ```
    template < typename U, typename T, bool isReuseSource = false, const NormalizeConfig& config = NLCFG_NORM>
    __aicore__ inline void Normalize(const LocalTensor<T>& output, const LocalTensor<float>& outputRstd, const LocalTensor<float>& inputMean, const LocalTensor<float>& inputVariance, const LocalTensor<T>& inputX, const LocalTensor<U>& gamma, const LocalTensor<U>& beta, const LocalTensor<uint8_t>& sharedTmpBuffer, const float epsilon, const NormalizePara& para)
    ```

-   接口框架申请临时空间

    ```
    template < typename U, typename T, bool isReuseSource = false, const NormalizeConfig& config = NLCFG_NORM>
    __aicore__ inline void Normalize(const LocalTensor<T>& output, const LocalTensor<float>& outputRstd, const LocalTensor<float>& inputMean, const LocalTensor<float>& inputVariance, const LocalTensor<T>& inputX, const LocalTensor<U>& gamma, const LocalTensor<U>& beta, const float epsilon, const NormalizePara& para)
    ```

由于该接口的内部实现中涉及复杂的计算，需要额外的临时空间来存储计算过程中的中间变量。临时空间支持**接口框架申请**和开发者**通过sharedTmpBuffer入参传入**两种方式。

-   接口框架申请临时空间，开发者无需申请，但是需要预留临时空间的大小。

-   通过sharedTmpBuffer入参传入，使用该tensor作为临时空间进行处理，接口框架不再申请。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。

接口框架申请的方式，开发者需要预留临时空间；通过sharedTmpBuffer传入的情况，开发者需要为tensor申请空间。临时空间大小BufferSize的获取方式如下：通过[Normalize Tiling](Normalize-Tiling.md)中提供的GetNormalizeMaxMinTmpSize接口获取所需最大和最小临时空间大小，最小空间可以保证功能正确，最大空间用于提升性能。

## 参数说明<a name="section1792963542116"></a>

**表 1**  模板参数说明

<a name="table128761218104510"></a>
<table><thead align="left"><tr id="row1787681864515"><th class="cellrowborder" valign="top" width="19.39%" id="mcps1.2.3.1.1"><p id="p1387651817453"><a name="p1387651817453"></a><a name="p1387651817453"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.61%" id="mcps1.2.3.1.2"><p id="p16876121816450"><a name="p16876121816450"></a><a name="p16876121816450"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row28761718174515"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p88760188458"><a name="p88760188458"></a><a name="p88760188458"></a>U</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p1876151810457"><a name="p1876151810457"></a><a name="p1876151810457"></a>beta，gamma操作数的数据类型。</p>
<p id="p1264818365429"><a name="p1264818365429"></a><a name="p1264818365429"></a><span id="ph9648123613427"><a name="ph9648123613427"></a><a name="ph9648123613427"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、bfloat16_t、float。</p>
<p id="p164863616422"><a name="p164863616422"></a><a name="p164863616422"></a><span id="ph8648036204214"><a name="ph8648036204214"></a><a name="ph8648036204214"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为: half、float。</p>
<p id="p66481936194217"><a name="p66481936194217"></a><a name="p66481936194217"></a><span id="ph1164883617422"><a name="ph1164883617422"></a><a name="ph1164883617422"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为: half、float。</p>
</td>
</tr>
<tr id="row91421942114514"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p0143154274510"><a name="p0143154274510"></a><a name="p0143154274510"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p214344224510"><a name="p214344224510"></a><a name="p214344224510"></a>output，inputX操作数的数据类型。</p>
<p id="p166031656114210"><a name="p166031656114210"></a><a name="p166031656114210"></a><span id="ph16038569426"><a name="ph16038569426"></a><a name="ph16038569426"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、bfloat16_t、float。</p>
<p id="p1760385654216"><a name="p1760385654216"></a><a name="p1760385654216"></a><span id="ph860325674213"><a name="ph860325674213"></a><a name="ph860325674213"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为: half、float。</p>
<p id="p16031556154219"><a name="p16031556154219"></a><a name="p16031556154219"></a><span id="ph116037567421"><a name="ph116037567421"></a><a name="ph116037567421"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为: half、float。</p>
</td>
</tr>
<tr id="row58761518134511"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p687611816458"><a name="p687611816458"></a><a name="p687611816458"></a>isReuseSource</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p196681591367"><a name="p196681591367"></a><a name="p196681591367"></a>该参数预留，传入默认值false即可。</p>
</td>
</tr>
<tr id="row1870819634812"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p127081060486"><a name="p127081060486"></a><a name="p127081060486"></a>config</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p202731227454"><a name="p202731227454"></a><a name="p202731227454"></a>配置Normalize接口中输入输出相关信息。NormalizeConfig类型，定义如下。</p>
<a name="screen8520133163317"></a><a name="screen8520133163317"></a><pre class="screen" codetype="Cpp" id="screen8520133163317">struct NormalizeConfig {
    ReducePattern reducePattern = ReducePattern::AR;
    int32_t aLength = -1;
    bool isNoBeta = false;
    bool isNoGamma = false;
    bool isOnlyOutput = false;
};</pre>
<a name="ul1167113259457"></a><a name="ul1167113259457"></a><ul id="ul1167113259457"><li>reducePattern：当前仅支持ReducePattern::AR模式，表示输入的内轴R轴为reduce计算轴。</li><li>aLength：用于描述输入的A轴大小。支持的取值如下：<a name="ul6718193712463"></a><a name="ul6718193712463"></a><ul id="ul6718193712463"><li>-1：默认值。取<a href="#table2087718184450">接口参数</a>para中的aLength作为A轴大小。</li><li>1：支持outputRstd数据非对齐搬出，支持inputMean，inputVariance数据非对齐搬入。aLength为其它取值时，不支持上述三个输入输出的非对齐搬入和非对齐搬出。该取值需要与<a href="#table2087718184450">接口参数</a>para中的aLength数值一致。</li><li>其它值：该值需要与<a href="#table2087718184450">接口参数</a>para中的aLength数值一致。</li></ul>
</li><li>isNoBeta：计算时，输入beta是否使用。<a name="ul11364174572711"></a><a name="ul11364174572711"></a><ul id="ul11364174572711"><li>false：默认值，Normalize计算中使用输入beta。</li><li>true：Normalize计算中不使用输入beta。此时，公式中与beta相关的计算被省略。</li></ul>
</li><li>isNoGamma：可选输入gamma是否使用。<a name="ul748573616312"></a><a name="ul748573616312"></a><ul id="ul748573616312"><li>false：默认值，Normalize计算中使用可选输入gamma。</li><li>true：Normalize计算中不使用输入gamma。此时，公式中与gamma相关的计算被省略。</li></ul>
</li><li>isOnlyOutput：是否只输出y，不输出标准差的倒数rstd。当前该参数仅支持取值为false，表示y和rstd的结果全部输出。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="table2087718184450"></a>
<table><thead align="left"><tr id="row1877161884515"><th class="cellrowborder" valign="top" width="19.621962196219624%" id="mcps1.2.4.1.1"><p id="p28771318104519"><a name="p28771318104519"></a><a name="p28771318104519"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="10.781078107810782%" id="mcps1.2.4.1.2"><p id="p587720189452"><a name="p587720189452"></a><a name="p587720189452"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="69.5969596959696%" id="mcps1.2.4.1.3"><p id="p13877191817454"><a name="p13877191817454"></a><a name="p13877191817454"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row68772018194518"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p0877131824517"><a name="p0877131824517"></a><a name="p0877131824517"></a>output</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p987741824515"><a name="p987741824515"></a><a name="p987741824515"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p178776184450"><a name="p178776184450"></a><a name="p178776184450"></a>目的操作数，shape为[A, R]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。</p>
<p id="p10380171663712"><a name="p10380171663712"></a><a name="p10380171663712"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1687714181456"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p88779186454"><a name="p88779186454"></a><a name="p88779186454"></a>outputRstd</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p1987741874510"><a name="p1987741874510"></a><a name="p1987741874510"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p14878121814452"><a name="p14878121814452"></a><a name="p14878121814452"></a>标准差的倒数，shape为[A]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。</p>
<p id="p1267171815374"><a name="p1267171815374"></a><a name="p1267171815374"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1387871834515"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p614744784112"><a name="p614744784112"></a><a name="p614744784112"></a>inputMean</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p214754714110"><a name="p214754714110"></a><a name="p214754714110"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p1214634719419"><a name="p1214634719419"></a><a name="p1214634719419"></a>均值，shape为[A]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。</p>
<p id="p1152731963712"><a name="p1152731963712"></a><a name="p1152731963712"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_2"><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_2"><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_2"><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row08781418134517"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p996817421415"><a name="p996817421415"></a><a name="p996817421415"></a>inputVariance</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p1297120794216"><a name="p1297120794216"></a><a name="p1297120794216"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p4653311114317"><a name="p4653311114317"></a><a name="p4653311114317"></a>方差，shape为[A]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。</p>
<p id="p18606320103712"><a name="p18606320103712"></a><a name="p18606320103712"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_3"><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_3"><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_3"><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row78781718104510"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p2053216459413"><a name="p2053216459413"></a><a name="p2053216459413"></a>inputX</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p1253244524110"><a name="p1253244524110"></a><a name="p1253244524110"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p1953264554116"><a name="p1953264554116"></a><a name="p1953264554116"></a>源操作数，shape为[A, R]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。inputX的数据类型需要与目的操作数保持一致，尾轴长度需要32B对齐。</p>
<p id="p798122111372"><a name="p798122111372"></a><a name="p798122111372"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_4"><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_4"><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_4"><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row14547131215418"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p11607173954118"><a name="p11607173954118"></a><a name="p11607173954118"></a>gamma</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p116074396413"><a name="p116074396413"></a><a name="p116074396413"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p14607203919417"><a name="p14607203919417"></a><a name="p14607203919417"></a>缩放系数，shape为[R]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。gamma的数据类型精度不低于源操作数的数据类型精度。</p>
<p id="p136010233375"><a name="p136010233375"></a><a name="p136010233375"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_5"><a name="zh-cn_topic_0000001530181537_ph173308471594_5"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_5"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_5"><a name="zh-cn_topic_0000001530181537_ph9902231466_5"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_5"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_5"><a name="zh-cn_topic_0000001530181537_ph1782115034816_5"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_5"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1421841014412"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p1260719399418"><a name="p1260719399418"></a><a name="p1260719399418"></a>beta</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p206071739164112"><a name="p206071739164112"></a><a name="p206071739164112"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p196071039104113"><a name="p196071039104113"></a><a name="p196071039104113"></a>平移系数，shape为[R]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。beta的数据类型精度不低于源操作数的数据类型精度。</p>
<p id="p346362419371"><a name="p346362419371"></a><a name="p346362419371"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_6"><a name="zh-cn_topic_0000001530181537_ph173308471594_6"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_6"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_6"><a name="zh-cn_topic_0000001530181537_ph9902231466_6"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_6"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_6"><a name="zh-cn_topic_0000001530181537_ph1782115034816_6"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_6"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row8879181894517"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p1187910186454"><a name="p1187910186454"></a><a name="p1187910186454"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p1487913183458"><a name="p1487913183458"></a><a name="p1487913183458"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p3879171874513"><a name="p3879171874513"></a><a name="p3879171874513"></a>共享缓冲区，用于存放API内部计算产生的临时数据。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。共享缓冲区大小的获取方式请参考<a href="Normalize-Tiling.md">Normalize Tiling</a>。</p>
<p id="p13879191812459"><a name="p13879191812459"></a><a name="p13879191812459"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_7"><a name="zh-cn_topic_0000001530181537_ph173308471594_7"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_7"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_7"><a name="zh-cn_topic_0000001530181537_ph9902231466_7"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_7"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_7"><a name="zh-cn_topic_0000001530181537_ph1782115034816_7"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_7"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row087919187455"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p198797185455"><a name="p198797185455"></a><a name="p198797185455"></a>epsilon</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p98791218164519"><a name="p98791218164519"></a><a name="p98791218164519"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p16879191884520"><a name="p16879191884520"></a><a name="p16879191884520"></a>防除零的权重系数。</p>
</td>
</tr>
<tr id="row11731132543214"><td class="cellrowborder" valign="top" width="19.621962196219624%" headers="mcps1.2.4.1.1 "><p id="p13731525123217"><a name="p13731525123217"></a><a name="p13731525123217"></a>para</p>
</td>
<td class="cellrowborder" valign="top" width="10.781078107810782%" headers="mcps1.2.4.1.2 "><p id="p1073152563216"><a name="p1073152563216"></a><a name="p1073152563216"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="69.5969596959696%" headers="mcps1.2.4.1.3 "><p id="p1945105915511"><a name="p1945105915511"></a><a name="p1945105915511"></a>Normalize计算所需的参数信息。NormalizePara类型，定义如下。</p>
<a name="screen150919104214"></a><a name="screen150919104214"></a><pre class="screen" codetype="Cpp" id="screen150919104214">struct NormalizePara {
    uint32_t aLength;
    uint32_t rLength;
    uint32_t rLengthWithPadding;
};</pre>
<a name="ul20925141115211"></a><a name="ul20925141115211"></a><ul id="ul20925141115211"><li>aLength：指定输入inputX的A轴长度。</li><li>rLength：指定输入inputX的R轴长度。</li><li>rLengthWithPadding：指定输入inputX的R轴对齐后的长度，该值是32B对齐的。</li></ul>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section2673867191"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。
-   缩放系数gamma和平移系数beta的数据类型精度必须不低于源操作数inputX的数据类型精度。比如，inputX的数据类型是half，gamma、beta的数据类型可以是half或者float，精度不低于inputX。比如，inputX的数据类型是bfloat16\_t，gamma、beta的数据类型可以是bfloat16\_t或者float，精度不低于inputX。
-   src和dst的Tensor空间不可以复用。
-   输入仅支持ND格式。
-   R轴不支持切分。

## 调用示例<a name="section94691236101419"></a>

```
#include "kernel_operator.h"
constexpr int32_t BUFFER_NUM = 1;  // tensor num for each queue

template <const AscendC::NormalizeConfig& CONFIG>
class KernelNormalize {
 public:
  __aicore__ inline KernelNormalize() {}

  __aicore__ inline void Init(GM_ADDR x, GM_ADDR mean, GM_ADDR variance, GM_ADDR gamma, GM_ADDR beta, GM_ADDR rstd, GM_ADDR y, const float epsilon, const AscendC::NormalizePara& para) {
    this->meanRstdSize = (para.aLength + 7) / 8 * 8;  // 此时进行32B对齐处理
    // get start index for current core, core parallel
    xGm.SetGlobalBuffer((__gm__ DTYPE_X*)x, para.aLength * para.rLengthWithPadding);
    meanGm.SetGlobalBuffer((__gm__ float*)mean, this->meanRstdSize);
    varianceGm.SetGlobalBuffer((__gm__ float*)variance, this->meanRstdSize);
    gammaGm.SetGlobalBuffer((__gm__ DTYPE_GAMMA*)gamma, para.rLengthWithPadding);
    betaGm.SetGlobalBuffer((__gm__ DTYPE_BETA*)beta, para.rLengthWithPadding);

    rstdGm.SetGlobalBuffer((__gm__ float*)rstd, this->meanRstdSize);
    yGm.SetGlobalBuffer((__gm__ DTYPE_Y*)y, para.aLength * para.rLengthWithPadding);

    // pipe alloc memory to queue, the unit is Bytes
    pipe.InitBuffer(inQueueX, BUFFER_NUM, para.aLength * para.rLengthWithPadding * sizeof(DTYPE_X));
    pipe.InitBuffer(inQueueMean, BUFFER_NUM, this->meanRstdSize * sizeof(float));
    pipe.InitBuffer(inQueueVariance, BUFFER_NUM, this->meanRstdSize * sizeof(float));
    pipe.InitBuffer(inQueueGamma, BUFFER_NUM, para.rLengthWithPadding * sizeof(DTYPE_GAMMA));
    pipe.InitBuffer(inQueueBeta, BUFFER_NUM, para.rLengthWithPadding * sizeof(DTYPE_BETA));

    pipe.InitBuffer(outQueueRstd, BUFFER_NUM, this->meanRstdSize * sizeof(float));
    pipe.InitBuffer(outQueueY, BUFFER_NUM, para.aLength * para.rLengthWithPadding * sizeof(DTYPE_Y));

    this->epsilon = epsilon;
    this->para = para;
  }

  __aicore__ inline void Compute() {
    AscendC::LocalTensor<DTYPE_X> xLocal = inQueueX.DeQue<DTYPE_X>();
    AscendC::LocalTensor<float> meanLocal = inQueueMean.DeQue<float>();
    AscendC::LocalTensor<float> varianceLocal = inQueueVariance.DeQue<float>();
    AscendC::LocalTensor<DTYPE_GAMMA> gammaLocal = inQueueGamma.DeQue<DTYPE_GAMMA>();
    AscendC::LocalTensor<DTYPE_BETA> betaLocal = inQueueBeta.DeQue<DTYPE_BETA>();

    AscendC::LocalTensor<float> rstdLocal = outQueueRstd.AllocTensor<float>();
    AscendC::LocalTensor<DTYPE_Y> yLocal = outQueueY.AllocTensor<DTYPE_Y>();

    AscendC::Duplicate(rstdLocal, (float)0, this->meanRstdSize);
    AscendC::Duplicate(yLocal, (DTYPE_Y)0, para.aLength * para.rLengthWithPadding);

    AscendC::Normalize<DTYPE_Y, DTYPE_X, false, CONFIG>(yLocal, rstdLocal, meanLocal, varianceLocal, xLocal, gammaLocal, betaLocal, epsilon, para);

    outQueueRstd.EnQue<float>(rstdLocal);
    outQueueY.EnQue<DTYPE_Y>(yLocal);
    inQueueX.FreeTensor(xLocal);
    inQueueMean.FreeTensor(meanLocal);
    inQueueVariance.FreeTensor(varianceLocal);
    inQueueGamma.FreeTensor(gammaLocal);
    inQueueBeta.FreeTensor(betaLocal);

  }
  __aicore__ inline void Process() {
    CopyIn();
    Compute();
    CopyOut();
  }

 private:
  __aicore__ inline void CopyIn() {
    // alloc tensor from queue memory
    AscendC::LocalTensor<DTYPE_X> xLocal = inQueueX.AllocTensor<DTYPE_X>();
    AscendC::LocalTensor<float> meanLocal = inQueueMean.AllocTensor<float>();
    AscendC::LocalTensor<float> varianceLocal = inQueueVariance.AllocTensor<float>();
    AscendC::LocalTensor<DTYPE_GAMMA> gammaLocal = inQueueGamma.AllocTensor<DTYPE_GAMMA>();
    AscendC::LocalTensor<DTYPE_BETA> betaLocal = inQueueBeta.AllocTensor<DTYPE_BETA>();
    // copy progress_th tile from global tensor to local tensor
    AscendC::DataCopy(xLocal, xGm, para.aLength * para.rLengthWithPadding);
    AscendC::DataCopy(meanLocal, meanGm, this->meanRstdSize);
    AscendC::DataCopy(varianceLocal, varianceGm, this->meanRstdSize);
    AscendC::DataCopy(gammaLocal, gammaGm, para.rLengthWithPadding);
    AscendC::DataCopy(betaLocal, betaGm, para.rLengthWithPadding);

    // enque input tensors to VECIN queue
    inQueueX.EnQue(xLocal);
    inQueueMean.EnQue(meanLocal);
    inQueueVariance.EnQue(varianceLocal);
    inQueueGamma.EnQue(gammaLocal);
    inQueueBeta.EnQue(betaLocal);
  }

  __aicore__ inline void CopyOut() {
    // deque output tensor from VECOUT queue
    AscendC::LocalTensor<float> rstdLocal = outQueueRstd.DeQue<float>();
    AscendC::LocalTensor<DTYPE_Y> yLocal = outQueueY.DeQue<DTYPE_Y>();
    // copy progress_th tile from local tensor to global tensor
    AscendC::DataCopy(rstdGm, rstdLocal, this->meanRstdSize);
    AscendC::DataCopy(yGm, yLocal, para.aLength * para.rLengthWithPadding);
    // free output tensor for reuse
    outQueueRstd.FreeTensor(rstdLocal);
    outQueueY.FreeTensor(yLocal);
  }

 private:
  AscendC::TPipe pipe;
  // create queues for input, in this case depth is equal to buffer num
  AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueX;
  AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueMean;
  AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueVariance;
  AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueGamma;
  AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueBeta;
  // create queue for output, in this case depth is equal to buffer num
  AscendC::TQue<AscendC::TPosition::VECOUT, BUFFER_NUM> outQueueRstd;
  AscendC::TQue<AscendC::TPosition::VECOUT, BUFFER_NUM> outQueueY;

  AscendC::GlobalTensor<float> meanGm;
  AscendC::GlobalTensor<float> varianceGm;
  AscendC::GlobalTensor<DTYPE_X> xGm;
  AscendC::GlobalTensor<DTYPE_GAMMA> gammaGm;
  AscendC::GlobalTensor<DTYPE_BETA> betaGm;

  AscendC::GlobalTensor<float> rstdGm;
  AscendC::GlobalTensor<DTYPE_Y> yGm;

  float epsilon;
  uint32_t meanRstdSize;
  AscendC::NormalizePara para;
};
__aicore__ constexpr AscendC::NormalizeConfig GenConfig(bool isNoBeta, bool isNoGamma)
{
    return {.reducePattern = AscendC::ReducePattern::AR,
        .aLength = -1,
        .isNoBeta = isNoBeta,
        .isNoGamma = isNoGamma,
        .isOnlyOutput = false};
}
// with beta and gamma
constexpr AscendC::NormalizeConfig CONFIG1 = GenConfig(false, false);
constexpr AscendC::NormalizeConfig CONFIG2 = GenConfig(false, true);
constexpr AscendC::NormalizeConfig CONFIG3 = GenConfig(true, false);
constexpr AscendC::NormalizeConfig CONFIG4 = GenConfig(true, true);

extern "C" __global__ __aicore__ void normalize_custom(GM_ADDR x, GM_ADDR mean, GM_ADDR variance, GM_ADDR gamma, GM_ADDR beta, GM_ADDR rstd, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tilingData, tiling);
    float epsilon = tilingData.epsilon;
    AscendC::NormalizePara para(tilingData.aLength, tilingData.rLength, tilingData.rLengthWithPadding);
    if (TILING_KEY_IS(1)) {
      if (!tilingData.isNoBeta && !tilingData.isNoGamma) {
          KernelNormalize<CONFIG1> op;
          op.Init(x, mean, variance, gamma, beta, rstd, y, epsilon, para);
          op.Process();
      } else if (!tilingData.isNoBeta && tilingData.isNoGamma) {
          KernelNormalize<CONFIG2> op;
          op.Init(x, mean, variance, gamma, beta, rstd, y, epsilon, para);
          op.Process();
      } else if (tilingData.isNoBeta && !tilingData.isNoGamma) {
          KernelNormalize<CONFIG3> op;
          op.Init(x, mean, variance, gamma, beta, rstd, y, epsilon, para);
          op.Process();
      } else if (tilingData.isNoBeta && tilingData.isNoGamma) {
          KernelNormalize<CONFIG4> op;
          op.Init(x, mean, variance, gamma, beta, rstd, y, epsilon, para);
          op.Process();
      }
    }
  }
```

