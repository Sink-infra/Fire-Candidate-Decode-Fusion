# PlatformAscendCManager<a name="ZH-CN_TOPIC_0000002078498914"></a>

## 功能说明<a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_section36583473819"></a>

基于Kernel Launch算子工程，通过Kernel直调（Kernel Launch）方式调用算子的场景下，可能需要获取硬件平台相关信息，比如获取硬件平台的核数。PlatformAscendCManager类提供获取平台信息的功能：通过该类的GetInstance方法可以获取一个PlatformAscendC类的指针，再通过该指针获取硬件平台相关信息，支持获取的信息可参考[PlatformAscendC](PlatformAscendC.md)。

> [!CAUTION]注意 
>-   使用该功能需要包含"tiling/platform/platform\_ascendc.h"头文件，并在编译脚本中链接tiling\_api、platform动态库。
>    -   包含头文件的样例如下：
>        ```
>        #include "tiling/platform/platform_ascendc.h"
>        ```
>    -   链接动态库的样例如下:
>        ```
>        add_executable(main main.cpp)
>        target_link_libraries(main PRIVATE
>          kernels
>          tiling_api
>          platform
>        )
>        ```
>-   当前该类仅支持如下型号：
>    -   Atlas A2 训练系列产品/Atlas A2 推理系列产品
>    -   Atlas A3 训练系列产品/Atlas A3 推理系列产品

## 函数原型<a name="zh-cn_topic_0000001796358754_section7979556121414"></a>

```
class PlatformAscendCManager {
public:
    static PlatformAscendC* GetInstance();
    // 在仅有CPU环境、无对应的NPU硬件环境时，需要传入customSocVersion来指定对应的AI处理器型号。注意：因为GetInstance实现属于单例模式，仅在第一次调用时传入的customSocVersion生效。
    static PlatformAscendC* GetInstance(const char *customSocVersion);
private:
...
}
```

## 参数说明<a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_section189014013619"></a>

