# LayerNormGrad<a name="ZH-CN_TOPIC_0000001575083838"></a>

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

LayerNormGrad是一个函数，用于计算LayerNorm的反向传播梯度。该接口单独使用会输出x、resForGamma；也可以和LayerNormGradBeta配合使用，输出的resForGamma传递给LayerNormGradBeta， LayerNormGradBeta接口会输出gamma和beta，配合使用时就可以同时得到x、Gamma、beta。

算法公式为:

```
pd_xl(BSH) = data_dy * data_gamma
pd_var(H) = np.sum(((-0.5) * pd_xl * (data_x - data_mean) * np.power((data_variance + EPSILON), (-1.5))), reduce_axis, keepdims=True)
pd_mean(BS1) = np.sum(((-1.0) * pd_xl * np.power((data_variance + EPSILON), (-0.5))), reduce_axis, keepdims=True) + pd_var * (1.0 / H) * np.sum(((-2.0) * (data_x - data_mean)), reduce_axis, keepdims=True)
pd_x(BSH) = pd_xl * np.power((data_variance + EPSILON), (-0.5)) + pd_var * (2.0 / H) * (data_x - data_mean) + pd_mean * (1.0 / H)
res_for_gamma(BSH) = (data_x - data_mean) * np.power((data_variance + EPSILON), (-0.5))
```

## 实现原理<a name="section13229175017585"></a>

以float类型，ND格式，输入为inputDy\[B, S, H\], inputX\[B, S, H\], inputVariance\[B, S\], inputMean\[B, S\], inputGamma\[H\]为例，描述LayerNormGrad高阶API内部算法框图，如下图所示。

**图 1**  LayerNormGrad算法框图<a name="fig386485443713"></a>  
![](figures/LayerNormGrad算法框图.png "LayerNormGrad算法框图")

计算过程分为如下几步，均在Vector上进行：

1.  ComputePdX1步骤：计算inputDy\*inputGamma，结果存储至x1Tensor；
2.  ComputePdX2步骤：inputMean先通过Brcb将shape扩充到\[B, S, H\]，再计算inputX-inputMean，结果存储至x2Tensor；
3.  ComputePdVar步骤：实现公式np.sum\(\(\(-0.5\) \* x1Tensor \* x2Tensor \* np.power\(\(inputVariance + EPSILON\), \(-1.5\)\)\)\)的计算，power方法的实现通过Sqrt, Div, Mul三条基础API组合实现，结果存储至pdVarTensor；
4.  ComputePdMean：实现公式np.sum\(\(\(-1.0\) \* x1Tensor \* np.power\(\(inputVariance + EPSILON\), \(-0.5\)\)\)\) + pd\_var \* \(1.0 / H\) \* np.sum\(\(\(-2.0\) \* \(x2Tensor\)\)\)的计算，power方法通过Sqrt, Div两条基础API组合实现，结果存储至pdMeanTensor。同时，利用中间计算结果，根据公式x2Tensor \* np.power\(\(inputVariance + EPSILON\), \(-0.5\)\)，计算出resForGamma的结果；
5.  ComputePdX步骤：实现公式x1Tensor \* np.power\(\(inputVariance + EPSILON\), \(-0.5\)\) + pd\_var\*\(2.0 / H\)\*\(x2Tensor\) + pd\_mean\*\(1.0 / H\)的计算，结果存入outputPdX。

## 函数原型<a name="section1834111321944"></a>

由于该接口的内部实现中涉及复杂的计算，需要额外的临时空间来存储计算过程中的中间变量。临时空间大小BufferSize的获取方法：通过[LayerNormGrad Tiling](LayerNormGrad-Tiling.md)中提供的GetLayerNormGradMaxMinTmpSize接口获取所需最大和最小临时空间大小，最小空间可以保证功能正确，最大空间用于提升性能。

临时空间支持**接口框架申请**和开发者**通过sharedTmpBuffer入参传入**两种方式，因此LayerNormGrad接口的函数原型有两种：

