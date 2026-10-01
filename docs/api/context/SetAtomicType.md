# SetAtomicType<a name="ZH-CN_TOPIC_0000001835083633"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="table38301303189"></a>
<table><thead align="left"><tr id="row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p1883113061818"><a name="p1883113061818"></a><a name="p1883113061818"></a><span id="ph20833205312295"><a name="ph20833205312295"></a><a name="ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p783113012187"><a name="p783113012187"></a><a name="p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row1272474920205"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p17301775812"><a name="p17301775812"></a><a name="p17301775812"></a><span id="ph2272194216543"><a name="ph2272194216543"></a><a name="ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p37256491200"><a name="p37256491200"></a><a name="p37256491200"></a>√</p>
</td>
</tr>
<tr id="row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p48327011813"><a name="p48327011813"></a><a name="p48327011813"></a><span id="ph583230201815"><a name="ph583230201815"></a><a name="ph583230201815"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7948163910184"><a name="p7948163910184"></a><a name="p7948163910184"></a>√</p>
</td>
</tr>
<tr id="row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p14832120181815"><a name="p14832120181815"></a><a name="p14832120181815"></a><span id="ph1483216010188"><a name="ph1483216010188"></a><a name="ph1483216010188"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p19948143911820"><a name="p19948143911820"></a><a name="p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

通过设置模板参数来设定原子操作不同的数据类型。

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T>
__aicore__ inline void SetAtomicType()
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
<td class="cellrowborder" valign="top" width="91.36999999999999%" headers="mcps1.2.3.1.2 "><p id="p3844958114318"><a name="p3844958114318"></a><a name="p3844958114318"></a>设定不同的数据类型。</p>
<p id="p815762322517"><a name="p815762322517"></a><a name="p815762322517"></a><span id="ph1215792313251"><a name="ph1215792313251"></a><a name="ph1215792313251"></a><term id="zh-cn_topic_0000001312391781_term11962195213215_1"><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a><a name="zh-cn_topic_0000001312391781_term11962195213215_1"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811_1"><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a><a name="zh-cn_topic_0000001312391781_term184716139811_1"></a>Atlas A2 推理系列产品</term></span>，支持int8_t/int16_t/half/bfloat16_t/int32_t/float</p>
<p id="p523904010149"><a name="p523904010149"></a><a name="p523904010149"></a><span id="ph16239174011416"><a name="ph16239174011416"></a><a name="ph16239174011416"></a><term id="zh-cn_topic_0000001312391781_term1253731311225_1"><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a><a name="zh-cn_topic_0000001312391781_term1253731311225_1"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115_1"><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a><a name="zh-cn_topic_0000001312391781_term131434243115_1"></a>Atlas A3 推理系列产品</term></span>，支持int8_t/int16_t/half/bfloat16_t/int32_t/float</p>
<p id="p10469331113812"><a name="p10469331113812"></a><a name="p10469331113812"></a><span id="ph18469163111387"><a name="ph18469163111387"></a><a name="ph18469163111387"></a>Ascend 950PR/Ascend 950DT</span>, 支持int8_t/int16_t/half/bfloat16_t/int32_t/float</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

需要和[SetAtomicAdd](SetAtomicAdd.md)、[SetAtomicMax](SetAtomicMax(ISASI).md)、[SetAtomicMin](SetAtomicMin(ISASI).md)配合使用。

使用完成后，建议清空原子操作的状态（详见[DisableDmaAtomic](DisableDmaAtomic.md)），以免影响后续相关指令功能。

## 调用示例<a name="section177231425115410"></a>

```
// 本演示示例使用DataCopy从VECOUT搬出到外部dstGlobal时进行原子最小，并调用SetAtomicType修改原子最小的数据类型。
#include "kernel_operator.h"

static const int data_size = 256;
template <typename T>
class KernelDataCopyAtomicMin {
public:
    __aicore__ inline KernelDataCopyAtomicMin() {}
    __aicore__ inline void Init(GM_ADDR src0_gm, GM_ADDR src1_gm, GM_ADDR dst_gm, uint32_t size)
    {
        this->size = size;
        src0Global.SetGlobalBuffer((__gm__ T *)src0_gm);
        src1Global.SetGlobalBuffer((__gm__ T *)src1_gm);
        dstGlobal.SetGlobalBuffer((__gm__ T *)dst_gm);
        pipe.InitBuffer(queueSrc0, 1, size * sizeof(T));
        pipe.InitBuffer(queueSrc1, 1, size * sizeof(T));
        pipe.InitBuffer(queueDst0, 1, size * sizeof(T));
        pipe.InitBuffer(queueDst1, 1, size * sizeof(T));
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
        AscendC::LocalTensor<T> src0local = queueSrc0.AllocTensor<T>();
        AscendC::LocalTensor<T> src1local = queueSrc1.AllocTensor<T>();
        AscendC::DataCopy(src0local, src0Global, size);
        AscendC::DataCopy(src1local, src1Global, size);
        queueSrc0.EnQue(src0local);
        queueSrc1.EnQue(src1local);
    }
    __aicore__ inline void Compute()
    {
        AscendC::LocalTensor<T> src0local = queueSrc0.DeQue<T>();
        AscendC::LocalTensor<T> src1local = queueSrc1.DeQue<T>();
        AscendC::LocalTensor<T> dst0Local = queueDst0.AllocTensor<T>();
        AscendC::LocalTensor<T> dst1Local = queueDst1.AllocTensor<T>();
        AscendC::Abs(dst0Local, src0local, size);
        AscendC::Abs(dst1Local, src1local, size);
        queueDst0.EnQue(dst0Local);
        queueDst1.EnQue(dst1Local);
        queueSrc0.FreeTensor(src0local);
        queueSrc1.FreeTensor(src1local);
    }
    __aicore__ inline void CopyOut()
    {
        AscendC::LocalTensor<T> dst0Local = queueDst0.DeQue<T>();
        AscendC::LocalTensor<T> dst1Local = queueDst1.DeQue<T>();
        AscendC::DataCopy(dstGlobal, dst1Local, size);
        AscendC::PipeBarrier<PIPE_MTE3>();
        AscendC::SetAtomicMin<int8_t>();  // 此处设置的类型可随意，此例中以int8_t为例
        AscendC::SetAtomicType<T>();  // 此处设置真实的数据类型
        AscendC::DataCopy(dstGlobal, dst0Local, size);
        queueDst0.FreeTensor(dst0Local);
        queueDst1.FreeTensor(dst1Local);
        AscendC::DisableDmaAtomic();
    }
private:
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> queueSrc0;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> queueSrc1;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> queueDst0;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> queueDst1;
    AscendC::GlobalTensor<T> src0Global, src1Global, dstGlobal;
    uint32_t size;
};
extern "C" __global__ __aicore__ void data_copy_atomic_min_kernel(GM_ADDR src0_gm, GM_ADDR src1_gm, GM_ADDR dst_gm)
{
    KernelDataCopyAtomicMin<half> op;
    op.Init(src0_gm, src1_gm, dst_gm, data_size);
    op.Process();
}
```

```
每个核的输入数据为: 
Src0: [1,1,1,1,1,...,1] // 256个1
Src1: [2,2,2,2,2,...,2] // 256个2
最终输出数据: [1,1,1,1,1,...,1] // 256个1
```

