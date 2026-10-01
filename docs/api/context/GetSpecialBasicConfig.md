# GetSpecialBasicConfig<a name="ZH-CN_TOPIC_0000002072972672"></a>

## 功能说明<a name="section618mcpsimp"></a>

用于配置SpecialBasicBlock模板的参数，获取自定义SpecialBasicBlock模板。当前为预留接口。

## 函数原型<a name="section620mcpsimp"></a>

```
__aicore__ constexpr MatmulConfig GetSpecialBasicConfig(const uint32_t basicM, const uint32_t basicN, const uint32_t basicK, const uint32_t singleCoreM, const uint32_t singleCoreN, const uint32_t singleCoreK, const uint32_t stepM, const uint32_t stepN, const bool intrinsicsLimit = false, const bool batchLoop = false, const BatchMode bmmMode = BatchMode::BATCH_LESS_THAN_L1)
```

## 参数说明<a name="section622mcpsimp"></a>

本接口的所有参数用于设置[MatmulConfig结构体](MatmulConfig.md#table1761013213153)中的参数，其中互相对应的参数的功能作用相同。

**表 1**  接口参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="17.77%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="11.799999999999999%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="70.43%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row14726151462416"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p11726614162415"><a name="p11726614162415"></a><a name="p11726614162415"></a>basicM</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p2072611143247"><a name="p2072611143247"></a><a name="p2072611143247"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p57261114122414"><a name="p57261114122414"></a><a name="p57261114122414"></a>用于设置参数basicM。</p>
<p id="p561173251510"><a name="p561173251510"></a><a name="p561173251510"></a><span id="ph108911580253"><a name="ph108911580253"></a><a name="ph108911580253"></a>与<a href="TCubeTiling结构体.md">TCubeTiling结构体</a>中的baseM参数含义相同，Matmul计算时base块M轴长度，以元素为单位。</span></p>
</td>
</tr>
<tr id="row3821161216246"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p8821141219248"><a name="p8821141219248"></a><a name="p8821141219248"></a>basicN</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p1282141232419"><a name="p1282141232419"></a><a name="p1282141232419"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p98211612172418"><a name="p98211612172418"></a><a name="p98211612172418"></a>用于设置参数basicN。</p>
<p id="p46111132121513"><a name="p46111132121513"></a><a name="p46111132121513"></a><span id="ph781319299267"><a name="ph781319299267"></a><a name="ph781319299267"></a>与<a href="TCubeTiling结构体.md">TCubeTiling结构体</a>中的baseN参数含义相同，Matmul计算时base块N轴长度，以元素为单位。</span></p>
</td>
</tr>
<tr id="row156808105249"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p3680151013243"><a name="p3680151013243"></a><a name="p3680151013243"></a>basicK</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p6680191019241"><a name="p6680191019241"></a><a name="p6680191019241"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p156801210162411"><a name="p156801210162411"></a><a name="p156801210162411"></a>用于设置参数basicK。</p>
<p id="p11611932111512"><a name="p11611932111512"></a><a name="p11611932111512"></a><span id="ph186378112276"><a name="ph186378112276"></a><a name="ph186378112276"></a>与<a href="TCubeTiling结构体.md">TCubeTiling结构体</a>中的baseK参数含义相同，Matmul计算时base块K轴长度，以元素为单位。</span></p>
</td>
</tr>
<tr id="row17814105682915"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p1181511561291"><a name="p1181511561291"></a><a name="p1181511561291"></a>singleCoreM</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p11815556182912"><a name="p11815556182912"></a><a name="p11815556182912"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p1881516563298"><a name="p1881516563298"></a><a name="p1881516563298"></a>用于设置参数singleCoreM。</p>
<p id="p16121732161510"><a name="p16121732161510"></a><a name="p16121732161510"></a><span id="ph142788325310"><a name="ph142788325310"></a><a name="ph142788325310"></a>单核内M轴shape大小，以元素为单位。</span></p>
</td>
</tr>
<tr id="row1379113540298"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p979195414292"><a name="p979195414292"></a><a name="p979195414292"></a>singleCoreN</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p10791554142911"><a name="p10791554142911"></a><a name="p10791554142911"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p279155492910"><a name="p279155492910"></a><a name="p279155492910"></a>用于设置参数singleCoreN。</p>
<p id="p86122032181510"><a name="p86122032181510"></a><a name="p86122032181510"></a><span id="ph1237918363215"><a name="ph1237918363215"></a><a name="ph1237918363215"></a>单核内N轴shape大小，以元素为单位。</span></p>
</td>
</tr>
<tr id="row9844152142912"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p3846105217293"><a name="p3846105217293"></a><a name="p3846105217293"></a>singleCoreK</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p0846752182910"><a name="p0846752182910"></a><a name="p0846752182910"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p484625282912"><a name="p484625282912"></a><a name="p484625282912"></a>用于设置参数singleCoreK。</p>
<p id="p961243217151"><a name="p961243217151"></a><a name="p961243217151"></a><span id="ph9173174013214"><a name="ph9173174013214"></a><a name="ph9173174013214"></a>单核内K轴shape大小，以元素为单位。</span></p>
</td>
</tr>
<tr id="row1925735018290"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p5257125011291"><a name="p5257125011291"></a><a name="p5257125011291"></a>stepM</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p1525820509291"><a name="p1525820509291"></a><a name="p1525820509291"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p4258145072918"><a name="p4258145072918"></a><a name="p4258145072918"></a>用于设置参数stepM。</p>
<p id="p1461283212151"><a name="p1461283212151"></a><a name="p1461283212151"></a><span id="ph179599146332"><a name="ph179599146332"></a><a name="ph179599146332"></a>左矩阵在A1中缓存的bufferM方向上baseM的倍数。</span></p>
</td>
</tr>
<tr id="row73182487290"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p63186484293"><a name="p63186484293"></a><a name="p63186484293"></a>stepN</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p731824817292"><a name="p731824817292"></a><a name="p731824817292"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p53191748192912"><a name="p53191748192912"></a><a name="p53191748192912"></a>用于设置参数stepN。</p>
<p id="p12612143291517"><a name="p12612143291517"></a><a name="p12612143291517"></a><span id="ph414535033316"><a name="ph414535033316"></a><a name="ph414535033316"></a>右矩阵在B1中缓存的bufferN方向上baseN的倍数。</span></p>
</td>
</tr>
<tr id="row42461942101815"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p163481714145518"><a name="p163481714145518"></a><a name="p163481714145518"></a>intrinsicsLimit</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p33487148556"><a name="p33487148556"></a><a name="p33487148556"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p1147594373717"><a name="p1147594373717"></a><a name="p1147594373717"></a>用于设置参数intrinsicsCheck。</p>
<p id="p19428173891615"><a name="p19428173891615"></a><a name="p19428173891615"></a><span id="ph31274661320"><a name="ph31274661320"></a><a name="ph31274661320"></a>当左矩阵或右矩阵在单核上内轴（即尾轴）大于等于65535（元素个数）时，是否使能循环执行数据从<span id="zh-cn_topic_0000002013427316_ph610031519596"><a name="zh-cn_topic_0000002013427316_ph610031519596"></a><a name="zh-cn_topic_0000002013427316_ph610031519596"></a>Global Memory</span>到<span id="zh-cn_topic_0000002013427316_ph6551115913423"><a name="zh-cn_topic_0000002013427316_ph6551115913423"></a><a name="zh-cn_topic_0000002013427316_ph6551115913423"></a><span id="zh-cn_topic_0000002013427316_ph455120597421"><a name="zh-cn_topic_0000002013427316_ph455120597421"></a><a name="zh-cn_topic_0000002013427316_ph455120597421"></a>L1 Buffer</span></span>的搬入。例如，左矩阵A[M, K]，单核上的内轴数据singleCoreK大于65535，配置该参数为true后，API内部通过循环执行数据的搬入。参数取值如下：</span></p>
<a name="ul143331631192217"></a><a name="ul143331631192217"></a><ul id="ul143331631192217"><li><span id="ph19135469139"><a name="ph19135469139"></a><a name="ph19135469139"></a>false：当左矩阵或右矩阵在单核上内轴大于等于65535时，不使能循环执行数据的搬入（默认值）。</span></li><li><span id="ph514346131313"><a name="ph514346131313"></a><a name="ph514346131313"></a>true：当左矩阵或右矩阵在单核上内轴大于等于65535时，使能循环执行数据的搬入。</span></li></ul>
<p id="p1075824511418"><a name="p1075824511418"></a><a name="p1075824511418"></a><span id="ph197581045191417"><a name="ph197581045191417"></a><a name="ph197581045191417"></a>Kirin X90</span>仅支持默认值。</p>
</td>
</tr>
<tr id="row1282014916166"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p14807165981614"><a name="p14807165981614"></a><a name="p14807165981614"></a>batchLoop</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p0807115911613"><a name="p0807115911613"></a><a name="p0807115911613"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p154881241113711"><a name="p154881241113711"></a><a name="p154881241113711"></a>用于设置参数isNBatch。</p>
<p id="p176131132121514"><a name="p176131132121514"></a><a name="p176131132121514"></a><span id="ph1391761111166"><a name="ph1391761111166"></a><a name="ph1391761111166"></a>是否多Batch输入多Batch输出。仅对BatchMatmul有效，使能该参数后，仅支持Norm模板，且需调用<a href="IterateNBatch.md">IterateNBatch</a>实现多Batch输入多Batch输出。参数取值如下：</span></p>
<a name="ul261310324151"></a><a name="ul261310324151"></a><ul id="ul261310324151"><li><span id="ph7918911141611"><a name="ph7918911141611"></a><a name="ph7918911141611"></a>false：不使能多Batch（默认值）。</span></li><li><span id="ph3919141191616"><a name="ph3919141191616"></a><a name="ph3919141191616"></a>true：使能多Batch。</span><p id="p18362134218423"><a name="p18362134218423"></a><a name="p18362134218423"></a><span id="ph3362542104213"><a name="ph3362542104213"></a><a name="ph3362542104213"></a>Kirin X90</span>仅支持默认值。</p>
</li></ul>
</td>
</tr>
<tr id="row13809141083811"><td class="cellrowborder" valign="top" width="17.77%" headers="mcps1.2.4.1.1 "><p id="p7810910113819"><a name="p7810910113819"></a><a name="p7810910113819"></a>bmmMode</p>
</td>
<td class="cellrowborder" valign="top" width="11.799999999999999%" headers="mcps1.2.4.1.2 "><p id="p120714113815"><a name="p120714113815"></a><a name="p120714113815"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="70.43%" headers="mcps1.2.4.1.3 "><p id="p481014105388"><a name="p481014105388"></a><a name="p481014105388"></a>用于设置参数batchMode。该参数用于BatchMatmul场景。</p>
<p id="p059314332299"><a name="p059314332299"></a><a name="p059314332299"></a><span id="ph115931633172912"><a name="ph115931633172912"></a><a name="ph115931633172912"></a>BatchMatmul场景中Layout类型为NORMAL时，设置BatchMatmul输入A/B矩阵的多batch数据总和与<span id="zh-cn_topic_0000002013427316_ph6593193372910"><a name="zh-cn_topic_0000002013427316_ph6593193372910"></a><a name="zh-cn_topic_0000002013427316_ph6593193372910"></a>L1 Buffer</span>的大小关系。参数取值如下：</span></p>
<a name="ul10593183316296"></a><a name="ul10593183316296"></a><ul id="ul10593183316296"><li><span id="ph5593733142910"><a name="ph5593733142910"></a><a name="ph5593733142910"></a>BatchMode::BATCH_LESS_THAN_L1：多batch数据总和&lt;<span id="zh-cn_topic_0000002013427316_ph145931033102914"><a name="zh-cn_topic_0000002013427316_ph145931033102914"></a><a name="zh-cn_topic_0000002013427316_ph145931033102914"></a>L1 Buffer</span> Size；</span></li><li><span id="ph25931334295"><a name="ph25931334295"></a><a name="ph25931334295"></a>BatchMode::BATCH_LARGE_THAN_L1：多batch数据总和&gt;<span id="zh-cn_topic_0000002013427316_ph10593333142912"><a name="zh-cn_topic_0000002013427316_ph10593333142912"></a><a name="zh-cn_topic_0000002013427316_ph10593333142912"></a>L1 Buffer</span> Size；</span></li><li><span id="ph9593203310298"><a name="ph9593203310298"></a><a name="ph9593203310298"></a>BatchMode::SINGLE_LARGE_THAN_L1：单batch数据总和&gt;<span id="zh-cn_topic_0000002013427316_ph259363362918"><a name="zh-cn_topic_0000002013427316_ph259363362918"></a><a name="zh-cn_topic_0000002013427316_ph259363362918"></a>L1 Buffer</span> Size。</span></li></ul>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

[MatmulConfig结构体](MatmulConfig.md#table1761013213153)。

## 约束说明<a name="section633mcpsimp"></a>

无

