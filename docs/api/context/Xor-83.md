# Xor<a name="ZH-CN_TOPIC_0000001714185806"></a>

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

按元素执行Xor运算，Xor（异或）的概念和运算规则如下：

-   概念：参加运算的两个数据，按二进制位进行“异或”运算。
-   运算规则：0^0=0；0^1=1；1^0=1；1^1=0；即：参加运算的两个对象，如果两个相应位为“异”（值不同），则该位结果为1，否则为 0【同0异1】。

计算公式如下：

![](figures/zh-cn_formulaimage_0000002342367301.png)

![](figures/zh-cn_formulaimage_0000002188545700.png)

```
例如：3^5=6，即0000 0011^0000 0101 = 0000 0110
```

## 函数原型<a name="section620mcpsimp"></a>

-   通过sharedTmpBuffer入参传入临时空间
    -   源操作数Tensor全部/部分参与计算

        ```
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void Xor(const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor, const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
        ```

    -   源操作数Tensor全部参与计算

        ```
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void Xor(const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor, const LocalTensor<uint8_t>& sharedTmpBuffer)
        ```

-   接口框架申请临时空间
    -   源操作数Tensor全部/部分参与计算

        ```
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void Xor(const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor, const uint32_t calCount)
        ```

    -   源操作数Tensor全部参与计算

        ```
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void Xor(const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor)
        ```

由于该接口的内部实现中涉及复杂的数学计算，需要额外的临时空间来存储计算过程中的中间变量。临时空间支持开发者**通过sharedTmpBuffer入参传入**和**接口框架申请**两种方式。

-   通过sharedTmpBuffer入参传入，使用该tensor作为临时空间进行处理，接口框架不再申请。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。
-   接口框架申请临时空间，开发者无需申请，但是需要预留临时空间的大小。

通过sharedTmpBuffer传入的情况，开发者需要为tensor申请空间；接口框架申请的方式，开发者需要预留临时空间。临时空间大小BufferSize的获取方式如下：通过[GetXorMaxMinTmpSize](GetXorMaxMinTmpSize.md)中提供的接口获取需要预留空间范围的大小。

## 参数说明<a name="section622mcpsimp"></a>

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
<p id="p18371315130"><a name="p18371315130"></a><a name="p18371315130"></a><span id="ph4372141181317"><a name="ph4372141181317"></a><a name="ph4372141181317"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：int16_t、uint16_t。</p>
<p id="p1314412701316"><a name="p1314412701316"></a><a name="p1314412701316"></a><span id="ph11441227191318"><a name="ph11441227191318"></a><a name="ph11441227191318"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：int16_t、uint16_t。</p>
<p id="p4369182010169"><a name="p4369182010169"></a><a name="p4369182010169"></a><span id="ph1136972016166"><a name="ph1136972016166"></a><a name="ph1136972016166"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：int16_t、uint16_t。</p>
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