-   通过sharedTmpBuffer入参传入临时空间

    ```
    template <typename T, bool isReuseSource = false>
    __aicore__ inline void LayerNormGrad(const LocalTensor<T>& outputPdX, const LocalTensor<T>& resForGamma, const LocalTensor<T>& inputDy, const LocalTensor<T>& inputX, const LocalTensor<T>& inputVariance, const LocalTensor<T>& inputMean, const LocalTensor<T>& inputGamma, LocalTensor<uint8_t>& sharedTmpBuffer, T epsilon, LayerNormGradTiling &tiling, const LayerNormGradShapeInfo& shapeInfo = {})
    ```

    该方式下开发者需自行申请并管理临时内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。

-   接口框架申请临时空间

    ```
    template <typename T, bool isReuseSource = false>
    __aicore__ inline void LayerNormGrad(const LocalTensor<T>& outputPdX, const LocalTensor<T>& resForGamma, const LocalTensor<T>& inputDy, const LocalTensor<T>& inputX, const LocalTensor<T>& inputVariance, const LocalTensor<T>& inputMean, const LocalTensor<T>& inputGamma, T epsilon, LayerNormGradTiling& tiling, const LayerNormGradShapeInfo& shapeInfo = {})
    ```

    该方式下开发者无需申请，但是需要预留临时空间的大小。

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table575571914269"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001574764914_row18755131942614"><th class="cellrowborder" valign="top" width="19.39%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001574764914_p675519193268"><a name="zh-cn_topic_0000001574764914_p675519193268"></a><a name="zh-cn_topic_0000001574764914_p675519193268"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="80.61%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001574764914_p375511918267"><a name="zh-cn_topic_0000001574764914_p375511918267"></a><a name="zh-cn_topic_0000001574764914_p375511918267"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001574764914_row14755141911264"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001574764914_p47551198266"><a name="zh-cn_topic_0000001574764914_p47551198266"></a><a name="zh-cn_topic_0000001574764914_p47551198266"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001574764914_p125969172719"><a name="zh-cn_topic_0000001574764914_p125969172719"></a><a name="zh-cn_topic_0000001574764914_p125969172719"></a>操作数的数据类型。</p>
<p id="p17361924184019"><a name="p17361924184019"></a><a name="p17361924184019"></a><span id="ph193084399401"><a name="ph193084399401"></a><a name="ph193084399401"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half、float。</p>
<p id="zh-cn_topic_0000001574764914_p105913396307"><a name="zh-cn_topic_0000001574764914_p105913396307"></a><a name="zh-cn_topic_0000001574764914_p105913396307"></a><span id="ph168071952123011"><a name="ph168071952123011"></a><a name="ph168071952123011"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half、float。</p>
<p id="zh-cn_topic_0000001574764914_p10591143917308"><a name="zh-cn_topic_0000001574764914_p10591143917308"></a><a name="zh-cn_topic_0000001574764914_p10591143917308"></a><span id="ph1761169310"><a name="ph1761169310"></a><a name="ph1761169310"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half、float。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001574764914_row9756719122620"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001574764914_p1682112447268"><a name="zh-cn_topic_0000001574764914_p1682112447268"></a><a name="zh-cn_topic_0000001574764914_p1682112447268"></a>isReuseSource</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001574764914_p1275717613718"><a name="zh-cn_topic_0000001574764914_p1275717613718"></a><a name="zh-cn_topic_0000001574764914_p1275717613718"></a>是否允许修改源操作数，默认值为false。如果开发者允许源操作数被改写，可以使能该参数，使能后能够节省部分内存空间。</p>
<p id="zh-cn_topic_0000001574764914_p175786163713"><a name="zh-cn_topic_0000001574764914_p175786163713"></a><a name="zh-cn_topic_0000001574764914_p175786163713"></a>设置为<strong id="zh-cn_topic_0000001574764914_b575706193714"><a name="zh-cn_topic_0000001574764914_b575706193714"></a><a name="zh-cn_topic_0000001574764914_b575706193714"></a>true</strong>，则本接口内部计算时<strong id="zh-cn_topic_0000001574764914_b147578614378"><a name="zh-cn_topic_0000001574764914_b147578614378"></a><a name="zh-cn_topic_0000001574764914_b147578614378"></a>复用</strong>inputX的内存空间，节省内存空间；设置为<strong id="zh-cn_topic_0000001574764914_b475717616379"><a name="zh-cn_topic_0000001574764914_b475717616379"></a><a name="zh-cn_topic_0000001574764914_b475717616379"></a>false</strong>，则本接口内部计算时<strong id="zh-cn_topic_0000001574764914_b157575653719"><a name="zh-cn_topic_0000001574764914_b157575653719"></a><a name="zh-cn_topic_0000001574764914_b157575653719"></a>不复用</strong>inputX的内存空间。</p>
<p id="zh-cn_topic_0000001574764914_p177571162377"><a name="zh-cn_topic_0000001574764914_p177571162377"></a><a name="zh-cn_topic_0000001574764914_p177571162377"></a>对于float数据类型输入支持开启该参数，half数据类型输入不支持开启该参数。</p>
<p id="zh-cn_topic_0000001574764914_p62891018544"><a name="zh-cn_topic_0000001574764914_p62891018544"></a><a name="zh-cn_topic_0000001574764914_p62891018544"></a>isReuseSource的使用样例请参考<a href="更多样例-84.md#section639165323915">更多样例</a>。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="table12251635103118"></a>
<table><thead align="left"><tr id="row147393513315"><th class="cellrowborder" valign="top" width="19.515151515151516%" id="mcps1.2.4.1.1"><p id="p107353563110"><a name="p107353563110"></a><a name="p107353563110"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="17.898989898989896%" id="mcps1.2.4.1.2"><p id="p1773133533118"><a name="p1773133533118"></a><a name="p1773133533118"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="62.58585858585859%" id="mcps1.2.4.1.3"><p id="p1873235143119"><a name="p1873235143119"></a><a name="p1873235143119"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row10734355318"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p1739358317"><a name="p1739358317"></a><a name="p1739358317"></a>outputPdX</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p13731535143111"><a name="p13731535143111"></a><a name="p13731535143111"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p446091145118"><a name="p446091145118"></a><a name="p446091145118"></a>目的操作数，shape为[B, S, H]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。尾轴长度需要32B对齐。</p>
<p id="p16911647191712"><a name="p16911647191712"></a><a name="p16911647191712"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row57393553110"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p073935123115"><a name="p073935123115"></a><a name="p073935123115"></a>resForGamma</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p157319358315"><a name="p157319358315"></a><a name="p157319358315"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p2543123391815"><a name="p2543123391815"></a><a name="p2543123391815"></a>目的操作数，shape为[B, S, H]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。尾轴长度需要32B对齐。</p>
<p id="p1667364612223"><a name="p1667364612223"></a><a name="p1667364612223"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row273203510317"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p17333515314"><a name="p17333515314"></a><a name="p17333515314"></a>inputDy</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p373103543120"><a name="p373103543120"></a><a name="p373103543120"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p4705122819612"><a name="p4705122819612"></a><a name="p4705122819612"></a>源操作数，shape为[B, S, H]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。inputDy的数据类型需要与目的操作数保持一致，尾轴长度需要32B对齐。</p>
<p id="p8158135011227"><a name="p8158135011227"></a><a name="p8158135011227"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_2"><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_2"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_2"><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_2"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_2"><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_2"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row9731035103113"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p10730352311"><a name="p10730352311"></a><a name="p10730352311"></a>inputX</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p7731235183110"><a name="p7731235183110"></a><a name="p7731235183110"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p83698388549"><a name="p83698388549"></a><a name="p83698388549"></a>源操作数，shape为[B, S, H]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。inputX的数据类型需要与目的操作数保持一致，尾轴长度需要32B对齐。</p>
<p id="p15292175272210"><a name="p15292175272210"></a><a name="p15292175272210"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_3"><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_3"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_3"><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_3"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_3"><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_3"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1373935103114"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p1074435103110"><a name="p1074435103110"></a><a name="p1074435103110"></a>inputVariance</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p274143533119"><a name="p274143533119"></a><a name="p274143533119"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p315716230193"><a name="p315716230193"></a><a name="p315716230193"></a>方差，shape为[B, S]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。inputVariance的数据类型需要与目的操作数保持一致，尾轴长度需要32B对齐。需提前调用<a href="LayerNorm.md">LayerNorm</a>接口获取方差。</p>
<p id="p199741055102215"><a name="p199741055102215"></a><a name="p199741055102215"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_4"><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_4"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_4"><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_4"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_4"><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_4"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row374133563113"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p3741535183115"><a name="p3741535183115"></a><a name="p3741535183115"></a>inputMean</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p574183543112"><a name="p574183543112"></a><a name="p574183543112"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p19979182613198"><a name="p19979182613198"></a><a name="p19979182613198"></a>均值，shape为[B, S]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。inputMean的数据类型需要与目的操作数保持一致，尾轴长度需要32B对齐。需提前调用<a href="LayerNorm.md">LayerNorm</a>接口获取均值。</p>
<p id="p10414135892213"><a name="p10414135892213"></a><a name="p10414135892213"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_5"><a name="zh-cn_topic_0000001530181537_ph173308471594_5"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_5"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_5"><a name="zh-cn_topic_0000001530181537_ph9902231466_5"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_5"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_5"><a name="zh-cn_topic_0000001530181537_ph1782115034816_5"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_5"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1741335203110"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p874143523113"><a name="p874143523113"></a><a name="p874143523113"></a>inputGamma</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p57411353318"><a name="p57411353318"></a><a name="p57411353318"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p11621739151312"><a name="p11621739151312"></a><a name="p11621739151312"></a>源操作数，shape为[H]，LocalTensor数据结构的定义请参考<a href="LocalTensor.md">LocalTensor</a>。inputGamma的数据类型需要与目的操作数保持一致，尾轴长度需要32B对齐。</p>
<p id="p850112112313"><a name="p850112112313"></a><a name="p850112112313"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_6"><a name="zh-cn_topic_0000001530181537_ph173308471594_6"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_6"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_6"><a name="zh-cn_topic_0000001530181537_ph9902231466_6"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_6"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_6"><a name="zh-cn_topic_0000001530181537_ph1782115034816_6"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_6"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row1984913418518"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p215910345451"><a name="p215910345451"></a><a name="p215910345451"></a>sharedTmpBuffer</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p20159183474511"><a name="p20159183474511"></a><a name="p20159183474511"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p47801866195"><a name="p47801866195"></a><a name="p47801866195"></a>共享缓冲区，用于存放API内部计算产生的临时数据。该方式开发者可以自行管理sharedTmpBuffer内存空间，并在接口调用完成后，复用该部分内存，内存不会反复申请释放，灵活性较高，内存利用率也较高。共享缓冲区大小的获取方式请参考<a href="LayerNormGrad-Tiling.md">LayerNormGrad Tiling</a>。</p>
<p id="p167809613199"><a name="p167809613199"></a><a name="p167809613199"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_7"><a name="zh-cn_topic_0000001530181537_ph173308471594_7"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_7"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_7"><a name="zh-cn_topic_0000001530181537_ph9902231466_7"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_7"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_7"><a name="zh-cn_topic_0000001530181537_ph1782115034816_7"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_7"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
</td>
</tr>
<tr id="row197416350316"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p5746352312"><a name="p5746352312"></a><a name="p5746352312"></a>epsilon</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p1874935103113"><a name="p1874935103113"></a><a name="p1874935103113"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p77415357313"><a name="p77415357313"></a><a name="p77415357313"></a>防除零的权重系数。</p>
</td>
</tr>
<tr id="row197433517318"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p1674193543111"><a name="p1674193543111"></a><a name="p1674193543111"></a>tiling</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p147412358314"><a name="p147412358314"></a><a name="p147412358314"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p8555165810134"><a name="p8555165810134"></a><a name="p8555165810134"></a>LayerNormGrad计算所需Tiling信息。</p>
</td>
</tr>
<tr id="row20781113045518"><td class="cellrowborder" valign="top" width="19.515151515151516%" headers="mcps1.2.4.1.1 "><p id="p878253055512"><a name="p878253055512"></a><a name="p878253055512"></a>shapeInfo</p>
</td>
<td class="cellrowborder" valign="top" width="17.898989898989896%" headers="mcps1.2.4.1.2 "><p id="p6782830195514"><a name="p6782830195514"></a><a name="p6782830195514"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="62.58585858585859%" headers="mcps1.2.4.1.3 "><p id="p77829307554"><a name="p77829307554"></a><a name="p77829307554"></a>表示LayerNormGrad各个输入的数据排布格式Format。默认值表示输入的Format为ND。支持的取值为DataFormat::ND。LayerNormGradShapeInfo类型，具体定义如下。</p>
<a name="screen1989483602319"></a><a name="screen1989483602319"></a><pre class="screen" codetype="Cpp" id="screen1989483602319">struct LayerNormGradShapeInfo {
    DataFormat dataFormat = DataFormat::ND;
};</pre>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section5468191312484"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。
-   源操作数和目的操作数的Tensor空间可以复用。
-   仅支持输入shape为ND格式。
-   输入数据不满足对齐要求时，开发者需要进行补齐，补齐的数据应设置为0，防止出现异常值从而影响网络计算。
-   不支持对尾轴H轴的切分。

