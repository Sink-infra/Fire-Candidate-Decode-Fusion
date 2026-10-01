# Matmul使用说明<a name="ZH-CN_TOPIC_0000001570911006"></a>

Ascend C提供一组Matmul高阶API，方便用户快速实现Matmul矩阵乘法的运算操作。

Matmul的计算公式为：C = A \* B + Bias，其示意图如下。

-   A、B为源操作数，A为左矩阵，形状为\[M, K\]；B为右矩阵，形状为\[K, N\]。
-   C为目的操作数，存放矩阵乘结果的矩阵，形状为\[M, N\]。
-   Bias为矩阵乘偏置，形状为\[1, N\]。对A\*B结果矩阵的每一行都采用该Bias进行偏置。

**图 1**  Matmul矩阵乘示意图<a name="fig3161943163113"></a>  
![](figures/Matmul矩阵乘示意图.png "Matmul矩阵乘示意图")

> [!NOTE]说明 
>下文中提及的M轴方向，即为A矩阵纵向；K轴方向，即为A矩阵横向或B矩阵纵向；N轴方向，即为B矩阵横向；尾轴，即为矩阵最后一个维度。

Kernel侧实现Matmul矩阵乘运算的步骤概括为：

1.  创建Matmul对象。
2.  初始化操作。
3.  设置左矩阵A、右矩阵B、Bias。
4.  完成矩阵乘操作。
5.  结束矩阵乘操作。

使用Matmul API实现矩阵乘运算的具体步骤如下：