<a name="table148471830151913"></a>
<table><thead align="left"><tr id="row1984733010194"><th class="cellrowborder" valign="top" width="17.380000000000003%" id="mcps1.2.4.1.1"><p id="p2847730181917"><a name="p2847730181917"></a><a name="p2847730181917"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="10.38%" id="mcps1.2.4.1.2"><p id="p58476303197"><a name="p58476303197"></a><a name="p58476303197"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.24000000000001%" id="mcps1.2.4.1.3"><p id="p10847203021913"><a name="p10847203021913"></a><a name="p10847203021913"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row98477303196"><td class="cellrowborder" valign="top" width="17.380000000000003%" headers="mcps1.2.4.1.1 "><p id="p15847183018194"><a name="p15847183018194"></a><a name="p15847183018194"></a>dstTensor</p>
</td>
<td class="cellrowborder" valign="top" width="10.38%" headers="mcps1.2.4.1.2 "><p id="p148471930161917"><a name="p148471930161917"></a><a name="p148471930161917"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.24000000000001%" headers="mcps1.2.4.1.3 "><p id="p17444349398"><a name="p17444349398"></a><a name="p17444349398"></a>目的操作数。</p>
<p id="p16703131355116"><a name="p16703131355116"></a><a name="p16703131355116"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row11848103091920"><td class="cellrowborder" valign="top" width="17.380000000000003%" headers="mcps1.2.4.1.1 "><p id="p58481330191917"><a name="p58481330191917"></a><a name="p58481330191917"></a>src0Tensor</p>
</td>
<td class="cellrowborder" valign="top" width="10.38%" headers="mcps1.2.4.1.2 "><p id="p158485305196"><a name="p158485305196"></a><a name="p158485305196"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.24000000000001%" headers="mcps1.2.4.1.3 "><p id="p3707144233915"><a name="p3707144233915"></a><a name="p3707144233915"></a>源操作数0。</p>
<p id="p261714443395"><a name="p261714443395"></a><a name="p261714443395"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p515144315188"><a name="p515144315188"></a><a name="p515144315188"></a>源操作数的数据类型需要与目的操作数保持一致。</p>
</td>
</tr>
<tr id="row208920431012"><td class="cellrowborder" valign="top" width="17.380000000000003%" headers="mcps1.2.4.1.1 "><p id="p7416205314019"><a name="p7416205314019"></a><a name="p7416205314019"></a>src1Tensor</p>
</td>
<td class="cellrowborder" valign="top" width="10.38%" headers="mcps1.2.4.1.2 "><p id="p154166530018"><a name="p154166530018"></a><a name="p154166530018"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.24000000000001%" headers="mcps1.2.4.1.3 "><p id="p8416653103"><a name="p8416653103"></a><a name="p8416653103"></a>源操作数1。</p>
<p id="p114164531600"><a name="p114164531600"></a><a name="p114164531600"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_2"><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_2"><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_2"><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p194161531507"><a name="p194161531507"></a><a name="p194161531507"></a>源操作数的数据类型需要与目的操作数保持一致。</p>
</td>
</tr>
<tr id="row4848123011192"><td class="cellrowborder" valign="top" width="17.380000000000003%" headers="mcps1.2.4.1.1 "><p id="p1313415271911"><a name="p1313415271911"></a><a name="p1313415271911"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="10.38%" headers="mcps1.2.4.1.2 "><p id="p5133352201914"><a name="p5133352201914"></a><a name="p5133352201914"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.24000000000001%" headers="mcps1.2.4.1.3 "><p id="p1148552183919"><a name="p1148552183919"></a><a name="p1148552183919"></a>临时缓存。</p>
<p id="p5983205412394"><a name="p5983205412394"></a><a name="p5983205412394"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_3"><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_3"><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_3"><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p104071111204211"><a name="p104071111204211"></a><a name="p104071111204211"></a>用于Xor内部复杂计算时存储中间变量，由开发者提供。</p>
<p id="p5881016172817"><a name="p5881016172817"></a><a name="p5881016172817"></a>临时空间大小BufferSize的获取方式请参考<a href="GetXorMaxMinTmpSize.md">GetXorMaxMinTmpSize</a>。</p>
</td>
</tr>
<tr id="row216545817417"><td class="cellrowborder" valign="top" width="17.380000000000003%" headers="mcps1.2.4.1.1 "><p id="p1949611581317"><a name="p1949611581317"></a><a name="p1949611581317"></a>calCount</p>
</td>
<td class="cellrowborder" valign="top" width="10.38%" headers="mcps1.2.4.1.2 "><p id="p174961758436"><a name="p174961758436"></a><a name="p174961758436"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.24000000000001%" headers="mcps1.2.4.1.3 "><p id="p184961858133"><a name="p184961858133"></a><a name="p184961858133"></a>参与计算的元素个数。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section38228281712"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   **不支持源操作数与目的操作数地址重叠。**
-   当前仅支持ND格式的输入，不支持其他格式。
-   calCount需要保证小于或等于src0Tensor和src1Tensor和dstTensor存储的元素范围。
-   对于不带calCount参数的接口，需要保证src0Tensor和src1Tensor的shape大小相等。
-   不支持sharedTmpBuffer与源操作数和目的操作数地址重叠。
-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。

## 调用示例<a name="section642mcpsimp"></a>

**调用样例kernel侧xor\_custom.cpp**

