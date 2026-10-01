# SetAtomicAdd<a name="ZH-CN_TOPIC_0000001487536650"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="table38301303189"></a>
<table><thead align="left"><tr id="row20831180131817"><th class="cellrowborder" valign="top" width="58.03%" id="mcps1.1.3.1.1"><p id="p1883113061818"><a name="p1883113061818"></a><a name="p1883113061818"></a><span id="ph20833205312295"><a name="ph20833205312295"></a><a name="ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="41.97%" id="mcps1.1.3.1.2"><p id="p783113012187"><a name="p783113012187"></a><a name="p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row1272474920205"><td class="cellrowborder" valign="top" width="58.03%" headers="mcps1.1.3.1.1 "><p id="p17301775812"><a name="p17301775812"></a><a name="p17301775812"></a><span id="ph2272194216543"><a name="ph2272194216543"></a><a name="ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="41.97%" headers="mcps1.1.3.1.2 "><p id="p37256491200"><a name="p37256491200"></a><a name="p37256491200"></a>√</p>
</td>
</tr>
<tr id="row220181016240"><td class="cellrowborder" valign="top" width="58.03%" headers="mcps1.1.3.1.1 "><p id="p48327011813"><a name="p48327011813"></a><a name="p48327011813"></a><span id="ph583230201815"><a name="ph583230201815"></a><a name="ph583230201815"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="41.97%" headers="mcps1.1.3.1.2 "><p id="p7948163910184"><a name="p7948163910184"></a><a name="p7948163910184"></a>√</p>
</td>
</tr>
<tr id="row173226882415"><td class="cellrowborder" valign="top" width="58.03%" headers="mcps1.1.3.1.1 "><p id="p14832120181815"><a name="p14832120181815"></a><a name="p14832120181815"></a><span id="ph1483216010188"><a name="ph1483216010188"></a><a name="ph1483216010188"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="41.97%" headers="mcps1.1.3.1.2 "><p id="p19948143911820"><a name="p19948143911820"></a><a name="p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

调用该接口后，可对后续的从VECOUT/L0C/L1到GM的数据传输开启原子累加，通过模板参数设定不同的累加数据类型。

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T>
__aicore__ inline void SetAtomicAdd()
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>
<table><thead align="left"><tr id="zh-cn_topic_0235751031_row27598891"><th class="cellrowborder" valign="top" width="8.63%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="91.36999999999999%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row42461942101815"><td class="cellrowborder" valign="top" width="8.63%" headers="mcps1.2.3.1.1 "><p id="p611771320276"><a name="p611771320276"></a><a name="p611771320276"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="91.36999999999999%" headers="mcps1.2.3.1.2 "><p id="p3844958114318"><a name="p3844958114318"></a><a name="p3844958114318"></a>设定不同的累加数据类型。</p>
<p id="p62516108542"><a name="p62516108542"></a><a name="p62516108542"></a><span id="ph1215792313251"><a name="ph1215792313251"></a><a name="ph1215792313251"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持的数据类型为int8_t/int16_t/half/bfloat16_t/int32_t/float；支持的数据通路为VECOUT/L0C/L1-&gt;GM。</p>
<p id="p698211333507"><a name="p698211333507"></a><a name="p698211333507"></a><span id="ph16239174011416"><a name="ph16239174011416"></a><a name="ph16239174011416"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持的数据类型为int8_t/int16_t/half/bfloat16_t/int32_t/float；支持的数据通路为VECOUT/L0C/L1-&gt;GM。</p>
<p id="p468305719192"><a name="p468305719192"></a><a name="p468305719192"></a><span id="ph6381337183714"><a name="ph6381337183714"></a><a name="ph6381337183714"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为int8_t/int16_t/half/bfloat16_t/int32_t/float；支持的数据通路为VECOUT/L0C Buffer-&gt;GM。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   累加操作完成后，建议通过[DisableDmaAtomic](DisableDmaAtomic.md)关闭原子累加，以免影响后续相关指令功能。
-   该指令执行前不会对GM的数据做清零操作，开发者根据实际的算子逻辑判断是否需要清零，如果需要自行进行清零操作。

## 调用示例<a name="section177231425115410"></a>

本示例中，使用DataCopy从VECOUT搬出数据到外部dstGlobal时进行原子累加。为保证原子累加的正确性，在核函数调用前，需要对dstGm清零。

调用核函数时，numBlocks设置为3，核函数调用示例如下：

```
...
// x为输入，z为输出
set_atomic_add_ops_kernel<<<3, nullptr, stream>>>(x, z);
...
```

核函数示例如下：

```
#include "kernel_operator.h"
class KernelSetAtomicAdd {
public:
    __aicore__ inline KernelSetAtomicAdd() {}
    __aicore__ inline void Init(__gm__ uint8_t* src0Gm, __gm__ uint8_t* dstGm)
    {
        src0Global.SetGlobalBuffer((__gm__ float*)src0Gm);
        dstGlobal.SetGlobalBuffer((__gm__ float*)dstGm);
        pipe.InitBuffer(inQueueSrc0, 1, 256 * sizeof(float));
        pipe.InitBuffer(outQueueDst, 1, 256 * sizeof(float));
    }
    __aicore__ inline void Process()
    {
        CopyIn();
        Compute();
        CopyOut();
    }
private:
    __aicore__ inline void CopyIn()
    {}
    __aicore__ inline void Compute()
    {}
    __aicore__ inline void CopyOut()
    {
        AscendC::LocalTensor<float> src0Local = inQueueSrc0.AllocTensor<float>();
        AscendC::DisableDmaAtomic();
        AscendC::DataCopy(src0Local, src0Global, 256);
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(0);
        
        AscendC::SetAtomicAdd<float>();
        AscendC::DataCopy(dstGlobal, src0Local, 256);
        AscendC::DisableDmaAtomic();
        inQueueSrc0.FreeTensor(src0Local);
    }
private:
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueSrc0;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueDst;
    AscendC::GlobalTensor<float> src0Global, dstGlobal;
};
extern "C" __global__ __aicore__ void set_atomic_add_ops_kernel(__gm__ uint8_t* src0Gm, __gm__ uint8_t* dstGm)
{
    KernelSetAtomicAdd op;
    op.Init(src0Gm, dstGm);
    op.Process();
}
```

结果示例如下：

```
每个核的输入数据Src0: [1,1,1,1,1,...,1] // 256个1
最终输出数据dstGm: [3,3,3,3,3,...,3] // 256个3
```

