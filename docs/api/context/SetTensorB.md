# SetTensorB<a name="ZH-CN_TOPIC_0000001621512217"></a>

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
<tr id="row684613241156"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p1384614241357"><a name="p1384614241357"></a><a name="p1384614241357"></a><span id="ph32117402515"><a name="ph32117402515"></a><a name="ph32117402515"></a>Kirin X90</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p18846524051"><a name="p18846524051"></a><a name="p18846524051"></a>√</p>
</td>
</tr>
<tr id="row11665194710311"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p142462448111"><a name="p142462448111"></a><a name="p142462448111"></a><span id="ph49484268126"><a name="ph49484268126"></a><a name="ph49484268126"></a>Kirin 9030</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p9246184415111"><a name="p9246184415111"></a><a name="p9246184415111"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

设置矩阵乘的右矩阵B。

## 函数原型<a name="section620mcpsimp"></a>

```
__aicore__ inline void SetTensorB(const GlobalTensor<SrcBT>& gm, bool isTransposeB = false)
```

```
__aicore__ inline void SetTensorB(const LocalTensor<SrcBT>& rightMatrix, bool isTransposeB = false)
```

```
__aicore__ inline void SetTensorB(SrcBT bScalar)
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
<tbody><tr id="row14755141911264"><td class="cellrowborder" valign="top" width="19.39%" headers="mcps1.2.3.1.1 "><p id="p47551198266"><a name="p47551198266"></a><a name="p47551198266"></a>SrcBT</p>
</td>
<td class="cellrowborder" valign="top" width="80.61%" headers="mcps1.2.3.1.2 "><p id="p125969172719"><a name="p125969172719"></a><a name="p125969172719"></a>操作数的数据类型。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="table9646134355611"></a>
<table><thead align="left"><tr id="row964714433565"><th class="cellrowborder" valign="top" width="14.99%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="12.02%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.99%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row106481443135617"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p477815376373"><a name="p477815376373"></a><a name="p477815376373"></a>gm</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p1277883718379"><a name="p1277883718379"></a><a name="p1277883718379"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p1490395212411"><a name="p1490395212411"></a><a name="p1490395212411"></a>B矩阵。<span id="ph15942199192220"><a name="ph15942199192220"></a><a name="ph15942199192220"></a><span id="ph1294215916225"><a name="ph1294215916225"></a><a name="ph1294215916225"></a><span id="ph894279182218"><a name="ph894279182218"></a><a name="ph894279182218"></a>类型为<a href="GlobalTensor.md">GlobalTensor</a>。</span></span></span>SrcBT参数表示B矩阵的数据类型。</p>
<p id="p4249131217541"><a name="p4249131217541"></a><a name="p4249131217541"></a><span id="ph1324931265418"><a name="ph1324931265418"></a><a name="ph1324931265418"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half/float/bfloat16_t/int8_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t</p>
<p id="p169694814194"><a name="p169694814194"></a><a name="p169694814194"></a><span id="ph13754548217"><a name="ph13754548217"></a><a name="ph13754548217"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half/float/bfloat16_t/int8_t/int4b_t</p>
<p id="p116811630632"><a name="p116811630632"></a><a name="p116811630632"></a><span id="ph398410521454"><a name="ph398410521454"></a><a name="ph398410521454"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half/float/bfloat16_t/int8_t/int4b_t</p>
<p id="p117992040194119"><a name="p117992040194119"></a><a name="p117992040194119"></a><span id="ph1535441174119"><a name="ph1535441174119"></a><a name="ph1535441174119"></a>Kirin X90</span>，支持的数据类型为：half/int8_t</p>
<p id="p6764121617207"><a name="p6764121617207"></a><a name="p6764121617207"></a><span id="ph1976411682013"><a name="ph1976411682013"></a><a name="ph1976411682013"></a>Kirin 9030</span>，支持的数据类型为：half</p>
</td>
</tr>
<tr id="row36481043185619"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p9779137113719"><a name="p9779137113719"></a><a name="p9779137113719"></a>rightMatrix</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p177993793720"><a name="p177993793720"></a><a name="p177993793720"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p55019672515"><a name="p55019672515"></a><a name="p55019672515"></a>B矩阵。<span id="ph173308471594"><a name="ph173308471594"></a><a name="ph173308471594"></a><span id="ph9902231466"><a name="ph9902231466"></a><a name="ph9902231466"></a><span id="ph1782115034816"><a name="ph1782115034816"></a><a name="ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为TSCM/VECOUT。</span></span></span>SrcBT参数表示B矩阵的数据类型。</p>
<p id="p17842164919549"><a name="p17842164919549"></a><a name="p17842164919549"></a><span id="ph1384274915549"><a name="ph1384274915549"></a><a name="ph1384274915549"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half/float/bfloat16_t/int8_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t</p>
<p id="p61861832113112"><a name="p61861832113112"></a><a name="p61861832113112"></a><span id="ph118683216317"><a name="ph118683216317"></a><a name="ph118683216317"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half/float/bfloat16_t/int8_t/int4b_t</p>
<p id="p1085283116555"><a name="p1085283116555"></a><a name="p1085283116555"></a><span id="ph10828115135510"><a name="ph10828115135510"></a><a name="ph10828115135510"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half/float/bfloat16_t/int8_t/int4b_t</p>
<p id="p1942985792313"><a name="p1942985792313"></a><a name="p1942985792313"></a><span id="ph19429257182318"><a name="ph19429257182318"></a><a name="ph19429257182318"></a>Kirin X90</span>，支持的数据类型为：half/int8_t</p>
<p id="p94291757142316"><a name="p94291757142316"></a><a name="p94291757142316"></a><span id="ph742965716232"><a name="ph742965716232"></a><a name="ph742965716232"></a>Kirin 9030</span>，支持的数据类型为：half</p>
<p id="p1446474164516"><a name="p1446474164516"></a><a name="p1446474164516"></a>若设置TSCM首地址，默认矩阵可全载，已经位于TSCM，Iterate接口无需再进行GM-&gt;A1/B1搬运。</p>
</td>
</tr>
<tr id="row544992215412"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p842415112510"><a name="p842415112510"></a><a name="p842415112510"></a>bScalar</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p11424251182517"><a name="p11424251182517"></a><a name="p11424251182517"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p133243216288"><a name="p133243216288"></a><a name="p133243216288"></a>B矩阵中设置的值。支持传入标量数据，标量数据会被扩展为一个形状为[1, K]的tensor参与矩阵乘计算，tensor的数值均为该标量值。例如，开发者可以通过将bScalar设置为1来实现矩阵A在K方向的reduce sum操作。SrcBT参数表示B矩阵的数据类型。</p>
<p id="p1142013187554"><a name="p1142013187554"></a><a name="p1142013187554"></a><span id="ph94201318145518"><a name="ph94201318145518"></a><a name="ph94201318145518"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：half/float</p>
<p id="p84480413314"><a name="p84480413314"></a><a name="p84480413314"></a><span id="ph6448641113116"><a name="ph6448641113116"></a><a name="ph6448641113116"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_3"><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_3"><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为：half/float</p>
<p id="p533219326285"><a name="p533219326285"></a><a name="p533219326285"></a><span id="ph12627102319413"><a name="ph12627102319413"></a><a name="ph12627102319413"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_3"><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_3"><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为：half/float</p>
<p id="p1017710018241"><a name="p1017710018241"></a><a name="p1017710018241"></a><span id="ph9177200192412"><a name="ph9177200192412"></a><a name="ph9177200192412"></a>Kirin X90</span>，支持的数据类型为：half</p>
<p id="p417718015246"><a name="p417718015246"></a><a name="p417718015246"></a><span id="ph81774072416"><a name="ph81774072416"></a><a name="ph81774072416"></a>Kirin 9030</span>，支持的数据类型为：half</p>
</td>
</tr>
<tr id="row16491543185617"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p577933710373"><a name="p577933710373"></a><a name="p577933710373"></a>isTransposeB</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p677953710371"><a name="p677953710371"></a><a name="p677953710371"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p1777915376375"><a name="p1777915376375"></a><a name="p1777915376375"></a>B矩阵是否需要转置。</p>
<p id="p19435554154514"><a name="p19435554154514"></a><a name="p19435554154514"></a><strong id="b18435155413453"><a name="b18435155413453"></a><a name="b18435155413453"></a>注意：</strong></p>
<a name="ul9388365274"></a><a name="ul9388365274"></a><ul id="ul9388365274"><li>若B矩阵MatmulType的ISTRANS参数设置为true，该参数可以为true也可以为false，即运行时可以转置和非转置交替使用；</li><li>若B矩阵MatmulType的ISTRANS参数设置为false，该参数只能设置为false，若强行设置为true，精度会有异常；</li><li>对于非half、非bfloat16_t输入类型的场景，为了确保Tiling侧与Kernel侧<span id="ph456826193017"><a name="ph456826193017"></a><a name="ph456826193017"></a>L1 Buffer</span>空间计算大小保持一致及结果精度正确，该参数取值必须与Kernel侧定义B矩阵MatmulType的<a href="Matmul使用说明.md#p84551411817">ISTRANS</a>参数以及Tiling侧SetBType()接口的<a href="SetBType.md#p47369411111">isTrans</a>参数保持一致，即上述三个参数必须同时设置为true或同时设置为false。</li></ul>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

传入的TensorB地址空间大小需要保证不小于singleK \* singleN。

## 调用示例<a name="section1665082013318"></a>

```
REGIST_MATMUL_OBJ(&pipe, GetSysWorkSpacePtr(), mm, &tiling);
mm.SetTensorA(gm_a);
mm.SetTensorB(gm_b);    // 设置右矩阵B
mm.SetBias(gm_bias);
mm.IterateAll(gm_c);
```

