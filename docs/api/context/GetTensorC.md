# GetTensorC<a name="ZH-CN_TOPIC_0000001621512221"></a>

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
<tr id="row2024235811401"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p4242155874012"><a name="p4242155874012"></a><a name="p4242155874012"></a><span id="ph14879133874118"><a name="ph14879133874118"></a><a name="ph14879133874118"></a>Kirin X90</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p824235884010"><a name="p824235884010"></a><a name="p824235884010"></a>√</p>
</td>
</tr>
<tr id="row141206104332"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p142462448111"><a name="p142462448111"></a><a name="p142462448111"></a><span id="ph49484268126"><a name="ph49484268126"></a><a name="ph49484268126"></a>Kirin 9030</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p9246184415111"><a name="p9246184415111"></a><a name="p9246184415111"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

Iterate后，获取一块或者两块C矩阵片，可以直接输出到GM tensor中，也可以输出到VECIN tensor中。当[MatmulConfig](MatmulConfig.md#table1761013213153)参数中的ScheduleType取值为ScheduleType::INNER\_PRODUCT时，获取一块C矩阵片；当[MatmulConfig](MatmulConfig.md#table1761013213153)参数中的ScheduleType取值为ScheduleType::OUTER\_PRODUCT时，获取两块C矩阵片。

该接口和[Iterate](Iterate.md)接口配合使用，用于在调用Iterate完成迭代计算后，根据[MatmulConfig](MatmulConfig.md#table1761013213153)参数中的ScheduleType取值获取一块或两块baseM \* baseN大小的矩阵分片。

迭代获取C矩阵分片的过程分为同步和异步两种模式：

-   **同步：**执行完一次Iterate后执行一次GetTensorC，需要同步等待C矩阵分片获取完成。
-   **异步：**调用Iterate后，无需立即调用GetTensorC同步等待，可以先执行其他逻辑，待需要获取结果时再调用GetTensorC。异步方式可以减少同步等待，提高并行度，开发者对计算性能要求较高时，可以选用该方式。

## 函数原型<a name="section620mcpsimp"></a>

-   获取C矩阵，输出至VECIN

    ```
    template <bool sync = true>
    __aicore__ inline void GetTensorC(const LocalTensor<DstT>& co2Local, uint8_t enAtomic = 0, bool enSequentialWrite = false)
    ```

    -   支持同步模式
    -   支持异步模式

-   获取C矩阵，输出至GM

    ```
    template <bool sync = true>
    __aicore__ inline void GetTensorC(const GlobalTensor<DstT>& gm, uint8_t enAtomic = 0, bool enSequentialWrite = false)
    ```

    -   支持同步模式
    -   支持异步模式

-   获取C矩阵，同时输出至GM和VECIN

    ```
    template <bool sync = true>
    __aicore__ inline void GetTensorC(const GlobalTensor<DstT> &gm, const LocalTensor<DstT> &co2Local, uint8_t enAtomic = 0, bool enSequentialWrite = false)
    ```

    -   支持同步模式
    -   支持异步模式
    -   纯Cube模式（只有矩阵计算）模式暂不支持该接口

-   获取异步场景用于缓存结果的Workspace上的C矩阵，后续使用过程由开发者自行控制

    C矩阵输出到VECIN时，分配给VECIN的Unified Buffer的大小会影响Matmul计算的力度，分配给VECIN的大小过小时，无法充分利用硬件算力。提供该接口支持返回缓存在Workspace上的C矩阵，由开发者自行控制后续使用过程。

    注意，在初始化时，C矩阵的逻辑位置应设置为TPosition::VECIN，调用该接口获取缓存的C矩阵后，自行拷贝到Unified Buffer。

    ```
    template <bool sync = true>
    __aicore__ inline GlobalTensor<DstT> GetTensorC(uint8_t enAtomic = 0, bool enSequentialWrite = false)
    ```

    -   支持异步模式

以下接口中的doPad、height、width、srcGap、dstGap参数待废弃，使用过程中无需传入，保持默认值即可；上文介绍的输出至VECIN的原型实际为不传入默认值的函数原型。

```
template <bool sync = true, bool doPad = false>
__aicore__ inline void GetTensorC(const LocalTensor<DstT>& c, uint8_t enAtomic = 0, bool enSequentialWrite = false, uint32_t height = 0, uint32_t width = 0, uint32_t srcGap = 0, uint32_t dstGap = 0)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table8746171282418"></a>
<table><thead align="left"><tr id="row8746191212419"><th class="cellrowborder" valign="top" width="17.04%" id="mcps1.2.3.1.1"><p id="p474618126245"><a name="p474618126245"></a><a name="p474618126245"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="82.96%" id="mcps1.2.3.1.2"><p id="p1574681216240"><a name="p1574681216240"></a><a name="p1574681216240"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1574617127244"><td class="cellrowborder" valign="top" width="17.04%" headers="mcps1.2.3.1.1 "><p id="p2746171214244"><a name="p2746171214244"></a><a name="p2746171214244"></a>sync</p>
</td>
<td class="cellrowborder" valign="top" width="82.96%" headers="mcps1.2.3.1.2 "><p id="p12551641162414"><a name="p12551641162414"></a><a name="p12551641162414"></a>设置同步或者异步模式：同步模式设置为true；异步模式设置为false。</p>
<p id="p166181831242"><a name="p166181831242"></a><a name="p166181831242"></a><span id="ph161814311418"><a name="ph161814311418"></a><a name="ph161814311418"></a>Ascend 950PR/Ascend 950DT</span>支持异步模式。</p>
<p id="p17790185515333"><a name="p17790185515333"></a><a name="p17790185515333"></a><span id="ph137907556334"><a name="ph137907556334"></a><a name="ph137907556334"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>支持异步模式。</p>
<p id="p1380206322"><a name="p1380206322"></a><a name="p1380206322"></a><span id="ph12801866217"><a name="ph12801866217"></a><a name="ph12801866217"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>支持异步模式。</p>
<p id="p8606483592"><a name="p8606483592"></a><a name="p8606483592"></a><span id="ph166061835918"><a name="ph166061835918"></a><a name="ph166061835918"></a>Kirin X90</span>支持异步模式。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="table9646134355611"></a>
<table><thead align="left"><tr id="row964714433565"><th class="cellrowborder" valign="top" width="17.41%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="9.6%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.99%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row36481043185619"><td class="cellrowborder" valign="top" width="17.41%" headers="mcps1.2.4.1.1 "><p id="p18557238111919"><a name="p18557238111919"></a><a name="p18557238111919"></a>c/co2Local</p>
</td>
<td class="cellrowborder" valign="top" width="9.6%" headers="mcps1.2.4.1.2 "><p id="p1557143861914"><a name="p1557143861914"></a><a name="p1557143861914"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p35571838101917"><a name="p35571838101917"></a><a name="p35571838101917"></a>取出C矩阵到VECIN，数据格式仅支持NZ。</p>
<p id="p2811125958"><a name="p2811125958"></a><a name="p2811125958"></a><span id="ph381114512515"><a name="ph381114512515"></a><a name="ph381114512515"></a>Ascend 950PR/Ascend 950DT</span>支持的数据类型为half、float、bfloat16_t、int32_t、int8_t、fp8_e4m3fn_t、hifloat8_t</p>
<p id="p14555829193812"><a name="p14555829193812"></a><a name="p14555829193812"></a>针对<span id="ph13555329143813"><a name="ph13555329143813"></a><a name="ph13555329143813"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>支持的数据类型为half、float、bfloat16_t、int32_t、int8_t</p>
<p id="p596915528172"><a name="p596915528172"></a><a name="p596915528172"></a>针对<span id="ph64580125327"><a name="ph64580125327"></a><a name="ph64580125327"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>支持的数据类型为half、float、bfloat16_t、int32_t、int8_t</p>
<p id="p1898214319561"><a name="p1898214319561"></a><a name="p1898214319561"></a><span id="ph10819182912570"><a name="ph10819182912570"></a><a name="ph10819182912570"></a>Kirin X90</span>支持的数据类型为half、int8_t、int32_t</p>
<p id="p176214365195"><a name="p176214365195"></a><a name="p176214365195"></a><span id="ph2016514362198"><a name="ph2016514362198"></a><a name="ph2016514362198"></a>Kirin 9030</span>支持的数据类型为half</p>
</td>
</tr>
<tr id="row16491543185617"><td class="cellrowborder" valign="top" width="17.41%" headers="mcps1.2.4.1.1 "><p id="p15571138171913"><a name="p15571138171913"></a><a name="p15571138171913"></a>gm</p>
</td>
<td class="cellrowborder" valign="top" width="9.6%" headers="mcps1.2.4.1.2 "><p id="p17557183815199"><a name="p17557183815199"></a><a name="p17557183815199"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p14557153819197"><a name="p14557153819197"></a><a name="p14557153819197"></a>取出C矩阵到GM，数据格式可以为ND或NZ。</p>
<p id="p17925937152"><a name="p17925937152"></a><a name="p17925937152"></a><span id="ph1092511375511"><a name="ph1092511375511"></a><a name="ph1092511375511"></a>Ascend 950PR/Ascend 950DT</span>支持的数据类型为half、float、bfloat16_t、int32_t、int8_t、fp8_e4m3fn_t、hifloat8_t</p>
<p id="p1338319673916"><a name="p1338319673916"></a><a name="p1338319673916"></a>针对<span id="ph73832653911"><a name="ph73832653911"></a><a name="ph73832653911"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_3"><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_3"><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a>Atlas A3 推理系列产品</term></span>支持的数据类型为half、float、bfloat16_t、int32_t、int8_t</p>
<p id="p19121245143015"><a name="p19121245143015"></a><a name="p19121245143015"></a>针对<span id="ph691214513016"><a name="ph691214513016"></a><a name="ph691214513016"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_3"><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_3"><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a>Atlas A2 推理系列产品</term></span>支持的数据类型为half、float、bfloat16_t、int32_t、int8_t</p>
<p id="p1029023813579"><a name="p1029023813579"></a><a name="p1029023813579"></a><span id="ph9974133865712"><a name="ph9974133865712"></a><a name="ph9974133865712"></a>Kirin X90</span>支持的数据类型为half、int8_t、int32_t</p>
<p id="p20344194661915"><a name="p20344194661915"></a><a name="p20344194661915"></a><span id="ph103441946181915"><a name="ph103441946181915"></a><a name="ph103441946181915"></a>Kirin 9030</span>支持的数据类型为half</p>
</td>
</tr>
<tr id="row6652117123"><td class="cellrowborder" valign="top" width="17.41%" headers="mcps1.2.4.1.1 "><p id="p1543122095916"><a name="p1543122095916"></a><a name="p1543122095916"></a>enAtomic</p>
</td>
<td class="cellrowborder" valign="top" width="9.6%" headers="mcps1.2.4.1.2 "><p id="p18557143813197"><a name="p18557143813197"></a><a name="p18557143813197"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p11557163871916"><a name="p11557163871916"></a><a name="p11557163871916"></a>是否开启Atomic操作，默认值为0<strong id="b1375714215539"><a name="b1375714215539"></a><a name="b1375714215539"></a>。</strong></p>
<p id="p553784617537"><a name="p553784617537"></a><a name="p553784617537"></a>参数取值：</p>
<p id="p19697610154618"><a name="p19697610154618"></a><a name="p19697610154618"></a>0：不开启Atomic操作</p>
<p id="p108028241466"><a name="p108028241466"></a><a name="p108028241466"></a>1：开启AtomicAdd累加操作</p>
<p id="p875272616463"><a name="p875272616463"></a><a name="p875272616463"></a>2：开启AtomicMax求最大值操作</p>
<p id="p337993234616"><a name="p337993234616"></a><a name="p337993234616"></a>3：开启AtomicMin求最小值操作</p>
</td>
</tr>
<tr id="row125553601913"><td class="cellrowborder" valign="top" width="17.41%" headers="mcps1.2.4.1.1 "><p id="p6557838161917"><a name="p6557838161917"></a><a name="p6557838161917"></a>enSequentialWrite</p>
</td>
<td class="cellrowborder" valign="top" width="9.6%" headers="mcps1.2.4.1.2 "><p id="p145578389196"><a name="p145578389196"></a><a name="p145578389196"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p8557183831912"><a name="p8557183831912"></a><a name="p8557183831912"></a>是否开启连续写模式（连续写，写入[baseM, baseN]；非连续写，写入[singleCoreM, singleCoreN]中对应的位置），默认值false（非连续写模式）。</p>
<p id="p84385745317"><a name="p84385745317"></a><a name="p84385745317"></a>注意：非连续写模式，内部会按照迭代顺序算好偏移，开发者不需要关注；如果开发者需要决定排布顺序，可以选择连续写模式，自行按照设定的偏移进行搬运操作。</p>
</td>
</tr>
</tbody>
</table>

**图 1**  非连续写模式示意图<a name="fig322951853119"></a>  
![](figures/非连续写模式示意图.png "非连续写模式示意图")

**图 2**  连续写模式示意图<a name="fig580415103338"></a>  
![](figures/连续写模式示意图.png "连续写模式示意图")

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   传入的C矩阵地址空间大小需要保证不小于baseM \* baseN。
-   异步场景时，需要使用一块临时空间来缓存Iterate计算结果，调用GetTensorC时会在该临时空间中获取C的矩阵分片。临时空间通过[SetWorkspace](SetWorkspace.md)接口进行设置。SetWorkspace接口需要在Iterate接口之前调用。
-   当使能MixDualMaster（双主模式）场景时，即模板参数[enableMixDualMaster](MatmulConfig.md#p9218181073719)设置为true，不支持使用该接口。

## 调用示例<a name="section817251712597"></a>

-   获取C矩阵，输出至VECIN

    ```
    // 同步模式样例
    while (mm.Iterate()) {   
        mm.GetTensorC(ubCmatrix); 
    }
    
    // 异步模式样例
    mm.template Iterate<false>();
    // 其他操作
    for (int i = 0; i < singleM / baseM * singleN / baseN; ++i) {   
        mm.template GetTensorC<false>(ubCmatrix); 
        // 其他操作
    }
    ```

-   获取C矩阵，输出至GM，同步模式样例

    ```
    while (mm.Iterate()) {   
        mm.GetTensorC(gm); 
    }
    ```

-   获取C矩阵，同时输出至GM和VECIN，同步模式样例

    ```
    while (mm.Iterate()) {   
        mm.GetTensorC(gm, ubCmatrix); 
    }
    ```

-   获取API接口返回的GM上的C矩阵，手动拷贝至UB，异步模式样例

    ```
    // BaseM * BaseN = 128 *256
    mm.SetTensorA(gmA);
    mm.SetTensorB(gmB);
    mm.SetTail(singleM, singleN, singleK);
    mm.template Iterate<false>(); 
    // 其他操作
    for (int i = 0; i < singleM / baseM * singleN / baseN; ++i) {  
        // 获取每次计算的BaseM*BaseN的数据128*256 
        GlobalTensor<T> global = mm.template GetTensorC<false>();
        for(int j = 0; j < 4; ++j) {
            LocalTensor local = que.AllocTensor<half>(); // 分配64*128大小的UB空间
            DataCopy(local, global[64 * 128 * i], 64 * 128); // 将GM的数据拷贝进UB中，进行后续的Vector操作
            // 其他Vector 操作
        }
    }
    ```

