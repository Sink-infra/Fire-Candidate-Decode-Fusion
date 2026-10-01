# RpSort16<a name="ZH-CN_TOPIC_0000001443137898"></a>

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
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p37256491200"><a name="p37256491200"></a><a name="p37256491200"></a>x</p>
</td>
</tr>
<tr id="row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p48327011813"><a name="p48327011813"></a><a name="p48327011813"></a><span id="ph583230201815"><a name="ph583230201815"></a><a name="ph583230201815"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7948163910184"><a name="p7948163910184"></a><a name="p7948163910184"></a>x</p>
</td>
</tr>
<tr id="row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p14832120181815"><a name="p14832120181815"></a><a name="p14832120181815"></a><span id="ph1483216010188"><a name="ph1483216010188"></a><a name="ph1483216010188"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p19948143911820"><a name="p19948143911820"></a><a name="p19948143911820"></a>x</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

根据Region Proposals中的score域对其进行排序（score大的排前面），每次排16个Region Proposals。

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T>
__aicore__ inline void RpSort16(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001429830437_row118356578583"><th class="cellrowborder" valign="top" width="13.58%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001429830437_p48354572582"><a name="zh-cn_topic_0000001429830437_p48354572582"></a><a name="zh-cn_topic_0000001429830437_p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="86.42%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001429830437_p583535795817"><a name="zh-cn_topic_0000001429830437_p583535795817"></a><a name="zh-cn_topic_0000001429830437_p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001429830437_row1835857145817"><td class="cellrowborder" valign="top" width="13.58%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001429830437_p5835457165816"><a name="zh-cn_topic_0000001429830437_p5835457165816"></a><a name="zh-cn_topic_0000001429830437_p5835457165816"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="86.42%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001429830437_p168351657155818"><a name="zh-cn_topic_0000001429830437_p168351657155818"></a><a name="zh-cn_topic_0000001429830437_p168351657155818"></a>操作数数据类型。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="table16397194910519"></a>
<table><thead align="left"><tr id="row14397114985110"><th class="cellrowborder" valign="top" width="13.661366136613662%" id="mcps1.2.4.1.1"><p id="p10397449125111"><a name="p10397449125111"></a><a name="p10397449125111"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="12.591259125912593%" id="mcps1.2.4.1.2"><p id="p83971498517"><a name="p83971498517"></a><a name="p83971498517"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="73.74737473747375%" id="mcps1.2.4.1.3"><p id="p339754919510"><a name="p339754919510"></a><a name="p339754919510"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row19397104975114"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p15397104905113"><a name="p15397104905113"></a><a name="p15397104905113"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="p639784985113"><a name="p639784985113"></a><a name="p639784985113"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="p23620781116"><a name="p23620781116"></a><a name="p23620781116"></a>目的操作数，存储经过排序后的Region Proposals。</p>
<p id="p16703131355116"><a name="p16703131355116"></a><a name="p16703131355116"></a><span id="zh-cn_topic_0000001530181537_ph173308471594"><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><a name="zh-cn_topic_0000001530181537_ph173308471594"></a><span id="zh-cn_topic_0000001530181537_ph9902231466"><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><a name="zh-cn_topic_0000001530181537_ph9902231466"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816"><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p5170152413011"><a name="p5170152413011"></a><a name="p5170152413011"></a><span id="ph1479701815419"><a name="ph1479701815419"></a><a name="ph1479701815419"></a>LocalTensor的起始地址需要32字节对齐。</span></p>
</td>
</tr>
<tr id="row9397134918514"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p8397849185111"><a name="p8397849185111"></a><a name="p8397849185111"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="p153971249195113"><a name="p153971249195113"></a><a name="p153971249195113"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="p2042185715113"><a name="p2042185715113"></a><a name="p2042185715113"></a>源操作数，存储未经过排序的Region Proposals。</p>
<p id="p557485981119"><a name="p557485981119"></a><a name="p557485981119"></a><span id="zh-cn_topic_0000001530181537_ph173308471594_1"><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><a name="zh-cn_topic_0000001530181537_ph173308471594_1"></a><span id="zh-cn_topic_0000001530181537_ph9902231466_1"><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><a name="zh-cn_topic_0000001530181537_ph9902231466_1"></a><span id="zh-cn_topic_0000001530181537_ph1782115034816_1"><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a><a name="zh-cn_topic_0000001530181537_ph1782115034816_1"></a>类型为<a href="LocalTensor.md">LocalTensor</a>，支持的TPosition为VECIN/VECCALC/VECOUT。</span></span></span></p>
<p id="p131511267302"><a name="p131511267302"></a><a name="p131511267302"></a><span id="ph3760152613303"><a name="ph3760152613303"></a><a name="ph3760152613303"></a>LocalTensor的起始地址需要32字节对齐。</span></p>
</td>
</tr>
<tr id="row139734965111"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="p1839754914514"><a name="p1839754914514"></a><a name="p1839754914514"></a>repeatTime</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="p103974492515"><a name="p103974492515"></a><a name="p103974492515"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="p539774915112"><a name="p539774915112"></a><a name="p539774915112"></a>重复迭代次数，int32_t类型，每次排16个Region Proposals。取值范围：repeatTime∈[0,255]。</p>
</td>
</tr>
</tbody>
</table>

## 约束说明<a name="section633mcpsimp"></a>

-   用户需保证src和dst中存储的Region Proposal数目大于实际所需数据，否则会存在tensor越界错误。
-   当存在proposal\[i\]与proposal\[j\]的score值相同时，如果i\>j，则proposal\[j\]将首先被选出来，排在前面。
-   操作数地址对齐要求请参见[通用地址对齐约束](通用说明和约束.md#section796754519912)。

## 调用示例<a name="section642mcpsimp"></a>

-   接口使用样例

    ```
    // repeatTime = 2, 对2个Region Proposal进行排序
    AscendC::RpSort16(dstLocal, dstLocal, 2);
    ```

-   完整样例

    ```
    #include "kernel_operator.h"
    
    class KernelVecProposal {
    public:
        __aicore__ inline KernelVecProposal() {}
        __aicore__ inline void Init(__gm__ uint8_t* src, __gm__ uint8_t* dstGm)
        {
            srcGlobal.SetGlobalBuffer((__gm__ half*)src);
            dstGlobal.SetGlobalBuffer((__gm__ half*)dstGm);
    
            pipe.InitBuffer(inQueueSrc, 1, srcDataSize * sizeof(half));
            pipe.InitBuffer(outQueueDst, 1, dstDataSize * sizeof(half));
        }
        __aicore__ inline void Process()
        {
            CopyIn();
            PreProcess();
            Compute();
            CopyOut();
        }
    
    private:
        __aicore__ inline void CopyIn()
        {
            AscendC::LocalTensor<half> srcLocal = inQueueSrc.AllocTensor<half>();
            AscendC::DataCopy(srcLocal, srcGlobal, srcDataSize);
            inQueueSrc.EnQue(srcLocal);
        }
        __aicore__ inline void PreProcess()
        {
            AscendC::LocalTensor<half> srcLocal = inQueueSrc.DeQue<half>();
            AscendC::LocalTensor<half> dstLocal = outQueueDst.AllocTensor<half>();
            AscendC::ProposalConcat(dstLocal, srcLocal, repeat, mode); // sort排序是基于score的，此处先创建一个有score数据的proposal，需要注意的是，非score处的数据可能是随机值
            outQueueDst.EnQue<half>(dstLocal);
            inQueueSrc.FreeTensor(srcLocal);
        }
        __aicore__ inline void Compute()
        {
            AscendC::LocalTensor<half> dstLocal = outQueueDst.DeQue<half>();
            AscendC::RpSort16(dstLocal, dstLocal, repeat);
            outQueueDst.EnQue<half>(dstLocal);
        }
        __aicore__ inline void CopyOut()
        {
            AscendC::LocalTensor<half> dstLocal = outQueueDst.DeQue<half>();
            AscendC::DataCopy(dstGlobal, dstLocal, dstDataSize);
            outQueueDst.FreeTensor(dstLocal);
        }
    
    private:
        AscendC::TPipe pipe;
        AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueSrc;
        AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueDst;
        AscendC::GlobalTensor<half> srcGlobal, dstGlobal;
        int srcDataSize = 32;
        int dstDataSize = 256;
        int repeat = srcDataSize / 16;
        int mode = 4;
    };
    
    extern "C" __global__ __aicore__ void vec_proposal_kernel(__gm__ uint8_t* src, __gm__ uint8_t* dstGm)
    {
        KernelVecProposal op;
        op.Init(src, dstGm);
        op.Process();
    }
    ```

    ```
    示例结果
    输入数据(src_gm):
    [ -1.624 -42.3   -54.12   91.25  -99.4    36.72   67.44  -66.3   -52.53
       3.377 -62.47  -15.85  -31.47    3.143  58.47  -83.75 21.58   63.47    
       7.234  35.16  -39.72   37.8    73.06  -98.7    44.1 -77.2    67.2    
       19.62  -87.9   -14.875  15.86  -77.75]
    输出数据(dst_gm):
    [  0.      0.      0.      0.     91.25    0.      0.      0.      0.
       0.      0.      0.     67.44    0.      0.      0.      0.      0.
       0.      0.     58.47    0.      0.      0.      0.      0.      0.
       0.     36.72    0.      0.      0.      0.      0.      0.      0.
       3.377   0.      0.      0.      0.      0.      0.      0.      3.143
       0.      0.      0.      0.      0.      0.      0.     -1.624   0.
       0.      0.      0.      0.      0.      0.    -15.85    0.      0.
       0.      0.      0.      0.      0.    -31.47    0.      0.      0.
       0.      0.      0.      0.    -42.3     0.      0.      0.      0.
       0.      0.      0.    -52.53    0.      0.      0.      0.      0.
       0.      0.    -54.12    0.      0.      0.      0.      0.      0.
       0.    -62.47    0.      0.      0.      0.      0.      0.      0.
     -66.3     0.      0.      0.      0.      0.      0.      0.    -83.75
       0.      0.      0.      0.      0.      0.      0.    -99.4     0.
       0.      0.      0.      0.      0.      0.     73.06    0.      0.      
       0.      0.      0.      0.      0.     67.2     0.      0.      0.      
       0.      0.      0.      0.     63.47    0.      0.      0.      0.      
       0.      0.      0.     44.1     0.      0.      0.      0.      0.      
       0.      0.     37.8     0.      0.      0.      0.      0.      0.      
       0.     35.16    0.      0.      0.      0.      0.      0.      0.     
      21.58    0.      0.      0.      0.      0.      0.      0.     19.62    
       0.      0.      0.      0.      0.      0.      0.     15.86    0.      
       0.      0.      0.      0.      0.      0.      7.234   0.      0.      
       0.      0.      0.      0.      0.    -14.875   0.      0.      0.      
       0.      0.      0.      0.    -39.72    0.      0.      0.      0.      
       0.      0.      0.    -77.2     0.      0.      0.      0.      0.      
       0.      0.    -77.75    0.      0.      0.      0.      0.      0.      
       0.    -87.9     0.      0.      0.      0.      0.      0.      0.    
     -98.7     0.      0.      0.   ]
    ```