```
#include "kernel_operator.h"

constexpr int32_t BUFFER_NUM = 1;
class KernelXor {
public:
    __aicore__ inline KernelXor() {}
    __aicore__ inline void Init(GM_ADDR x, GM_ADDR y, GM_ADDR z, uint32_t totalLength, uint32_t totalLength2, uint32_t tilenum, uint32_t tmpSize, uint32_t mcount)
    {
        this->totalLength = totalLength;
        this->blockLength = totalLength / AscendC::GetBlockNum();
        this->blockLength2 = totalLength2 / AscendC::GetBlockNum();
        this->tilenum = tilenum;
        this->tmpSize = tmpSize;
        this->mcount = mcount;
        this->tileLength = this->blockLength / tilenum / BUFFER_NUM;
        this->tileLength2 = this->blockLength2 / tilenum / BUFFER_NUM;

        xGm.SetGlobalBuffer((__gm__ int16_t *)x + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
        yGm.SetGlobalBuffer((__gm__ int16_t *)y + this->blockLength2 * AscendC::GetBlockIdx(), this->blockLength2);
        zGm.SetGlobalBuffer((__gm__ int16_t *)z + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);

        if (this->tmpSize != 0) {
            pipe.InitBuffer(tmpQueue, BUFFER_NUM, this->tmpSize);
        }
        pipe.InitBuffer(inQueueX, BUFFER_NUM, this->tileLength * sizeof(int16_t));
        pipe.InitBuffer(inQueueY, BUFFER_NUM, this->tileLength2 * sizeof(int16_t));
        pipe.InitBuffer(outQueueZ, BUFFER_NUM, this->tileLength * sizeof(int16_t));
    }
    __aicore__ inline void Process()
    {
        int32_t loopCount = this->tilenum * BUFFER_NUM;
        for (int32_t i = 0; i < loopCount; i++) {
            CopyIn(i);
            Compute(i);
            CopyOut(i);
        }
    }

private:
    __aicore__ inline void CopyIn(int32_t progress)
    {
        AscendC::LocalTensor<int16_t> xLocal = inQueueX.AllocTensor<int16_t>();
        AscendC::DataCopy(xLocal, xGm[progress * this->tileLength], this->tileLength);
        inQueueX.EnQue(xLocal);
        AscendC::LocalTensor<int16_t> yLocal = inQueueY.AllocTensor<int16_t>();
        AscendC::DataCopy(yLocal, yGm[progress * this->tileLength2], this->tileLength2);
        inQueueY.EnQue(yLocal);
    }
    __aicore__ inline void Compute(int32_t progress)
    {
        AscendC::LocalTensor<int16_t> xLocal = inQueueX.DeQue<int16_t>();
        AscendC::LocalTensor<int16_t> yLocal = inQueueY.DeQue<int16_t>();
        AscendC::LocalTensor<int16_t> zLocal = outQueueZ.AllocTensor<int16_t>();
        if (this->tmpSize != 0) {
            AscendC::LocalTensor<uint8_t> tmpLocal = tmpQueue.AllocTensor<uint8_t>();
            if (this->mcount != this->totalLength) {
                AscendC::Xor(zLocal, xLocal, yLocal, tmpLocal, this->mcount);
            } else {
                AscendC::Xor(zLocal, xLocal, yLocal, tmpLocal);
            }
            tmpQueue.FreeTensor(tmpLocal);
        } else {
            if (this->mcount != this->totalLength) {
                AscendC::Xor(zLocal, xLocal, yLocal, this->mcount);
            } else {
                AscendC::Xor(zLocal, xLocal, yLocal);
            }
        }
        outQueueZ.EnQue<int16_t>(zLocal);
        inQueueX.FreeTensor(xLocal);
        inQueueY.FreeTensor(yLocal);
    }
    __aicore__ inline void CopyOut(int32_t progress)
    {
        AscendC::LocalTensor<int16_t> zLocal = outQueueZ.DeQue<int16_t>();
        AscendC::DataCopy(zGm[progress * this->tileLength], zLocal, this->tileLength);
        outQueueZ.FreeTensor(zLocal);
    }

private:
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueX;
    AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> inQueueY;
    AscendC::TQue<AscendC::TPosition::VECIN, BUFFER_NUM> tmpQueue;
    AscendC::TQue<AscendC::TPosition::VECOUT, BUFFER_NUM> outQueueZ;
    AscendC::GlobalTensor<int16_t> xGm;
    AscendC::GlobalTensor<int16_t> yGm;
    AscendC::GlobalTensor<int16_t> zGm;
    uint32_t blockLength;
    uint32_t blockLength2;
    uint32_t tilenum;
    uint32_t tileLength;
    uint32_t tileLength2;
    uint32_t tmpSize;
    uint32_t mcount;
    uint32_t totalLength;
};

extern "C" __global__ __aicore__ void xor_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z, GM_ADDR workspace, GM_ADDR tiling)
{
    GET_TILING_DATA(tilingData, tiling);
    KernelXor op;
    op.Init(x, y, z, tilingData.totalLength, tilingData.totalLength2, tilingData.tilenum, tilingData.tmpSize, tilingData.mcount);
    if (TILING_KEY_IS(1)) {
        op.Process();
    }
}
```

**host侧xor\_custom\_tiling.h**

```
#include "register/op_def_registry.h"
#include "register/tilingdata_base.h"
namespace optiling {
  BEGIN_TILING_DATA_DEF(XorCustomTilingData)
  TILING_DATA_FIELD_DEF(uint32_t, totalLength);
  TILING_DATA_FIELD_DEF(uint32_t, totalLength2);
  TILING_DATA_FIELD_DEF(uint32_t, tmpSize);
  TILING_DATA_FIELD_DEF(uint32_t, tilenum);
  TILING_DATA_FIELD_DEF(uint32_t, mcount);
  END_TILING_DATA_DEF;
  REGISTER_TILING_DATA_CLASS(XorCustom, XorCustomTilingData)
}
```