<a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_table111938719446"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_row6223476444"><th class="cellrowborder" valign="top" width="17.22%" id="mcps1.1.4.1.1"><p id="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p10223674448"><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p10223674448"></a><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p10223674448"></a>参数</p>
</th>
<th class="cellrowborder" valign="top" width="15.340000000000002%" id="mcps1.1.4.1.2"><p id="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p645511218169"><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p645511218169"></a><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p645511218169"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="67.44%" id="mcps1.1.4.1.3"><p id="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p1922337124411"><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p1922337124411"></a><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p1922337124411"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_row152234713443"><td class="cellrowborder" valign="top" width="17.22%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000001796358754_p4871895189"><a name="zh-cn_topic_0000001796358754_p4871895189"></a><a name="zh-cn_topic_0000001796358754_p4871895189"></a>customSocVersion</p>
</td>
<td class="cellrowborder" valign="top" width="15.340000000000002%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p167701536957"><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p167701536957"></a><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p167701536957"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="67.44%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p4611154016587"><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p4611154016587"></a><a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_p4611154016587"></a>AI处理器型号。</p>
<a name="zh-cn_topic_0000001796358754_ul1124912113117"></a><a name="zh-cn_topic_0000001796358754_ul1124912113117"></a><ul id="zh-cn_topic_0000001796358754_ul1124912113117"><li>针对如下<span id="zh-cn_topic_0000001265392790_ph20833205312295"><a name="zh-cn_topic_0000001265392790_ph20833205312295"></a><a name="zh-cn_topic_0000001265392790_ph20833205312295"></a>产品</span>：在安装<span id="zh-cn_topic_0000001265392790_ph196874123168"><a name="zh-cn_topic_0000001265392790_ph196874123168"></a><a name="zh-cn_topic_0000001265392790_ph196874123168"></a>AI处理器</span>的服务器执行<strong id="zh-cn_topic_0000001265392790_b17687612191618"><a name="zh-cn_topic_0000001265392790_b17687612191618"></a><a name="zh-cn_topic_0000001265392790_b17687612191618"></a>npu-smi info</strong>命令进行查询，获取<strong id="zh-cn_topic_0000001265392790_b10161437131915"><a name="zh-cn_topic_0000001265392790_b10161437131915"></a><a name="zh-cn_topic_0000001265392790_b10161437131915"></a>Name</strong>信息。实际配置值为AscendName，例如<strong id="zh-cn_topic_0000001265392790_b16284944181920"><a name="zh-cn_topic_0000001265392790_b16284944181920"></a><a name="zh-cn_topic_0000001265392790_b16284944181920"></a>Name</strong>取值为<em id="zh-cn_topic_0000001265392790_i1478775919179"><a name="zh-cn_topic_0000001265392790_i1478775919179"></a><a name="zh-cn_topic_0000001265392790_i1478775919179"></a>xxxyy</em>，实际配置值为Ascend<em id="zh-cn_topic_0000001265392790_i1678775901719"><a name="zh-cn_topic_0000001265392790_i1678775901719"></a><a name="zh-cn_topic_0000001265392790_i1678775901719"></a>xxxyy</em>。<p id="zh-cn_topic_0000001265392790_p3529538154519"><a name="zh-cn_topic_0000001265392790_p3529538154519"></a><a name="zh-cn_topic_0000001265392790_p3529538154519"></a><span id="zh-cn_topic_0000001265392790_ph1483216010188"><a name="zh-cn_topic_0000001265392790_ph1483216010188"></a><a name="zh-cn_topic_0000001265392790_ph1483216010188"></a><term id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</li><li>针对如下<span id="zh-cn_topic_0000001265392790_ph13446185695413"><a name="zh-cn_topic_0000001265392790_ph13446185695413"></a><a name="zh-cn_topic_0000001265392790_ph13446185695413"></a>产品</span>，在安装<span id="zh-cn_topic_0000001265392790_ph17911124171120"><a name="zh-cn_topic_0000001265392790_ph17911124171120"></a><a name="zh-cn_topic_0000001265392790_ph17911124171120"></a>AI处理器</span>的服务器执行<strong id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_b206066255591"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_b206066255591"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_b206066255591"></a>npu-smi info -t board -i </strong><em id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_i16609202515915"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_i16609202515915"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_i16609202515915"></a>id</em><strong id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_b14358631175910"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_b14358631175910"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_b14358631175910"></a> -c </strong><em id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_i16269732165915"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_i16269732165915"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001264656721_zh-cn_topic_0000001117597244_i16269732165915"></a>chip_id</em>命令进行查询，获取<strong id="zh-cn_topic_0000001265392790_b11257114917192"><a name="zh-cn_topic_0000001265392790_b11257114917192"></a><a name="zh-cn_topic_0000001265392790_b11257114917192"></a>Chip Name</strong>和<strong id="zh-cn_topic_0000001265392790_b72671651121916"><a name="zh-cn_topic_0000001265392790_b72671651121916"></a><a name="zh-cn_topic_0000001265392790_b72671651121916"></a>NPU Name</strong>信息，实际配置值为Chip Name_NPU Name。例如<strong id="zh-cn_topic_0000001265392790_b13136111611203"><a name="zh-cn_topic_0000001265392790_b13136111611203"></a><a name="zh-cn_topic_0000001265392790_b13136111611203"></a>Chip Name</strong>取值为Ascend<em id="zh-cn_topic_0000001265392790_i68701996189"><a name="zh-cn_topic_0000001265392790_i68701996189"></a><a name="zh-cn_topic_0000001265392790_i68701996189"></a>xxx</em>，<strong id="zh-cn_topic_0000001265392790_b51347352112"><a name="zh-cn_topic_0000001265392790_b51347352112"></a><a name="zh-cn_topic_0000001265392790_b51347352112"></a>NPU Name</strong>取值为1234，实际配置值为Ascend<em id="zh-cn_topic_0000001265392790_i82901912141813"><a name="zh-cn_topic_0000001265392790_i82901912141813"></a><a name="zh-cn_topic_0000001265392790_i82901912141813"></a>xxx</em><em id="zh-cn_topic_0000001265392790_i154501458102213"><a name="zh-cn_topic_0000001265392790_i154501458102213"></a><a name="zh-cn_topic_0000001265392790_i154501458102213"></a>_</em>1234。其中：<a name="zh-cn_topic_0000001265392790_ul2747601334"></a><a name="zh-cn_topic_0000001265392790_ul2747601334"></a><ul id="zh-cn_topic_0000001265392790_ul2747601334"><li>id：设备id，通过<strong id="zh-cn_topic_0000001265392790_b83171930133314"><a name="zh-cn_topic_0000001265392790_b83171930133314"></a><a name="zh-cn_topic_0000001265392790_b83171930133314"></a>npu-smi info -l</strong>命令查出的NPU ID即为设备id。</li><li>chip_id：芯片id，通过<strong id="zh-cn_topic_0000001265392790_b18888204343317"><a name="zh-cn_topic_0000001265392790_b18888204343317"></a><a name="zh-cn_topic_0000001265392790_b18888204343317"></a>npu-smi info -m</strong>命令查出的Chip ID即为芯片id。</li></ul>
<p id="zh-cn_topic_0000001265392790_p1790216395447"><a name="zh-cn_topic_0000001265392790_p1790216395447"></a><a name="zh-cn_topic_0000001265392790_p1790216395447"></a><span id="zh-cn_topic_0000001265392790_ph2272194216543"><a name="zh-cn_topic_0000001265392790_ph2272194216543"></a><a name="zh-cn_topic_0000001265392790_ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
<p id="zh-cn_topic_0000001265392790_p12136131554410"><a name="zh-cn_topic_0000001265392790_p12136131554410"></a><a name="zh-cn_topic_0000001265392790_p12136131554410"></a><span id="zh-cn_topic_0000001265392790_ph13754548217"><a name="zh-cn_topic_0000001265392790_ph13754548217"></a><a name="zh-cn_topic_0000001265392790_ph13754548217"></a><term id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001265392790_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</li></ul>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_section25791320141317"></a>

无

## 约束说明<a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_section19165124931511"></a>

无

## 调用示例<a name="zh-cn_topic_0000001796358754_zh-cn_topic_0000001442758437_section320753512363"></a>

```
GetInfoFun() {
    ...
    auto coreNum = platform_ascendc::PlatformAscendCManager::GetInstance()->GetCoreNum();
    ...
    return;
}
```

