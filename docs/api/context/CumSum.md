# CumSum<a name="ZH-CN_TOPIC_0000001892493642"></a>

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
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7751240111217"><a name="p7751240111217"></a><a name="p7751240111217"></a>√</p>
</td>
</tr>
<tr id="row33481333123"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p2034813321217"><a name="p2034813321217"></a><a name="p2034813321217"></a><span id="ph334833191213"><a name="ph334833191213"></a><a name="ph334833191213"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p20751740131216"><a name="p20751740131216"></a><a name="p20751740131216"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

用于对输入张量按行或列进行累加和操作，输出结果中每个元素都是输入张量中对应位置及之前所有行或列的元素累加和。

计算公式如下：

![](figures/zh-cn_formulaimage_0000002340790900.png)

-   逐行累加算法
    -   First轴处理，按行累加和操作，即第一行不变，后面的行依次累加，输出结果的第i行第j列计算公式如下：

        ![](figures/zh-cn_formulaimage_0000002223791509.png)

        以tensor\(\[\[0, 1, 2\], \[3, 4, 5\]\]\)为例，输出结果是tensor\(\[\[0, 1, 2\], \[3, 5, 7\]\]\)

    -   Last轴处理，按列累加和操作，即第一列不变，后面的列依次累加，输出结果的第i行第j列计算公式如下：

        ![](figures/zh-cn_formulaimage_0000002223797889.png)

        以tensor\(\[\[0, 1, 2\], \[3, 4, 5\]\]\)为例，输出结果是tensor\(\[\[0, 1, 3\], \[3, 7, 12\]\]\)