**host侧xor\_custom.cpp**

```
#include "xor_custom_tiling.h"
#include "register/op_def_registry.h"
#include "tiling/tiling_api.h"

namespace optiling
{
    static ge::graphStatus TilingFunc(gert::TilingContext *context)
    {
        XorCustomTilingData tiling;
        const gert::RuntimeAttrs *xorAttrs = context->GetAttrs();
        const uint32_t tilenum = *(xorAttrs->GetAttrPointer<uint32_t>(0));
        const uint32_t numBlocks = *(xorAttrs->GetAttrPointer<uint32_t>(1));
        const uint32_t sizeflag = *(xorAttrs->GetAttrPointer<uint32_t>(2));
        const uint32_t countflag = *(xorAttrs->GetAttrPointer<uint32_t>(3));
        uint32_t totalLength = context->GetInputTensor(0)->GetShapeSize();
        uint32_t totalLength2 = context->GetInputTensor(1)->GetShapeSize();
        context->SetBlockDim(numBlocks);
        tiling.set_totalLength(totalLength);
        tiling.set_totalLength2(totalLength2);
        tiling.set_tilenum(tilenum);

        if (countflag == 0) {
            tiling.set_mcount(totalLength2);
        } else if (countflag == 1) {
            tiling.set_mcount(totalLength);
        }

        std::vector<int64_t> shapeVec = {totalLength};
        ge::Shape srcShape(shapeVec);
        uint32_t typeSize = sizeof(int16_t);
        uint32_t maxValue = 0;
        uint32_t minValue = 0;
        bool isReuseSource = false;
        AscendC::GetXorMaxMinTmpSize(srcShape, typeSize, isReuseSource, maxValue, minValue);
        // sizeflag 0：代表取最小的tempBuffer 1：取最大的tempBuffer
        if (sizeflag == 0) {
            tiling.set_tmpSize(minValue);
        } else if (sizeflag == 1) {
            tiling.set_tmpSize(maxValue);
        } else if (sizeflag == 2) {
            tiling.set_tmpSize(0);
        }
        tiling.SaveToBuffer(context->GetRawTilingData()->GetData(), context->GetRawTilingData()->GetCapacity());
        context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());
        context->SetTilingKey(1);
        size_t *currentWorkspace = context->GetWorkspaceSizes(1);
        currentWorkspace[0] = 0;
        return ge::GRAPH_SUCCESS;
    }
}
namespace ge
{
    static ge::graphStatus InferShape(gert::InferShapeContext *context)
    {
        const gert::Shape *xShape = context->GetInputShape(0);
        gert::Shape *yShape = context->GetOutputShape(0);
        *yShape = *xShape;
        return GRAPH_SUCCESS;
    }
}
namespace ops
{
    class XorCustom : public OpDef
    {
    public:
        explicit XorCustom(const char *name) : OpDef(name)
        {
            this->Input("x")
                .ParamType(REQUIRED)
                .DataType({ge::DT_INT16})
                .Format({ge::FORMAT_ND});
            this->Input("y")
                .ParamType(REQUIRED)
                .DataType({ge::DT_INT16})
                .Format({ge::FORMAT_ND});
            this->Output("z")
                .ParamType(REQUIRED)
                .DataType({ge::DT_INT16})
                .Format({ge::FORMAT_ND});
            this->SetInferShape(ge::InferShape);
            this->Attr("tilenum")
                .AttrType(REQUIRED)
                .Int(0);
            this->Attr("numBlocks")
                .AttrType(REQUIRED)
                .Int(0);
            this->Attr("sizeflag")
                .AttrType(REQUIRED)
                .Int(0);
            this->Attr("countflag")
                .AttrType(REQUIRED)
                .Int(0);
            this->AICore()
                .SetTiling(optiling::TilingFunc);
            this->AICore().AddConfig("ascendxxx"); // ascendxxx请修改为对应的AI处理器型号。
        }
    };
    OP_ADD(XorCustom);
} // namespace ops
```

结果示例如下：

```
输入输出的数据类型为int16_t，一维向量包含32个数。例如向量中第一个数据进行异或：(-5753) xor 18745 = -24386
输入数据(src0Local): [-5753 28501 20334 -5845  ... -20817 3403 21261 22241]
输入数据(src1Local): [18745 -24448 20873 10759 ... 21940 -26342 9251 31019]
输出数据(dstLocal): [-24386 -12331 7911 -15572 ... -1253 -27567 30510 12234]
```