## 调用示例<a name="section642mcpsimp"></a>

本样例中，输入inputX和inputDy的shape为\[2, 32, 16\]，inputVariance和inputMean的shape为\[2, 32\]，inputGamma的shape为\[16\]。输出outputPdX和resForGamma的shape为\[2, 32, 16\]。数据排布均为ND格式，数据类型均为float，不复用源操作数的内存空间。

```
#include "kernel_operator.h"

namespace MyCustomKernel {
struct VecTiling {
    LayerNormGradTiling layernormGradTilingData;
    float epsilon = 0;
};

template <bool isReuseSource = false> class KernelLayernormGrad {
public:
    __aicore__ inline KernelLayernormGrad() {}
    __aicore__ inline void Init(GM_ADDR inputXGm, GM_ADDR inputDyGm, GM_ADDR inputVarianceGm, GM_ADDR inputMeanGm,
        GM_ADDR inputGammaGm, GM_ADDR outputPdXGm, GM_ADDR resForGammaGm, VecTiling tilingData)
    {
        this->epsilon = tilingData.epsilon;
        tiling_ = tilingData.layernormGradTilingData;
        this->bLength = tiling_.bLength;
        this->sLength = tiling_.sLength;
        this->hLength = tiling_.hLength;
        bshLength = bLength * sLength * hLength;
        bsLength = bLength * sLength;
        inputXGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(inputXGm), bshLength);
        inputDyGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(inputDyGm), bshLength);
        inputVarianceGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(inputVarianceGm), bsLength);
        inputMeanGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(inputMeanGm), bsLength);
        inputGammaGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(inputGammaGm), hLength);
        outputPdXGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(outputPdXGm), bshLength);
        outputResForGammaGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float *>(resForGammaGm), bshLength);
        pipe.InitBuffer(inQueueX, 1, sizeof(float) * bshLength);
        pipe.InitBuffer(inQueueDy, 1, sizeof(float) * bshLength);
        pipe.InitBuffer(inQueueVariance, 1, sizeof(float) * bsLength);
        pipe.InitBuffer(inQueueMean, 1, sizeof(float) * bsLength);
        pipe.InitBuffer(inQueueGamma, 1, sizeof(float) * hLength);
        pipe.InitBuffer(outQueuePdX, 1, sizeof(float) * bshLength);
        pipe.InitBuffer(outQueueResForGamma, 1, sizeof(float) * bshLength);
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
        AscendC::LocalTensor<float> inputXLocal = inQueueX.AllocTensor<float>();
        AscendC::LocalTensor<float> inputDyLocal = inQueueDy.AllocTensor<float>();
        AscendC::LocalTensor<float> inputVarianceLocal = inQueueVariance.AllocTensor<float>();
        AscendC::LocalTensor<float> inputMeanLocal = inQueueMean.AllocTensor<float>();
        AscendC::LocalTensor<float> inputGammaLocal = inQueueGamma.AllocTensor<float>();
        
        AscendC::DataCopy(inputXLocal, inputXGlobal, bshLength);
        AscendC::DataCopy(inputDyLocal, inputDyGlobal, bshLength);
        AscendC::DataCopy(inputVarianceLocal, inputVarianceGlobal, bsLength);
        AscendC::DataCopy(inputMeanLocal, inputMeanGlobal, bsLength);
        AscendC::DataCopy(inputGammaLocal, inputGammaGlobal, hLength);
        inQueueX.EnQue(inputXLocal);
        inQueueDy.EnQue(inputDyLocal);
        inQueueVariance.EnQue(inputVarianceLocal);
        inQueueMean.EnQue(inputMeanLocal);
        inQueueGamma.EnQue(inputGammaLocal);
    }
    __aicore__ inline void Compute()
    {
        AscendC::LocalTensor<float> inputXLocal = inQueueX.DeQue<float>();
        AscendC::LocalTensor<float> inputDyLocal = inQueueDy.DeQue<float>();
        AscendC::LocalTensor<float> inputVarianceLocal = inQueueVariance.DeQue<float>();
        AscendC::LocalTensor<float> inputMeanLocal = inQueueMean.DeQue<float>();
        AscendC::LocalTensor<float> inputGammaLocal = inQueueGamma.DeQue<float>();
        AscendC::LocalTensor<float> outputPdXLocal = outQueuePdX.AllocTensor<float>();
        AscendC::LocalTensor<float> outputResForGammaLocal = outQueueResForGamma.AllocTensor<float>();
        AscendC::LayerNormGrad<float, isReuseSource>(outputPdXLocal, outputResForGammaLocal, 
            inputDyLocal, inputXLocal, inputVarianceLocal, inputMeanLocal, inputGammaLocal, 
            (float)epsilon, tiling_, {DataFormat::ND});
        outQueuePdX.EnQue(outputPdXLocal);
        outQueueResForGamma.EnQue(outputResForGammaLocal);
        inQueueX.FreeTensor(inputXLocal);
        inQueueDy.FreeTensor(inputDyLocal);
        inQueueVariance.FreeTensor(inputVarianceLocal);
        inQueueMean.FreeTensor(inputMeanLocal);
        inQueueGamma.FreeTensor(inputGammaLocal);
    }
    __aicore__ inline void CopyOut()
    {
        AscendC::LocalTensor<float> outputPdXLocal = outQueuePdX.DeQue<float>();
        AscendC::LocalTensor<float> outputResForGammaLocal = outQueueResForGamma.DeQue<float>();
        AscendC::DataCopy(outputPdXGlobal, outputPdXLocal, bshLength);
        AscendC::DataCopy(outputResForGammaGlobal, outputResForGammaLocal, bshLength);
        outQueuePdX.FreeTensor(outputPdXLocal);
        outQueueResForGamma.FreeTensor(outputResForGammaLocal);
    }
private:
    AscendC::GlobalTensor<float> inputXGlobal;
    AscendC::GlobalTensor<float> inputDyGlobal;
    AscendC::GlobalTensor<float> inputVarianceGlobal;
    AscendC::GlobalTensor<float> inputMeanGlobal;
    AscendC::GlobalTensor<float> inputGammaGlobal;
    AscendC::GlobalTensor<float> outputPdXGlobal;
    AscendC::GlobalTensor<float> outputResForGammaGlobal;
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueX;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueDy;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueVariance;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueMean;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueGamma;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueuePdX;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueResForGamma;
    uint32_t bLength;
    uint32_t sLength;
    uint32_t hLength;
    float epsilon;
    LayerNormGradTiling tiling_;
    uint32_t bshLength;
    uint32_t bsLength;
};
}

extern "C" __global__ __aicore__ void layernorm_grad_custom(GM_ADDR inputXGm, GM_ADDR inputDyGm, GM_ADDR inputVarianceGm, 
    GM_ADDR inputMeanGm, GM_ADDR inputGammaGm, GM_ADDR outputPdXGm, GM_ADDR resForGammaGm, 
    GM_ADDR workspace, GM_ADDR tiling)
{
    if ASCEND_IS_AIC {
        return;
    }
    MyCustomKernel::VecTiling tilingData;
    CopyTiling(&tilingData, tiling);
    MyCustomKernel::KernelLayernormGrad<false> op;
    op.Init(inputXGm, inputDyGm, inputVarianceGm, inputMeanGm, inputGammaGm, outputPdXGm, resForGammaGm, tilingData);
    op.Process();
}
```