-   Sklansky二分累加算法

    Sklansky二分累加算法是基于Sklansky Adder的并行前缀和逻辑实现的。[图1](#fig45153311379)为一维二进制的并行前缀和算法示意图。将该算法扩展至二维张量的累加和算法，以按行累加为例，[图2](#fig7422174293712)为该算法的执行步骤示意图，通过并行计算多行的加和，实现Sklansky二分累加算法下的按行累加和。

    **图 1**  Sklansky Adder算法示意图<a name="fig45153311379"></a>  
    ![](figures/Sklansky-Adder算法示意图.png "Sklansky-Adder算法示意图")

    **图 2**  基于Sklansky的二分累加示意图<a name="fig7422174293712"></a>  
    ![](figures/基于Sklansky的二分累加示意图.png "基于Sklansky的二分累加示意图")

## 函数原型<a name="section620mcpsimp"></a>

-   通过sharedTmpBuffer入参传入临时空间

    ```
    template <typename T, const CumSumConfig& config = defaultCumSumConfig>
    __aicore__ inline void CumSum(LocalTensor<T>& dstTensor, LocalTensor<T>& lastRowTensor, const LocalTensor<T>& srcTensor, LocalTensor<uint8_t>& sharedTmpBuffer, const CumSumInfo& cumSumInfo)
    ```

-   接口框架申请临时空间

    ```
    template <typename T, const CumSumConfig& config = defaultCumSumConfig>
    __aicore__ inline void CumSum(LocalTensor<T>& dstTensor, LocalTensor<T>& lastRowTensor, const LocalTensor<T>& srcTensor, const CumSumInfo& cumSumInfo)
    ```

由于该接口的内部实现中涉及精度转换。需要额外的临时空间来存储计算过程中的中间变量。临时空间支持**接口框架申请**和开发者**通过sharedTmpBuffer入参传入**两种方式。

-   接口框架申请临时空间，开发者无需申请，但是需要预留临时空间的大小。

-   通过sharedTmpBuffer入参传入，使用该tensor作为临时空间进行处理，接口框架不再申请。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。

接口框架申请的方式，开发者需要预留临时空间；通过sharedTmpBuffer传入的情况，开发者需要为tensor申请空间。临时空间大小BufferSize的获取方式如下：通过[GetCumSumMaxMinTmpSize](GetCumSumMaxMinTmpSize.md)中提供的接口获取需要预留空间的大小。

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table575571914269"></a>
<table><thead align="left"><tr id="row18755131942614"><th class="cellrowborder" valign="top" width="19.39%" id="mcps1.2.3.1.1"><p id="p675519193268"><a name="p675519193268"></a><a name="p675519193268"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.61%" id="mcps1.2.3.1.2"><p id="p375511918267"><a name="p375511918267"></a><a name="p375511918267"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14755141911264"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p47551198266"><a name="p47551198266"></a><a name="p47551198266"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p125969172719"><a name="p125969172719"></a><a name="p125969172719"></a>操作数的数据类型。</p>
<p id="p1470938145713"><a name="p1470938145713"></a><a name="p1470938145713"></a><span id="ph10709148195710"><a name="ph10709148195710"></a><a name="ph10709148195710"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、float。</p>
<p id="p77087865715"><a name="p77087865715"></a><a name="p77087865715"></a><span id="ph11708118165712"><a name="ph11708118165712"></a><a name="ph11708118165712"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half、float。</p>
<p id="p14708128115717"><a name="p14708128115717"></a><a name="p14708128115717"></a><span id="ph207091987579"><a name="ph207091987579"></a><a name="ph207091987579"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half、float。</p>
</td>
</tr>
<tr id="row431312153917"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p53131021173917"><a name="p53131021173917"></a><a name="p53131021173917"></a>config</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p51531559114315"><a name="p51531559114315"></a><a name="p51531559114315"></a>定义CumSum接口编译时config参数。CumSumConfig类型，具体定义如下方代码所示，其中参数的含义为：</p>
<p id="p168881610207"><a name="p168881610207"></a><a name="p168881610207"></a>isLastAxis：取值为true表示计算按last轴处理，取值为false表示计算按first轴处理；</p>
<p id="p166881716202015"><a name="p166881716202015"></a><a name="p166881716202015"></a>isReuseSource：是否可以复用srcTensor的内存空间；该参数预留，传入默认值false即可。</p>
<p id="p068821614209"><a name="p068821614209"></a><a name="p068821614209"></a>outputLastRow：是否输出最后一行数据。</p>
<div class="p" id="p06882168202"><a name="p06882168202"></a><a name="p06882168202"></a>algorithm：CumSum内部实现使用的累加和算法，该参数支持的取值如下：<a name="ul107741953144114"></a><a name="ul107741953144114"></a><ul id="ul107741953144114"><li>CumSumAlgorithm::CUMSUM_ALGORITHM_LINEBYLINE：逐行累加算法。</li><li>CumSumAlgorithm::CUMSUM_ALGORITHM_SKLANSKY：Sklansky二分累加算法。</li></ul>
</div>
</td>
</tr>
</tbody>
</table>

```
struct CumSumConfig {
    bool isLastAxis{true};
    bool isReuseSource{false};
    bool outputLastRow{false};
    CumSumAlgorithm algorithm{CumSumAlgorithm::CUMSUM_ALGORITHM_LINEBYLINE};
};
enum class CumSumAlgorithm {    
    CUMSUM_ALGORITHM_LINEBYLINE = 0,    
    CUMSUM_ALGORITHM_SKLANSKY = 1
};
```

**表 2**  接口参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="17.77%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="9.68%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.55%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row42461942101815"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p462911347151"><a name="p462911347151"></a><a name="p462911347151"></a>dstTensor</p>
</td>
<td class="cellrowborder" valign="top" width="9.68%" headers="mcps1.2.4.1.2 "><p id="p186293346150"><a name="p186293346150"></a><a name="p186293346150"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.55%" headers="mcps1.2.4.1.3 "><p id="p125181336588"><a name="p125181336588"></a><a name="p125181336588"></a>目的操作数。按first轴或last轴处理，输入元素的累加和。</p>
<p id="p896183233913"><a name="p896183233913"></a><a name="p896183233913"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row17371444131520"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p1838644151511"><a name="p1838644151511"></a><a name="p1838644151511"></a>lastRowTensor</p>
</td>
<td class="cellrowborder" valign="top" width="9.68%" headers="mcps1.2.4.1.2 "><p id="p73844410158"><a name="p73844410158"></a><a name="p73844410158"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.55%" headers="mcps1.2.4.1.3 "><p id="p3707144233915"><a name="p3707144233915"></a><a name="p3707144233915"></a>目的操作数。模板参数config中的outputLastRow参数取值为true时，输出的最后一行数据。</p>
<p id="p261714443395"><a name="p261714443395"></a><a name="p261714443395"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row947414514416"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p14571758135918"><a name="p14571758135918"></a><a name="p14571758135918"></a>srcTensor</p>
</td>
<td class="cellrowborder" valign="top" width="9.68%" headers="mcps1.2.4.1.2 "><p id="p1757114583596"><a name="p1757114583596"></a><a name="p1757114583596"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.55%" headers="mcps1.2.4.1.3 "><p id="p1757117589594"><a name="p1757117589594"></a><a name="p1757117589594"></a>源操作数。</p>
<p id="p20571165811599"><a name="p20571165811599"></a><a name="p20571165811599"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_2"><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_2"><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_2"><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row2011532061817"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p144696451846"><a name="p144696451846"></a><a name="p144696451846"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="9.68%" headers="mcps1.2.4.1.2 "><p id="p4469745640"><a name="p4469745640"></a><a name="p4469745640"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.55%" headers="mcps1.2.4.1.3 "><p id="p1148552183919"><a name="p1148552183919"></a><a name="p1148552183919"></a>临时缓存。</p>
<p id="p5983205412394"><a name="p5983205412394"></a><a name="p5983205412394"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_3"><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_3"><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_3"><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p104071111204211"><a name="p104071111204211"></a><a name="p104071111204211"></a>用于CumSum内部复杂计算时存储中间变量，由开发者提供。</p>
<p id="p951916063516"><a name="p951916063516"></a><a name="p951916063516"></a>临时空间大小BufferSize的获取方式请参考<a href="GetCumSumMaxMinTmpSize.md">GetCumSumMaxMinTmpSize</a>。</p>
</td>
</tr>
<tr id="row134741451043"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p187691656719"><a name="p187691656719"></a><a name="p187691656719"></a>cumSumInfo</p>
</td>
<td class="cellrowborder" valign="top" width="9.68%" headers="mcps1.2.4.1.2 "><p id="p64685451843"><a name="p64685451843"></a><a name="p64685451843"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.55%" headers="mcps1.2.4.1.3 "><p id="p1468912433196"><a name="p1468912433196"></a><a name="p1468912433196"></a>srcTensor的shape信息。CumSumInfo类型，具体定义如下方代码所示，其中参数的含义为：</p>
<p id="p4682442142115"><a name="p4682442142115"></a><a name="p4682442142115"></a>outter：表示输入数据的外轴长度。</p>
<p id="p1690924942114"><a name="p1690924942114"></a><a name="p1690924942114"></a>inner：表示输入数据的内轴长度。</p>
<p id="p1118312311316"><a name="p1118312311316"></a><a name="p1118312311316"></a>请注意：</p>
<p id="p382219882114"><a name="p382219882114"></a><a name="p382219882114"></a>cumSumInfo.outter和cumSumInfo.inner都应大于0。</p>
<p id="p1482218112117"><a name="p1482218112117"></a><a name="p1482218112117"></a>cumSumInfo.outter * cumSumInfo.inner不能大于dstTensor或srcTensor的大小。</p>
<p id="p182211832119"><a name="p182211832119"></a><a name="p182211832119"></a>cumSumInfo.inner * sizeof(T)必须是32字节的整数倍。</p>
<p id="p4823198102112"><a name="p4823198102112"></a><a name="p4823198102112"></a>当模板参数config中的outputLastRow取值为true时，cumSumInfo.inner不能大于lastRowTensor输出的最后一行数据的大小。</p>
</td>
</tr>
</tbody>
</table>

```
struct CumSumInfo
{
    uint32_t outter{0};
    uint32_t inner{0};
};
```

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section18375195021515"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。
-   输入input只支持二维结构。
-   cumSumInfo.inner \* sizeof\(T\)必须是32字节的整数倍。

## 调用示例<a name="section94691236101419"></a>

```
#include "kernel_operator.h"

template <typename T, const CumSumConfig& CONFIG>
class KernelCumSum
{
public:
    __aicore__ inline KernelCumSum(){}
    __aicore__ inline void Init(
        GM_ADDR srcGm, GM_ADDR dstGm, GM_ADDR lastRowGm, const AscendC::CumSumInfo& cumSumParams)
    {
        outer = cumSumParams.outter;
        inner = cumSumParams.inner;
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T *>(srcGm), outer * inner);
        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T *>(dstGm), outer * inner);
        lastRowGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T *>(lastRowGm), inner);
        pipe.InitBuffer(inQueueX, 1, outer * inner * sizeof(T));
        pipe.InitBuffer(outQueue, 1, outer * inner * sizeof(T));
        pipe.InitBuffer(lastRowQueue, 1, inner * sizeof(T));
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
        AscendC::LocalTensor<T> srcLocal = inQueueX.AllocTensor<T>();
        AscendC::DataCopy(srcLocal, srcGlobal, outer * inner);
        inQueueX.EnQue(srcLocal);
    }
    __aicore__ inline void Compute()
    {
        AscendC::LocalTensor<T> dstLocal = outQueue.AllocTensor<T>();
        AscendC::LocalTensor<T> lastRowLocal = lastRowQueue.AllocTensor<T>();
        AscendC::LocalTensor<T> srcLocal = inQueueX.DeQue<T>();
        
        const AscendC::CumSumInfo cumSumInfo{outer, inner};
        AscendC::CumSum<T, CONFIG>(dstLocal, lastRowLocal, srcLocal, cumSumInfo);
        outQueue.EnQue<T>(dstLocal);
        lastRowQueue.EnQue<T>(lastRowLocal);
        inQueueX.FreeTensor(srcLocal);
    }
    __aicore__ inline void CopyOut()
    {
        AscendC::LocalTensor<T> dstLocal = outQueue.DeQue<T>();
        AscendC::DataCopy(dstGlobal, dstLocal, outer * inner);
        outQueue.FreeTensor(dstLocal);
        AscendC::LocalTensor<T> lastRowLocal = lastRowQueue.DeQue<T>();
        AscendC::DataCopy(lastRowGlobal, lastRowLocal, inner);
        lastRowQueue.FreeTensor(lastRowLocal);
    }

private:
    AscendC::GlobalTensor<T> srcGlobal;
    AscendC::GlobalTensor<T> dstGlobal;
    AscendC::GlobalTensor<T> lastRowGlobal;
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueX;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueue;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> lastRowQueue;
    uint32_t outer{1};
    uint32_t inner{1};
};

constexpr AscendC::CumSumConfig cumSumConfig{true, false, true, AscendC::CumSumAlgorithm::CUMSUM_ALGORITHM_LINEBYLINE};

template <typename T>
__aicore__ inline void kernel_cumsum_operator(
    GM_ADDR srcGm, GM_ADDR dstGm, GM_ADDR lastRowGm, const AscendC::CumSumInfo &cumSumParams)
{
    KernelCumSum<T, cumSumConfig> op;
    op.Init(srcGm, dstGm, lastRowGm, cumSumParams);
    op.Process();
}
```