1.  创建Matmul对象。

    创建Matmul对象的示例如下：

    -   默认为MIX模式（包含矩阵计算和矢量计算），该场景下通常不定义ASCENDC\_CUBE\_ONLY宏，如果在程序中使用了ASCENDC\_CUBE\_ONLY宏，则必须使用ASCEND\_IS\_AIC宏和ASCEND\_IS\_AIV宏将Cube计算和Vector计算隔离开。
    -   纯Cube模式（只有矩阵计算）场景下，建议在代码中定义ASCENDC\_CUBE\_ONLY宏，避免额外的性能开销。

    ```
    // 纯cube模式（只有矩阵计算）场景下，需要设置该代码宏，并且必须在#include "lib/matmul_intf.h"之前设置
    // #define ASCENDC_CUBE_ONLY 
    #include "lib/matmul_intf.h"
    
    typedef AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, half> aType; 
    typedef AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, half> bType; 
    typedef AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, float> cType; 
    typedef AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, float> biasType; 
    AscendC::Matmul<aType, bType, cType, biasType> mm; 
    ```

    创建对象时需要传入A、B、C、Bias的参数类型信息， 类型信息通过[MatmulType](#table1188045714378)来定义，包括：内存逻辑位置、数据格式、数据类型、数据来源的内存逻辑位置。

    ```
    template <AscendC::TPosition POSITION, CubeFormat FORMAT, typename TYPE, bool ISTRANS = false, LayoutMode LAYOUT = LayoutMode::NONE, bool IBSHARE = false, TPosition SRCPOS = TPosition::GM> struct MatmulType {
        constexpr static AscendC::TPosition pos = POSITION;
        constexpr static CubeFormat format = FORMAT;
        using T = TYPE;
        constexpr static bool isTrans = ISTRANS;
        constexpr static LayoutMode layout = LAYOUT;
        constexpr static bool ibShare = IBSHARE;
        constexpr static TPosition srcPos = SRCPOS;
    };
    ```

    **表 1**  MatmulType参数说明

    <a name="table1188045714378"></a>
    <table><thead align="left"><tr id="row1588055783717"><th class="cellrowborder" valign="top" width="17.09%" id="mcps1.2.3.1.1"><p id="p108895312110"><a name="p108895312110"></a><a name="p108895312110"></a>参数</p>
    </th>
    <th class="cellrowborder" valign="top" width="82.91%" id="mcps1.2.3.1.2"><p id="p1588075712372"><a name="p1588075712372"></a><a name="p1588075712372"></a>说明</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="row088010575370"><td class="cellrowborder" valign="top" width="17.09%" headers="mcps1.2.3.1.1 "><p id="p18868503127"><a name="p18868503127"></a><a name="p18868503127"></a>POSITION</p>
    </td>
    <td class="cellrowborder" valign="top" width="82.91%" headers="mcps1.2.3.1.2 "><p id="p11880157123717"><a name="p11880157123717"></a><a name="p11880157123717"></a>内存逻辑位置。</p>
    <p id="p1880057123714"><a name="p1880057123714"></a><a name="p1880057123714"></a>针对<span id="ph1788095712378"><a name="ph1788095712378"></a><a name="ph1788095712378"></a>Ascend 950PR/Ascend 950DT</span>：</p>
    <a name="ul588111575374"></a><a name="ul588111575374"></a><ul id="ul588111575374"><li>A矩阵可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>B矩阵可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>Bias可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>C矩阵可设置为TPosition::GM，TPosition::VECIN</li></ul>
    <p id="p0777611165415"><a name="p0777611165415"></a><a name="p0777611165415"></a>注意，A矩阵、B矩阵或Bias矩阵设置为TPosition::VECOUT或TPosition::TSCM时，对应矩阵用于单核计算的数据必须全部在<span id="ph109961595911"><a name="ph109961595911"></a><a name="ph109961595911"></a>Unified Buffer</span>或<span id="ph1338381018554"><a name="ph1338381018554"></a><a name="ph1338381018554"></a><span id="ph438311085510"><a name="ph438311085510"></a><a name="ph438311085510"></a>L1 Buffer</span></span>上。</p>
    <p id="p688113573378"><a name="p688113573378"></a><a name="p688113573378"></a>针对<span id="ph178811574370"><a name="ph178811574370"></a><a name="ph178811574370"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span>：</p>
    <a name="ul13881557133718"></a><a name="ul13881557133718"></a><ul id="ul13881557133718"><li>A矩阵可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>B矩阵可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>Bias可设置为TPosition::GM，TPosition::VECOUT</li><li>C矩阵可设置为TPosition::GM，TPosition::VECIN, TPosition::CO1</li></ul>
    <p id="p58814571371"><a name="p58814571371"></a><a name="p58814571371"></a>针对<span id="ph7881135712378"><a name="ph7881135712378"></a><a name="ph7881135712378"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span>：</p>
    <a name="ul0881205793717"></a><a name="ul0881205793717"></a><ul id="ul0881205793717"><li>A矩阵可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>B矩阵可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>Bias可设置为TPosition::GM，TPosition::VECOUT</li><li>C矩阵可设置为TPosition::GM，TPosition::VECIN, TPosition::CO1</li></ul>
    <p id="p24261319154111"><a name="p24261319154111"></a><a name="p24261319154111"></a>注意，C矩阵设置为TPosition::CO1时，C矩阵的数据排布格式仅支持CubeFormat::NZ，C矩阵的数据类型仅支持float、int32_t。</p>
    <p id="p9346725237"><a name="p9346725237"></a><a name="p9346725237"></a>针对<span id="ph676816580257"><a name="ph676816580257"></a><a name="ph676816580257"></a>Kirin X90</span>:</p>
    <a name="ul7826194219218"></a><a name="ul7826194219218"></a><ul id="ul7826194219218"><li>A矩阵可设置为TPosition::GM，TPosition::VECOUT，TPosition::TSCM</li><li>B矩阵可设置为TPosition::GM，TPosition::VECOUT, TPosition::TSCM</li><li>Bias可设置为TPosition::GM，TPosition::VECOUT</li><li>C矩阵可设置为TPosition::GM，TPosition::CO1</li></ul>
    <p id="p2525112319217"><a name="p2525112319217"></a><a name="p2525112319217"></a>针对<span id="ph186139377275"><a name="ph186139377275"></a><a name="ph186139377275"></a>Kirin  9030</span>：</p>
    <a name="ul1592215102813"></a><a name="ul1592215102813"></a><ul id="ul1592215102813"><li>A矩阵可设置为TPosition::TSCM</li><li>B矩阵可设置为TPosition::TSCM</li><li>Bias可设置为TPosition::GM</li><li>C矩阵可设置为TPosition::GM</li></ul>
    </td>
    </tr>
    <tr id="row1488214577379"><td class="cellrowborder" valign="top" width="17.09%" headers="mcps1.2.3.1.1 "><p id="p15886145020124"><a name="p15886145020124"></a><a name="p15886145020124"></a>FORMAT</p>
    </td>
    <td class="cellrowborder" valign="top" width="82.91%" headers="mcps1.2.3.1.2 "><p id="p99724910252"><a name="p99724910252"></a><a name="p99724910252"></a>数据的物理排布格式。</p>
    <p id="p13882155720379"><a name="p13882155720379"></a><a name="p13882155720379"></a>针对<span id="ph8882257173717"><a name="ph8882257173717"></a><a name="ph8882257173717"></a>Ascend 950PR/Ascend 950DT</span>：</p>
    <a name="ul0882105783715"></a><a name="ul0882105783715"></a><ul id="ul0882105783715"><li>A矩阵可设置为CubeFormat::ND，CubeFormat::NZ，CubeFormat::COLUMN_MAJOR，CubeFormat::VECTOR</li><li>B矩阵可设置为CubeFormat::ND，CubeFormat::NZ，CubeFormat::COLUMN_MAJOR</li><li>Bias可设置为CubeFormat::ND</li><li>C矩阵可设置为CubeFormat::ND，CubeFormat::NZ，CubeFormat::ND_ALIGN，CubeFormat::COLUMN_MAJOR</li></ul>
    <p id="p6549174513344"><a name="p6549174513344"></a><a name="p6549174513344"></a>针对<span id="ph198841357193714"><a name="ph198841357193714"></a><a name="ph198841357193714"></a>Ascend 950PR/Ascend 950DT</span>，请注意：</p>
    <a name="ul7779101112358"></a><a name="ul7779101112358"></a><ul id="ul7779101112358"><li>仅在非MxMatmul场景中，A、B、C矩阵Format支持CubeFormat::COLUMN_MAJOR。当Format为CubeFormat::COLUMN_MAJOR时，对应矩阵仅支持内存逻辑位置为TPosition::GM。</li><li>输入A矩阵或B矩阵设置为TPosition::TSCM时，对应的Format仅支持CubeFormat::NZ。</li><li>C矩阵设置为TPosition::VECIN，CubeFormat::ND时，要求尾轴32字节对齐，比如数据类型是half的情况下，N要求是16的倍数。</li></ul>
    <p id="p19882165713712"><a name="p19882165713712"></a><a name="p19882165713712"></a>针对<span id="ph2883057143719"><a name="ph2883057143719"></a><a name="ph2883057143719"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>：</p>
    <a name="ul38831357143718"></a><a name="ul38831357143718"></a><ul id="ul38831357143718"><li>A矩阵可设置为CubeFormat::ND，CubeFormat::NZ, CubeFormat::VECTOR</li><li>B矩阵可设置为CubeFormat::ND，CubeFormat::NZ</li><li>Bias可设置为CubeFormat::ND</li><li>C矩阵可设置为CubeFormat::ND，CubeFormat::NZ，CubeFormat::ND_ALIGN</li></ul>
    <p id="p1288365783712"><a name="p1288365783712"></a><a name="p1288365783712"></a>针对<span id="ph19883115713710"><a name="ph19883115713710"></a><a name="ph19883115713710"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>：</p>
    <a name="ul3883185773710"></a><a name="ul3883185773710"></a><ul id="ul3883185773710"><li>A矩阵可设置为CubeFormat::ND，CubeFormat::NZ, CubeFormat::VECTOR</li><li>B矩阵可设置为CubeFormat::ND，CubeFormat::NZ</li><li>Bias可设置为CubeFormat::ND</li><li>C矩阵可设置为CubeFormat::ND，CubeFormat::NZ，CubeFormat::ND_ALIGN</li></ul>
    <p id="p413154614334"><a name="p413154614334"></a><a name="p413154614334"></a>针对<span id="ph3131154663310"><a name="ph3131154663310"></a><a name="ph3131154663310"></a>Kirin X90</span>:</p>
    <a name="ul6131124611337"></a><a name="ul6131124611337"></a><ul id="ul6131124611337"><li>A矩阵可设置为CubeFormat::ND，CubeFormat::NZ, CubeFormat::VECTOR</li><li>B矩阵可设置为CubeFormat::ND，CubeFormat::NZ</li><li>Bias可设置为CubeFormat::ND</li><li>C矩阵可设置为CubeFormat::ND，CubeFormat::NZ，CubeFormat::ND_ALIGN</li></ul>
    <div class="p" id="p1013184618337"><a name="p1013184618337"></a><a name="p1013184618337"></a>针对<span id="ph16131546133316"><a name="ph16131546133316"></a><a name="ph16131546133316"></a>Kirin  9030</span>：<a name="ul7597101117369"></a><a name="ul7597101117369"></a><ul id="ul7597101117369"><li>A矩阵可设置为CubeFormat::NZ，CubeFormat::VECTOR</li><li>B矩阵可设置为CubeFormat::NZ</li><li>Bias可设置为CubeFormat::ND</li><li>C矩阵可设置为CubeFormat::ND</li></ul>
    </div>
    <p id="p671644313140"><a name="p671644313140"></a><a name="p671644313140"></a>关于CubeFormat::NZ格式的A矩阵、B矩阵、C矩阵的对齐约束，请参考<a href="#table98851538118">表3</a>。</p>
    </td>
    </tr>
    <tr id="row16884145713371"><td class="cellrowborder" valign="top" width="17.09%" headers="mcps1.2.3.1.1 "><p id="p128866505123"><a name="p128866505123"></a><a name="p128866505123"></a>TYPE</p>
    </td>
    <td class="cellrowborder" valign="top" width="82.91%" headers="mcps1.2.3.1.2 "><p id="p374213782918"><a name="p374213782918"></a><a name="p374213782918"></a>数据类型。</p>
    <p id="p1884125723716"><a name="p1884125723716"></a><a name="p1884125723716"></a>针对<span id="ph138841457123711"><a name="ph138841457123711"></a><a name="ph138841457123711"></a>Ascend 950PR/Ascend 950DT</span>：</p>
    <a name="ul8884135718378"></a><a name="ul8884135718378"></a><ul id="ul8884135718378"><li>非MxMatmul场景：<a name="ul10364422162510"></a><a name="ul10364422162510"></a><ul id="ul10364422162510"><li>A矩阵可设置为half、float、bfloat16_t 、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t</li><li>B矩阵可设置为half、float、bfloat16_t 、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t</li><li>Bias可设置为half、float、int32_t、bfloat16_t</li><li>C矩阵可设置为half、float、bfloat16_t、int32_t、int8_t、fp8_e4m3fn_t、hifloat8_t</li></ul>
    </li><li>MxMatmul场景：<a name="ul131211327162512"></a><a name="ul131211327162512"></a><ul id="ul131211327162512"><li>A矩阵可设置为fp8_e4m3fn_t、fp8_e5m2_t、fp4x2_e2m1_t、fp4x2_e1m2_t</li><li>B矩阵可设置为fp8_e4m3fn_t、fp8_e5m2_t、fp4x2_e2m1_t、fp4x2_e1m2_t</li><li>Bias可设置为half、float、bfloat16_t</li><li>C矩阵可设置为half、float、bfloat16_t</li></ul>
    </li></ul>
    <div class="p" id="p88856572370"><a name="p88856572370"></a><a name="p88856572370"></a>针对<span id="ph4885185793710"><a name="ph4885185793710"></a><a name="ph4885185793710"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_2"><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_2"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_2"><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a><a name="zh-cn_topic_0000001312391781_term131434243115_2"></a>Atlas A3 推理系列产品</term></span>：<a name="ul1188518573370"></a><a name="ul1188518573370"></a><ul id="ul1188518573370"><li>A矩阵可设置为half、float、bfloat16_t 、int8_t、int4b_t</li><li>B矩阵可设置为half、float、bfloat16_t 、int8_t、int4b_t</li><li>Bias可设置为half、float、int32_t</li><li>C矩阵可设置为half、float、bfloat16_t、int32_t、int8_t</li></ul>
    </div>
    <div class="p" id="p138851257153713"><a name="p138851257153713"></a><a name="p138851257153713"></a>针对<span id="ph7885257133719"><a name="ph7885257133719"></a><a name="ph7885257133719"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_2"><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_2"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_2"><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a><a name="zh-cn_topic_0000001312391781_term184716139811_2"></a>Atlas A2 推理系列产品</term></span>：<a name="ul198858571377"></a><a name="ul198858571377"></a><ul id="ul198858571377"><li>A矩阵可设置为half、float、bfloat16_t 、int8_t、int4b_t</li><li>B矩阵可设置为half、float、bfloat16_t 、int8_t、int4b_t</li><li>Bias可设置为half、float、int32_t</li><li>C矩阵可设置为half、float、bfloat16_t、int32_t、int8_t</li></ul>
    </div>
    <a name="ul4885757173710"></a><a name="ul4885757173710"></a>
    <p id="p29613202398"><a name="p29613202398"></a><a name="p29613202398"></a>针对<span id="ph1896192053919"><a name="ph1896192053919"></a><a name="ph1896192053919"></a>Kirin X90</span>:</p>
    <a name="ul06053965010"></a><a name="ul06053965010"></a><ul id="ul06053965010"><li>A矩阵可设置为half、int8_t</li><li>B矩阵可设置为half、int8_t</li><li>Bias可设置为half、int32_t</li><li>C矩阵可设置为half、int32_t、int8_t</li></ul>
    <p id="p2990153173911"><a name="p2990153173911"></a><a name="p2990153173911"></a>针对<span id="ph1599023119399"><a name="ph1599023119399"></a><a name="ph1599023119399"></a>Kirin  9030</span>：</p>
    <a name="ul5933130135517"></a><a name="ul5933130135517"></a><ul id="ul5933130135517"><li>A矩阵可设置为half</li><li>B矩阵可设置为half</li><li>Bias可设置为half</li><li>C矩阵可设置为half</li></ul>
    <p id="p6886457163710"><a name="p6886457163710"></a><a name="p6886457163710"></a><strong id="b7886175793718"><a name="b7886175793718"></a><a name="b7886175793718"></a>注意：除fp8_e4m3fn_t/fp8_e5m2_t两种数据类型、B矩阵为int8_t数据类型外，A矩阵和B矩阵数据类型需要一致，具体数据类型组合关系请参考</strong><a href="#table1996113269499">表2</a>。A矩阵和B矩阵为int4b_t数据类型时，矩阵内轴的数据个数必须为偶数。例如，A矩阵为int4b_t数据类型且不转置时，<a href="TCubeTiling结构体.md#p11899125875617">singleCoreK</a>必须是偶数。</p>
    </td>
    </tr>
    <tr id="row198861357173716"><td class="cellrowborder" valign="top" width="17.09%" headers="mcps1.2.3.1.1 "><p id="p84551411817"><a name="p84551411817"></a><a name="p84551411817"></a>ISTRANS</p>
    </td>
    <td class="cellrowborder" valign="top" width="82.91%" headers="mcps1.2.3.1.2 "><p id="p4886185713714"><a name="p4886185713714"></a><a name="p4886185713714"></a>是否开启支持矩阵转置的功能。</p>
    <a name="ul1388645718375"></a><a name="ul1388645718375"></a><ul id="ul1388645718375"><li>true：开启支持矩阵转置的功能，运行时可以分别通过<a href="SetTensorA.md">SetTensorA</a>和<a href="SetTensorB.md">SetTensorB</a>中的isTransposeA、isTransposeB参数设置A、B矩阵是否转置。若设置A、B矩阵转置，Matmul会认为A矩阵形状为[K, M]，B矩阵形状为[N, K]。</li><li>false：默认值，不开启支持矩阵转置的功能，通过<a href="SetTensorA.md">SetTensorA</a>和<a href="SetTensorB.md">SetTensorB</a>不能设置A、B矩阵的转置情况。Matmul会认为A矩阵形状为[M, K]，B矩阵形状为[K, N]。</li></ul>
    <p id="p25351314131118"><a name="p25351314131118"></a><a name="p25351314131118"></a>注意，由于<span id="ph173181828090"><a name="ph173181828090"></a><a name="ph173181828090"></a><span id="ph14318162813918"><a name="ph14318162813918"></a><a name="ph14318162813918"></a>L1 Buffer</span></span>上的矩阵数据有分形对齐的约束，A、B矩阵转置和不转置时所需的L1空间可能不相同，在开启支持矩阵转置功能时，必须保证按照<a href="TCubeTiling结构体.md">Matmul Tiling参数</a>申请的L1空间不超过<span id="ph20110352101813"><a name="ph20110352101813"></a><a name="ph20110352101813"></a><span id="ph411015210181"><a name="ph411015210181"></a><a name="ph411015210181"></a>L1 Buffer</span></span>的规格，判断方式为(depthA1*Ceil(baseM/c0Size)*baseK + depthB1*Ceil(baseN/c0Size)*baseK) * db * sizeoof(dtype) &lt; L1Size，db表示L1是否开启double buffer，取值1（不开启double buffer）或2（开启double buffer），其余参数的含义请参考<a href="TCubeTiling结构体.md#table1563162142915">表1</a>。</p>
    <p id="p77636451395"><a name="p77636451395"></a><a name="p77636451395"></a><span id="ph18478174612915"><a name="ph18478174612915"></a><a name="ph18478174612915"></a>Kirin  9030</span>不支持此参数。</p>
    </td>
    </tr>
    <tr id="row1488675713371"><td class="cellrowborder" valign="top" width="17.09%" headers="mcps1.2.3.1.1 "><p id="p127582139543"><a name="p127582139543"></a><a name="p127582139543"></a>LAYOUT</p>
    </td>
    <td class="cellrowborder" valign="top" width="82.91%" headers="mcps1.2.3.1.2 "><p id="p14887957103713"><a name="p14887957103713"></a><a name="p14887957103713"></a>表征数据的排布。</p>
    <p id="p1887165763713"><a name="p1887165763713"></a><a name="p1887165763713"></a>NONE：默认值，表示不使用BatchMatmul；其他选项表示使用BatchMatmul。</p>
    <p id="p178871657133710"><a name="p178871657133710"></a><a name="p178871657133710"></a>NORMAL：BMNK的数据排布格式，具体可参考<a href="IterateBatch.md#li536045110115">IterateBatch</a>中对该数据排布的介绍。</p>
    <p id="p1388775703719"><a name="p1388775703719"></a><a name="p1388775703719"></a>BSNGD：原始BSH shape做reshape后的数据排布，具体可参考<a href="IterateBatch.md#li298041002213">IterateBatch</a>中对该数据排布的介绍。</p>
    <p id="p148874573377"><a name="p148874573377"></a><a name="p148874573377"></a>SBNGD：原始SBH shape做reshape后的数据排布，具体可参考<a href="IterateBatch.md#li6785191319227">IterateBatch</a>中对该数据排布的介绍。</p>
    <p id="p888713577371"><a name="p888713577371"></a><a name="p888713577371"></a>BNGS1S2：一般为前两种数据排布进行矩阵乘的输出，S1S2数据连续存放，一个S1S2为一个batch的计算数据，具体可参考<a href="IterateBatch.md#li1922441712222">IterateBatch</a>中对该数据排布的介绍。</p>
    <p id="p7298434453"><a name="p7298434453"></a><a name="p7298434453"></a><span id="ph38138331599"><a name="ph38138331599"></a><a name="ph38138331599"></a>Kirin  9030</span>不支持此参数。</p>
    </td>
    </tr>
    <tr id="row88871857133714"><td class="cellrowborder" valign="top" width="17.09%" headers="mcps1.2.3.1.1 "><p id="p1613334125414"><a name="p1613334125414"></a><a name="p1613334125414"></a>IBSHARE</p>
    </td>
    <td class="cellrowborder" valign="top" width="82.91%" headers="mcps1.2.3.1.2 "><p id="p1288710573379"><a name="p1288710573379"></a><a name="p1288710573379"></a>是否使能IBShare（IntraBlock Share）。IBShare的功能是能够复用<span id="ph988725713374"><a name="ph988725713374"></a><a name="ph988725713374"></a><span id="ph488711573374"><a name="ph488711573374"></a><a name="ph488711573374"></a>L1 Buffer</span></span>上相同的A矩阵或B矩阵数据，复用的矩阵必须在<span id="ph6401203815911"><a name="ph6401203815911"></a><a name="ph6401203815911"></a><span id="ph440113388919"><a name="ph440113388919"></a><a name="ph440113388919"></a>L1 Buffer</span></span>上全载。A矩阵和B矩阵仅有一个使能IBShare的场景，与<a href="MatmulConfig.md#table6981133810309">IBShare模板</a>配合使用，具体参数设置详见<a href="MatmulConfig.md#table1761013213153">表2</a>。</p>
    <p id="p58873575373"><a name="p58873575373"></a><a name="p58873575373"></a>注意，A矩阵和B矩阵同时使能IBShare的场景，表示<span id="ph12906134517152"><a name="ph12906134517152"></a><a name="ph12906134517152"></a><span id="ph16906945101512"><a name="ph16906945101512"></a><a name="ph16906945101512"></a>L1 Buffer</span></span>上的A矩阵和B矩阵同时复用，需要满足：</p>
    <a name="ul38871557143711"></a><a name="ul38871557143711"></a><ul id="ul38871557143711"><li>同一算子中其它Matmul对象的A矩阵和B矩阵也必须同时使能IBShare；</li><li><span id="ph68351959122717"><a name="ph68351959122717"></a><a name="ph68351959122717"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_3"><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_3"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_3"><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a><a name="zh-cn_topic_0000001312391781_term184716139811_3"></a>Atlas A2 推理系列产品</term></span>，获取矩阵计算结果时，只支持调用<a href="IterateAll.md">IterateAll</a>接口，且只支持输出到GlobalTensor，即计算结果放置于Global Memory的地址。</li><li><span id="ph10607560287"><a name="ph10607560287"></a><a name="ph10607560287"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_3"><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_3"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_3"><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a><a name="zh-cn_topic_0000001312391781_term131434243115_3"></a>Atlas A3 推理系列产品</term></span>，获取矩阵计算结果时，只支持调用<a href="IterateAll.md">IterateAll</a>接口，且只支持输出到GlobalTensor，即计算结果放置于Global Memory的地址。</li><li><span id="ph15897105112917"><a name="ph15897105112917"></a><a name="ph15897105112917"></a>Ascend 950PR/Ascend 950DT</span>，获取矩阵计算结果时，支持输出到GlobalTensor和LocalTensor。输出到LocalTensor，即计算结果放置于Local Memory的场景，默认实现策略与<a href="MatmulPolicy.md#li1329014255511">SplitM模板策略</a>相同，且仅支持输出数据类型为float，仅支持<a href="MatmulConfig.md#p159827389308">Norm模板</a>。</li></ul>
    <p id="p1260083582416"><a name="p1260083582416"></a><a name="p1260083582416"></a><span id="ph1260083515243"><a name="ph1260083515243"></a><a name="ph1260083515243"></a>Ascend 950PR/Ascend 950DT</span>支持该参数。</p>
    <p id="p10924173410583"><a name="p10924173410583"></a><a name="p10924173410583"></a><span id="ph10924103465815"><a name="ph10924103465815"></a><a name="ph10924103465815"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_4"><a name="zh-cn_topic_0000001312391781_term1253731311225_4"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_4"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_4"><a name="zh-cn_topic_0000001312391781_term131434243115_4"></a><a name="zh-cn_topic_0000001312391781_term131434243115_4"></a>Atlas A3 推理系列产品</term></span>支持该参数。</p>
    <p id="p19924634105816"><a name="p19924634105816"></a><a name="p19924634105816"></a><span id="ph19248344581"><a name="ph19248344581"></a><a name="ph19248344581"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_4"><a name="zh-cn_topic_0000001312391781_term11962195213215_4"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_4"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_4"><a name="zh-cn_topic_0000001312391781_term184716139811_4"></a><a name="zh-cn_topic_0000001312391781_term184716139811_4"></a>Atlas A2 推理系列产品</term></span>支持该参数。</p>
    <p id="p8135721367"><a name="p8135721367"></a><a name="p8135721367"></a><span id="ph11876125575"><a name="ph11876125575"></a><a name="ph11876125575"></a>Kirin X90</span>不支持此参数。</p>
    <p id="p11287139273"><a name="p11287139273"></a><a name="p11287139273"></a><span id="ph435218391589"><a name="ph435218391589"></a><a name="ph435218391589"></a>Kirin  9030</span>不支持此参数。</p>
    </td>
    </tr>
    <tr id="row6887957193712"><td class="cellrowborder" valign="top" width="17.09%" headers="mcps1.2.3.1.1 "><p id="p25525734011"><a name="p25525734011"></a><a name="p25525734011"></a>SRC_POSITION</p>
    </td>
    <td class="cellrowborder" valign="top" width="82.91%" headers="mcps1.2.3.1.2 "><p id="p12887145716372"><a name="p12887145716372"></a><a name="p12887145716372"></a>A/B矩阵的POSITION参数配置为TPosition::TSCM时，必须要设置TSCM中矩阵数据的来源的内存逻辑位置，默认为TPosition::GM。</p>
    <p id="p108871574376"><a name="p108871574376"></a><a name="p108871574376"></a>针对<span id="ph1788714572375"><a name="ph1788714572375"></a><a name="ph1788714572375"></a>Ascend 950PR/Ascend 950DT</span>：</p>
    <a name="ul588715572373"></a><a name="ul588715572373"></a><ul id="ul588715572373"><li>A矩阵可设置为TPosition::GM，TPosition::VECOUT</li><li>B矩阵可设置为TPosition::GM，TPosition::VECOUT</li></ul>
    </td>
    </tr>
    </tbody>
    </table>

    **表 2**  Matmul输入输出数据类型的支持列表

    <a name="table1996113269499"></a>
    <table><thead align="left"><tr id="row14961182654919"><th class="cellrowborder" valign="top" width="17.88%" id="mcps1.2.6.1.1"><p id="p1696192654916"><a name="p1696192654916"></a><a name="p1696192654916"></a>A矩阵</p>
    </th>
    <th class="cellrowborder" valign="top" width="16.37%" id="mcps1.2.6.1.2"><p id="p1796116269498"><a name="p1796116269498"></a><a name="p1796116269498"></a>B矩阵</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.639999999999999%" id="mcps1.2.6.1.3"><p id="p196172610496"><a name="p196172610496"></a><a name="p196172610496"></a>Bias</p>
    </th>
    <th class="cellrowborder" valign="top" width="14.430000000000001%" id="mcps1.2.6.1.4"><p id="p12961122616491"><a name="p12961122616491"></a><a name="p12961122616491"></a>C矩阵</p>
    </th>
    <th class="cellrowborder" valign="top" width="37.68%" id="mcps1.2.6.1.5"><p id="p484471411911"><a name="p484471411911"></a><a name="p484471411911"></a>支持平台</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="row1196162615492"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p996152614492"><a name="p996152614492"></a><a name="p996152614492"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p1997111258524"><a name="p1997111258524"></a><a name="p1997111258524"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p9961142614915"><a name="p9961142614915"></a><a name="p9961142614915"></a>float/half</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p99621226104910"><a name="p99621226104910"></a><a name="p99621226104910"></a>float/half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p6741161315362"><a name="p6741161315362"></a><a name="p6741161315362"></a><span id="ph7601132251110"><a name="ph7601132251110"></a><a name="ph7601132251110"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p8741101363614"><a name="p8741101363614"></a><a name="p8741101363614"></a><span id="ph9394123345614"><a name="ph9394123345614"></a><a name="ph9394123345614"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_5"><a name="zh-cn_topic_0000001312391781_term1253731311225_5"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_5"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_5"><a name="zh-cn_topic_0000001312391781_term131434243115_5"></a><a name="zh-cn_topic_0000001312391781_term131434243115_5"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p474251315369"><a name="p474251315369"></a><a name="p474251315369"></a><span id="ph045515502448"><a name="ph045515502448"></a><a name="ph045515502448"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_5"><a name="zh-cn_topic_0000001312391781_term11962195213215_5"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_5"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_5"><a name="zh-cn_topic_0000001312391781_term184716139811_5"></a><a name="zh-cn_topic_0000001312391781_term184716139811_5"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row199621026164912"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p1296202624918"><a name="p1296202624918"></a><a name="p1296202624918"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p159621926184911"><a name="p159621926184911"></a><a name="p159621926184911"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p196212613495"><a name="p196212613495"></a><a name="p196212613495"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p296222664917"><a name="p296222664917"></a><a name="p296222664917"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p21323201361"><a name="p21323201361"></a><a name="p21323201361"></a><span id="ph997219018128"><a name="ph997219018128"></a><a name="ph997219018128"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p3823191693616"><a name="p3823191693616"></a><a name="p3823191693616"></a><span id="ph18238371566"><a name="ph18238371566"></a><a name="ph18238371566"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_6"><a name="zh-cn_topic_0000001312391781_term1253731311225_6"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_6"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_6"><a name="zh-cn_topic_0000001312391781_term131434243115_6"></a><a name="zh-cn_topic_0000001312391781_term131434243115_6"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p118241163365"><a name="p118241163365"></a><a name="p118241163365"></a><span id="ph8457252154413"><a name="ph8457252154413"></a><a name="ph8457252154413"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_6"><a name="zh-cn_topic_0000001312391781_term11962195213215_6"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_6"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_6"><a name="zh-cn_topic_0000001312391781_term184716139811_6"></a><a name="zh-cn_topic_0000001312391781_term184716139811_6"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row244475111124"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p86982059161214"><a name="p86982059161214"></a><a name="p86982059161214"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p669885914123"><a name="p669885914123"></a><a name="p669885914123"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p169835911123"><a name="p169835911123"></a><a name="p169835911123"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p86983595127"><a name="p86983595127"></a><a name="p86983595127"></a>float/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p15166202793611"><a name="p15166202793611"></a><a name="p15166202793611"></a><span id="ph67572052124"><a name="ph67572052124"></a><a name="ph67572052124"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p14980102423617"><a name="p14980102423617"></a><a name="p14980102423617"></a><span id="ph18730108105716"><a name="ph18730108105716"></a><a name="ph18730108105716"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_7"><a name="zh-cn_topic_0000001312391781_term1253731311225_7"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_7"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_7"><a name="zh-cn_topic_0000001312391781_term131434243115_7"></a><a name="zh-cn_topic_0000001312391781_term131434243115_7"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p4980112414365"><a name="p4980112414365"></a><a name="p4980112414365"></a><span id="ph569845911214"><a name="ph569845911214"></a><a name="ph569845911214"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_7"><a name="zh-cn_topic_0000001312391781_term11962195213215_7"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_7"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_7"><a name="zh-cn_topic_0000001312391781_term184716139811_7"></a><a name="zh-cn_topic_0000001312391781_term184716139811_7"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row81081424532"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p810824219538"><a name="p810824219538"></a><a name="p810824219538"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p6109194213530"><a name="p6109194213530"></a><a name="p6109194213530"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p71091442185313"><a name="p71091442185313"></a><a name="p71091442185313"></a>int32_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p010934245319"><a name="p010934245319"></a><a name="p010934245319"></a>int32_t/half</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p12397831183617"><a name="p12397831183617"></a><a name="p12397831183617"></a><span id="ph01503912123"><a name="ph01503912123"></a><a name="ph01503912123"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p195778346369"><a name="p195778346369"></a><a name="p195778346369"></a><span id="ph66293110575"><a name="ph66293110575"></a><a name="ph66293110575"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_8"><a name="zh-cn_topic_0000001312391781_term1253731311225_8"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_8"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_8"><a name="zh-cn_topic_0000001312391781_term131434243115_8"></a><a name="zh-cn_topic_0000001312391781_term131434243115_8"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p145771734133619"><a name="p145771734133619"></a><a name="p145771734133619"></a><span id="ph3487175413443"><a name="ph3487175413443"></a><a name="ph3487175413443"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_8"><a name="zh-cn_topic_0000001312391781_term11962195213215_8"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_8"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_8"><a name="zh-cn_topic_0000001312391781_term184716139811_8"></a><a name="zh-cn_topic_0000001312391781_term184716139811_8"></a>Atlas A2 推理系列产品</term></span></p>
    <p id="p857753453617"><a name="p857753453617"></a><a name="p857753453617"></a><span id="ph1085913317124"><a name="ph1085913317124"></a><a name="ph1085913317124"></a>Kirin X90</span></p>
    </td>
    </tr>
    <tr id="row561656124819"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p1471141017486"><a name="p1471141017486"></a><a name="p1471141017486"></a>int4b_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p135313377483"><a name="p135313377483"></a><a name="p135313377483"></a>int4b_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p12717102489"><a name="p12717102489"></a><a name="p12717102489"></a>int32_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p727113223242"><a name="p727113223242"></a><a name="p727113223242"></a>int32_t/half</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p11809153718368"><a name="p11809153718368"></a><a name="p11809153718368"></a><span id="ph76269145575"><a name="ph76269145575"></a><a name="ph76269145575"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_9"><a name="zh-cn_topic_0000001312391781_term1253731311225_9"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_9"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_9"><a name="zh-cn_topic_0000001312391781_term131434243115_9"></a><a name="zh-cn_topic_0000001312391781_term131434243115_9"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p28091337123616"><a name="p28091337123616"></a><a name="p28091337123616"></a><span id="ph189325714442"><a name="ph189325714442"></a><a name="ph189325714442"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_9"><a name="zh-cn_topic_0000001312391781_term11962195213215_9"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_9"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_9"><a name="zh-cn_topic_0000001312391781_term184716139811_9"></a><a name="zh-cn_topic_0000001312391781_term184716139811_9"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row68030432129"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p10184164871215"><a name="p10184164871215"></a><a name="p10184164871215"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p1018444861219"><a name="p1018444861219"></a><a name="p1018444861219"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p10184348131214"><a name="p10184348131214"></a><a name="p10184348131214"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p14184194801213"><a name="p14184194801213"></a><a name="p14184194801213"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p9485154013619"><a name="p9485154013619"></a><a name="p9485154013619"></a><span id="ph896771451214"><a name="ph896771451214"></a><a name="ph896771451214"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p84361144123617"><a name="p84361144123617"></a><a name="p84361144123617"></a><span id="ph08421725717"><a name="ph08421725717"></a><a name="ph08421725717"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_10"><a name="zh-cn_topic_0000001312391781_term1253731311225_10"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_10"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_10"><a name="zh-cn_topic_0000001312391781_term131434243115_10"></a><a name="zh-cn_topic_0000001312391781_term131434243115_10"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p17436144113613"><a name="p17436144113613"></a><a name="p17436144113613"></a><span id="ph17184848161211"><a name="ph17184848161211"></a><a name="ph17184848161211"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_10"><a name="zh-cn_topic_0000001312391781_term11962195213215_10"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_10"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_10"><a name="zh-cn_topic_0000001312391781_term184716139811_10"></a><a name="zh-cn_topic_0000001312391781_term184716139811_10"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row3804111619153"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p171231421101515"><a name="p171231421101515"></a><a name="p171231421101515"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p2012311212155"><a name="p2012311212155"></a><a name="p2012311212155"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p1812362115151"><a name="p1812362115151"></a><a name="p1812362115151"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p112322191520"><a name="p112322191520"></a><a name="p112322191520"></a>float/half</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p14896174913369"><a name="p14896174913369"></a><a name="p14896174913369"></a><span id="ph11322121714128"><a name="ph11322121714128"></a><a name="ph11322121714128"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p1426194733610"><a name="p1426194733610"></a><a name="p1426194733610"></a><span id="ph14186972017"><a name="ph14186972017"></a><a name="ph14186972017"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_11"><a name="zh-cn_topic_0000001312391781_term1253731311225_11"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_11"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_11"><a name="zh-cn_topic_0000001312391781_term131434243115_11"></a><a name="zh-cn_topic_0000001312391781_term131434243115_11"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p92712472369"><a name="p92712472369"></a><a name="p92712472369"></a><span id="ph1112322121511"><a name="ph1112322121511"></a><a name="ph1112322121511"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_11"><a name="zh-cn_topic_0000001312391781_term11962195213215_11"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_11"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_11"><a name="zh-cn_topic_0000001312391781_term184716139811_11"></a><a name="zh-cn_topic_0000001312391781_term184716139811_11"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row13751942806"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p163762427019"><a name="p163762427019"></a><a name="p163762427019"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p1137634216010"><a name="p1137634216010"></a><a name="p1137634216010"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p937616423018"><a name="p937616423018"></a><a name="p937616423018"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p1337613425012"><a name="p1337613425012"></a><a name="p1337613425012"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p17661115363615"><a name="p17661115363615"></a><a name="p17661115363615"></a><span id="ph1860514195124"><a name="ph1860514195124"></a><a name="ph1860514195124"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p1959025603615"><a name="p1959025603615"></a><a name="p1959025603615"></a><span id="ph110110105014"><a name="ph110110105014"></a><a name="ph110110105014"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_12"><a name="zh-cn_topic_0000001312391781_term1253731311225_12"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_12"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_12"><a name="zh-cn_topic_0000001312391781_term131434243115_12"></a><a name="zh-cn_topic_0000001312391781_term131434243115_12"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p759025623612"><a name="p759025623612"></a><a name="p759025623612"></a><span id="ph1019411144516"><a name="ph1019411144516"></a><a name="ph1019411144516"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_12"><a name="zh-cn_topic_0000001312391781_term11962195213215_12"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_12"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_12"><a name="zh-cn_topic_0000001312391781_term184716139811_12"></a><a name="zh-cn_topic_0000001312391781_term184716139811_12"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row46043713615"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p71282013183619"><a name="p71282013183619"></a><a name="p71282013183619"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p7128913113616"><a name="p7128913113616"></a><a name="p7128913113616"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p512841363618"><a name="p512841363618"></a><a name="p512841363618"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p212821353618"><a name="p212821353618"></a><a name="p212821353618"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p85135598367"><a name="p85135598367"></a><a name="p85135598367"></a><span id="ph8624192110126"><a name="ph8624192110126"></a><a name="ph8624192110126"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p271911273714"><a name="p271911273714"></a><a name="p271911273714"></a><span id="ph18690181217011"><a name="ph18690181217011"></a><a name="ph18690181217011"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_13"><a name="zh-cn_topic_0000001312391781_term1253731311225_13"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_13"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_13"><a name="zh-cn_topic_0000001312391781_term131434243115_13"></a><a name="zh-cn_topic_0000001312391781_term131434243115_13"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p771922153710"><a name="p771922153710"></a><a name="p771922153710"></a><span id="ph18128101303615"><a name="ph18128101303615"></a><a name="ph18128101303615"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_13"><a name="zh-cn_topic_0000001312391781_term11962195213215_13"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_13"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_13"><a name="zh-cn_topic_0000001312391781_term184716139811_13"></a><a name="zh-cn_topic_0000001312391781_term184716139811_13"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row2050214177187"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p194813307182"><a name="p194813307182"></a><a name="p194813307182"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p1481530191812"><a name="p1481530191812"></a><a name="p1481530191812"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p13481730201817"><a name="p13481730201817"></a><a name="p13481730201817"></a>int32_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p18481123012185"><a name="p18481123012185"></a><a name="p18481123012185"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p41891770379"><a name="p41891770379"></a><a name="p41891770379"></a><span id="ph20446132461211"><a name="ph20446132461211"></a><a name="ph20446132461211"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p943110913373"><a name="p943110913373"></a><a name="p943110913373"></a><span id="ph119018351105"><a name="ph119018351105"></a><a name="ph119018351105"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_14"><a name="zh-cn_topic_0000001312391781_term1253731311225_14"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_14"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_14"><a name="zh-cn_topic_0000001312391781_term131434243115_14"></a><a name="zh-cn_topic_0000001312391781_term131434243115_14"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p843112903717"><a name="p843112903717"></a><a name="p843112903717"></a><span id="ph18481203081820"><a name="ph18481203081820"></a><a name="ph18481203081820"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_14"><a name="zh-cn_topic_0000001312391781_term11962195213215_14"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_14"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_14"><a name="zh-cn_topic_0000001312391781_term184716139811_14"></a><a name="zh-cn_topic_0000001312391781_term184716139811_14"></a>Atlas A2 推理系列产品</term></span></p>
    <p id="p1243120983710"><a name="p1243120983710"></a><a name="p1243120983710"></a><span id="ph08811473123"><a name="ph08811473123"></a><a name="ph08811473123"></a>Kirin X90</span></p>
    </td>
    </tr>
    <tr id="row10732163714419"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p121620444411"><a name="p121620444411"></a><a name="p121620444411"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p21694411410"><a name="p21694411410"></a><a name="p21694411410"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p116174484120"><a name="p116174484120"></a><a name="p116174484120"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p51684419413"><a name="p51684419413"></a><a name="p51684419413"></a>half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p1828115124377"><a name="p1828115124377"></a><a name="p1828115124377"></a><span id="ph12756426101210"><a name="ph12756426101210"></a><a name="ph12756426101210"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p8501815173716"><a name="p8501815173716"></a><a name="p8501815173716"></a><span id="ph08601838605"><a name="ph08601838605"></a><a name="ph08601838605"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_15"><a name="zh-cn_topic_0000001312391781_term1253731311225_15"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_15"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_15"><a name="zh-cn_topic_0000001312391781_term131434243115_15"></a><a name="zh-cn_topic_0000001312391781_term131434243115_15"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p85071513373"><a name="p85071513373"></a><a name="p85071513373"></a><span id="ph191644411410"><a name="ph191644411410"></a><a name="ph191644411410"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_15"><a name="zh-cn_topic_0000001312391781_term11962195213215_15"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_15"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_15"><a name="zh-cn_topic_0000001312391781_term184716139811_15"></a><a name="zh-cn_topic_0000001312391781_term184716139811_15"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row279202617185"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p279312262184"><a name="p279312262184"></a><a name="p279312262184"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p3793192621810"><a name="p3793192621810"></a><a name="p3793192621810"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p13793226131815"><a name="p13793226131815"></a><a name="p13793226131815"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p15793122681812"><a name="p15793122681812"></a><a name="p15793122681812"></a>half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p1844211853712"><a name="p1844211853712"></a><a name="p1844211853712"></a><span id="ph72010298126"><a name="ph72010298126"></a><a name="ph72010298126"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p595992016378"><a name="p595992016378"></a><a name="p595992016378"></a><span id="ph184551144704"><a name="ph184551144704"></a><a name="ph184551144704"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_16"><a name="zh-cn_topic_0000001312391781_term1253731311225_16"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_16"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_16"><a name="zh-cn_topic_0000001312391781_term131434243115_16"></a><a name="zh-cn_topic_0000001312391781_term131434243115_16"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p189591920193710"><a name="p189591920193710"></a><a name="p189591920193710"></a><span id="ph1415714731914"><a name="ph1415714731914"></a><a name="ph1415714731914"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_16"><a name="zh-cn_topic_0000001312391781_term11962195213215_16"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_16"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_16"><a name="zh-cn_topic_0000001312391781_term184716139811_16"></a><a name="zh-cn_topic_0000001312391781_term184716139811_16"></a>Atlas A2 推理系列产品</term></span></p>
    <p id="p1895911202379"><a name="p1895911202379"></a><a name="p1895911202379"></a><span id="ph171873262101"><a name="ph171873262101"></a><a name="ph171873262101"></a>Kirin X90</span></p>
    <p id="p1595922043711"><a name="p1595922043711"></a><a name="p1595922043711"></a><span id="ph081875221011"><a name="ph081875221011"></a><a name="ph081875221011"></a>Kirin  9030</span></p>
    </td>
    </tr>
    <tr id="row842765934119"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p1767833425"><a name="p1767833425"></a><a name="p1767833425"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p187671338429"><a name="p187671338429"></a><a name="p187671338429"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p1776714314422"><a name="p1776714314422"></a><a name="p1776714314422"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p1276763164213"><a name="p1276763164213"></a><a name="p1276763164213"></a>bfloat16_t/half</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p54151924193715"><a name="p54151924193715"></a><a name="p54151924193715"></a><span id="ph94201731141218"><a name="ph94201731141218"></a><a name="ph94201731141218"></a>Ascend 950PR/Ascend 950DT</span></p>
    <p id="p153512273379"><a name="p153512273379"></a><a name="p153512273379"></a><span id="ph27638471019"><a name="ph27638471019"></a><a name="ph27638471019"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_17"><a name="zh-cn_topic_0000001312391781_term1253731311225_17"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_17"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_17"><a name="zh-cn_topic_0000001312391781_term131434243115_17"></a><a name="zh-cn_topic_0000001312391781_term131434243115_17"></a>Atlas A3 推理系列产品</term></span></p>
    <p id="p535172713711"><a name="p535172713711"></a><a name="p535172713711"></a><span id="ph77671833424"><a name="ph77671833424"></a><a name="ph77671833424"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_17"><a name="zh-cn_topic_0000001312391781_term11962195213215_17"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_17"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_17"><a name="zh-cn_topic_0000001312391781_term184716139811_17"></a><a name="zh-cn_topic_0000001312391781_term184716139811_17"></a>Atlas A2 推理系列产品</term></span></p>
    </td>
    </tr>
    <tr id="row19662125710427"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p1835141611538"><a name="p1835141611538"></a><a name="p1835141611538"></a>fp8_e4m3fn_t/fp8_e5m2_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p19662165744215"><a name="p19662165744215"></a><a name="p19662165744215"></a>fp8_e4m3fn_t/fp8_e5m2_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p116624576422"><a name="p116624576422"></a><a name="p116624576422"></a>float/half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p966315574422"><a name="p966315574422"></a><a name="p966315574422"></a>fp8_e4m3fn_t/half/bfloat16_t/float</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p145689448374"><a name="p145689448374"></a><a name="p145689448374"></a><span id="ph166811461397"><a name="ph166811461397"></a><a name="ph166811461397"></a>Ascend 950PR/Ascend 950DT</span></p>
    </td>
    </tr>
    <tr id="row10694732183320"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p1769419324338"><a name="p1769419324338"></a><a name="p1769419324338"></a>hifloat8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p669483217333"><a name="p669483217333"></a><a name="p669483217333"></a>hifloat8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p16947328332"><a name="p16947328332"></a><a name="p16947328332"></a>float/half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p9694232183318"><a name="p9694232183318"></a><a name="p9694232183318"></a>hifloat8_t/half/bfloat16_t/float</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p770619462377"><a name="p770619462377"></a><a name="p770619462377"></a><span id="ph19379183743412"><a name="ph19379183743412"></a><a name="ph19379183743412"></a>Ascend 950PR/Ascend 950DT</span></p>
    </td>
    </tr>
    <tr id="row0950158367"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p79515157366"><a name="p79515157366"></a><a name="p79515157366"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p139581533614"><a name="p139581533614"></a><a name="p139581533614"></a>float</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p1095151533616"><a name="p1095151533616"></a><a name="p1095151533616"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p10816181554510"><a name="p10816181554510"></a><a name="p10816181554510"></a>float/half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p17735104817378"><a name="p17735104817378"></a><a name="p17735104817378"></a><span id="ph1654195363712"><a name="ph1654195363712"></a><a name="ph1654195363712"></a>Ascend 950PR/Ascend 950DT</span></p>
    </td>
    </tr>
    <tr id="row13271018193615"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p14271718143612"><a name="p14271718143612"></a><a name="p14271718143612"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p12721816360"><a name="p12721816360"></a><a name="p12721816360"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p3271184366"><a name="p3271184366"></a><a name="p3271184366"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p123674166436"><a name="p123674166436"></a><a name="p123674166436"></a>float/half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p118481250133711"><a name="p118481250133711"></a><a name="p118481250133711"></a><span id="ph188793433914"><a name="ph188793433914"></a><a name="ph188793433914"></a>Ascend 950PR/Ascend 950DT</span></p>
    </td>
    </tr>
    <tr id="row6691183710396"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p1169103763916"><a name="p1169103763916"></a><a name="p1169103763916"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p14692143763915"><a name="p14692143763915"></a><a name="p14692143763915"></a>half</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p569219377391"><a name="p569219377391"></a><a name="p569219377391"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p1772811020459"><a name="p1772811020459"></a><a name="p1772811020459"></a>float/half/bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p7971115283716"><a name="p7971115283716"></a><a name="p7971115283716"></a><span id="ph32784494409"><a name="ph32784494409"></a><a name="ph32784494409"></a>Ascend 950PR/Ascend 950DT</span></p>
    </td>
    </tr>
    <tr id="row1850414073910"><td class="cellrowborder" valign="top" width="17.88%" headers="mcps1.2.6.1.1 "><p id="p0505194018391"><a name="p0505194018391"></a><a name="p0505194018391"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.37%" headers="mcps1.2.6.1.2 "><p id="p450554063919"><a name="p450554063919"></a><a name="p450554063919"></a>int8_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.639999999999999%" headers="mcps1.2.6.1.3 "><p id="p19505154018392"><a name="p19505154018392"></a><a name="p19505154018392"></a>int32_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.430000000000001%" headers="mcps1.2.6.1.4 "><p id="p195055407390"><a name="p195055407390"></a><a name="p195055407390"></a>bfloat16_t</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.68%" headers="mcps1.2.6.1.5 "><p id="p183919556376"><a name="p183919556376"></a><a name="p183919556376"></a><span id="ph1220175011402"><a name="ph1220175011402"></a><a name="ph1220175011402"></a>Ascend 950PR/Ascend 950DT</span></p>
    </td>
    </tr>
    </tbody>
    </table>

2.  初始化操作。

    ```
    REGIST_MATMUL_OBJ(&pipe, GetSysWorkSpacePtr(), mm, &tiling); // 初始化matmul对象，参数含义请参考REGIST_MATMUL_OBJ章节
    ```

3.  设置左矩阵A、右矩阵B、Bias。

    ```
    mm.SetTensorA(gm_a);    // 设置左矩阵A
    mm.SetTensorB(gm_b);    // 设置右矩阵B
    mm.SetBias(gm_bias);    // 设置Bias
    
    ```

4.  完成矩阵乘操作。

    用户可以选择以下三种调用方式之一。

    -   调用[Iterate](Iterate.md#li135771283591)完成单次迭代计算，叠加while循环完成单核全量数据的计算。Iterate方式，可以自行控制迭代次数，完成所需数据量的计算，方式比较灵活。

        ```
        // API接口内部会进行循环结束条件判断处理
        while (mm.Iterate()) {   
            mm.GetTensorC(gm_c); 
        }
        ```

    -   调用[IterateAll](IterateAll.md)完成单核上所有数据的计算。IterateAll方式，无需循环迭代，使用比较简单。

        ```
        mm.IterateAll(gm_c);
        ```

    -   用户申请用于存放矩阵乘结果的逻辑位置CO1内存，调用一次或多次[Iterate](Iterate.md#li4843165185812)完成单次或多次迭代计算，在需要搬出计算结果时，调用[Fixpipe](Fixpipe.md)接口完成CO1上计算结果的搬运，然后释放申请的CO1内存。该方式下，用户可以灵活控制计算和搬运的节奏，根据实际需要，一次计算对应一次结果的搬出，或者将多次计算结果缓存在CO1内存中，再一次性搬出计算结果。

        在此种调用方式下，创建Matmul对象时，必须定义C矩阵的内存逻辑位置为TPosition::CO1、数据排布格式为CubeFormat::NZ、数据类型为float或int32\_t。

        -   Ascend 950PR/Ascend 950DT暂不支持该方式。

        ```
        // 定义C矩阵的类型信息
        typedef AscendC::MatmulType<AscendC::TPosition::CO1, CubeFormat::NZ, float> cType;
        // 创建Matmul对象
        AscendC::Matmul<aType, bType, cType, biasType> mm; 
        
        // 用户提前申请CO1的内存l0cTensor
        TQue<TPosition::CO1, 1> CO1_;
        // 128 * 1024为申请的CO1内存大小
        GetTPipePtr()->InitBuffer(CO1_, 1, 128 * 1024);
        // L0cT为C矩阵的数据类型。
        // A矩阵数据类型是int8_t或int4b_t时，C矩阵的数据类型是int32_t。
        // A矩阵数据类型是half、float或bfloat16_t时，C矩阵的数据类型是float。
        LocalTensor<L0cT> l0cTensor = CO1_.template AllocTensor<L0cT>();
        
        // 将l0cTensor作为入参传入Iterate，矩阵乘结果输出到用户申请的l0cTensor上
        mm.Iterate(false, l0cTensor);
        
        // 调用Fixpipe接口将CO1上的计算结果搬运到GM
        FixpipeParamsV220 params;
        params.nSize = nSize;
        params.mSize = mSize;
        params.srcStride = srcStride;
        params.dstStride = dstStride;
        CO1_.EnQue(l0cTensor);
        CO1_.template DeQue<L0cT>();
        Fixpipe<cType, L0cT, CFG_ROW_MAJOR>(gm[dstOffset], l0cTensor, params);
        
        //释放CO1内存
        CO1_.FreeTensor(l0cTensor);
        ```

5.  结束矩阵乘操作。

    ```
    mm.End();
    ```

**表 3**  CubeFormat::NZ格式的矩阵对齐要求

<a name="table98851538118"></a>
<table><thead align="left"><tr id="row7885231715"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p188858315118"><a name="p188858315118"></a><a name="p188858315118"></a>源/目的操作数</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p168851531917"><a name="p168851531917"></a><a name="p168851531917"></a>外轴</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p988573014"><a name="p988573014"></a><a name="p988573014"></a>内轴</p>
</th>
</tr>
</thead>
<tbody><tr id="row3885731510"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1888510312118"><a name="p1888510312118"></a><a name="p1888510312118"></a>A矩阵/B矩阵</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p788523816"><a name="p788523816"></a><a name="p788523816"></a>16的倍数</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p88851135117"><a name="p88851135117"></a><a name="p88851135117"></a>C0_size的倍数</p>
</td>
</tr>
<tr id="row748664714916"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p848718471396"><a name="p848718471396"></a><a name="p848718471396"></a>C矩阵（使能channel_split功能）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p144876471395"><a name="p144876471395"></a><a name="p144876471395"></a>16的倍数</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1948711471198"><a name="p1948711471198"></a><a name="p1948711471198"></a>C0_size的倍数</p>
</td>
</tr>
<tr id="row6135165918117"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p15817171514712"><a name="p15817171514712"></a><a name="p15817171514712"></a>C矩阵（不使能channel_split功能）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p51367599110"><a name="p51367599110"></a><a name="p51367599110"></a>16的倍数</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p5136165918111"><a name="p5136165918111"></a><a name="p5136165918111"></a>float/int32_t：16的倍数</p>
<p id="p1917716523120"><a name="p1917716523120"></a><a name="p1917716523120"></a>half/bfloat16_t/int8_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t：C0_size的倍数</p>
</td>
</tr>
</tbody>
</table>

对于上表中相关参数和概念的补充说明如下：

-   float/int32\_t数据类型的C0\_size为8，half/bfloat16\_t数据类型的C0\_size为16，int8\_t/fp8\_e4m3fn\_t/fp8\_e5m2\_t/hifloat8\_t数据类型的C0\_size为32，int4b\_t/fp4x2\_e2m1\_t/fp4x2\_e1m2\_t数据类型的C0\_size为64。
-   channel\_split功能通过[MatmulConfig](MatmulConfig.md#table1761013213153)中的isEnableChannelSplit参数配置，具体内容请参考[MatmulConfig](MatmulConfig.md#table1761013213153)。

## 需要包含的头文件<a name="section1682364117469"></a>

```
#include "lib/matmul/matmul_intf.h"
```

## 实现原理<a name="section13229175017585"></a>

以输入矩阵A \(GM, ND, half\)、矩阵B\(GM, ND, half\)，输出矩阵C \(GM, ND, float\)，无Bias场景为例，其中\(GM, ND, half\)表示数据存放在GM上，数据格式为ND，数据类型为half，描述Matmul高阶API典型场景的内部算法框图，如下图所示。

**图 2**  Matmul算法框图<a name="fig072411991916"></a>  
![](figures/Matmul算法框图.png "Matmul算法框图")

计算过程分为如下几步：

1.  数据从GM搬到A1：DataCopy每次从矩阵A，搬出一个stepM\*baseM\*stepKa\*baseK的矩阵块a1，循环多次完成矩阵A的搬运；数据从GM搬到B1：DataCopy每次从矩阵B，搬出一个stepKb\*baseK\*stepN\*baseN的矩阵块b1，循环多次完成矩阵B的搬运；
2.  数据从A1搬到A2：LoadData每次从矩阵块a1，搬出一个baseM \* baseK的矩阵块a0；数据从B1搬到B2，并完成转置：LoadData每次从矩阵块b1，搬出一个baseK \* baseN的矩阵块，并将其转置为baseN \* baseK的矩阵块b0；
3.  矩阵乘：每次完成一个矩阵块a0 \* b0的计算，得到baseM \* baseN的矩阵块co1；
4.  数据从矩阵块co1搬到矩阵块co2:DataCopy每次搬运一块baseM \* baseN的矩阵块co1到singleCoreM \* singleCoreN的矩阵块co2中；
5.  重复2-4步骤，完成矩阵块a1 \* b1的计算；
6.  数据从矩阵块co2搬到矩阵块C：DataCopy每次搬运一块singleCoreM \* singleCoreN的矩阵块co2到矩阵块C中；
7.  重复1-6步骤，完成矩阵A \* B = C的计算。

