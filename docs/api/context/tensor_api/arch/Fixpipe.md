# Fixpipe

## 产品支持情况<a name="section73648168211"></a>

| 产品                                        | 是否支持 |
| ------------------------------------------- | :------: |
| Ascend 950PR/Ascend 950DT                   |    √     |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 |    x     |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 |    x     |
| Atlas 200I/500 A2 推理产品                  |    x     |
| Atlas 推理系列产品AI Core                   |    x     |
| Atlas 推理系列产品Vector Core               |    x     |
| Atlas 训练系列产品                          |    x     |
| Atlas 200/300/500 推理产品                  |    x     |


## 功能说明<a name="section967653716714"></a>

Fixpipe能够在矩阵计算完成后对计算结果进行处理，例如量化操作，并把数据从Local Memory搬运到Global Memory中。

## 函数原型<a name="section1790918208125"></a>

```
template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeTemplate<T, U>, void>::type Fixpipe(const T& dst, const U& src)

template <const FixpipeTrait& trait = DEFAULT_FIXPIPE_TRAIT, typename T, typename U, typename V>
__aicore__ inline typename Std::enable_if<VerifyingFixpipeQuantTemplate<T, U, V>, void>::type 
Fixpipe(const T& dst, const U& src, const V& quant);
```

## 参数说明<a name="section966471201217"></a>

**表 1**  模板参数说明

| 参数名            | 描述                                                         |
| ----------------- | ------------------------------------------------------------ |
| T                 | 目的操作数的数据类型，通过TensorTrait构造的LocalTensor类型。 |
| U                 | 源操作数的数据类型，通过TensorTrait构造的GlobalTensor类型。  |
| V                 | 量化参数的数据类型，可以是scalar或者是通过TensorTrait构造的GlobalTensor类型。支持的数据类型为uint64_t |
| FixpipeTypeTraits | Fixpipe相关配置参数，类型为FIxpipeTrait：struct FixpipeTrait {    QuantMode_t quantPre = QuantMode_t::NoQuant;    bool reluEn = false;    bool isChannleSplit = false;    uint8_t unitFlag = false;    uint8_t dualDstCtl = false;    bool subBlockId = false;};constexpr FixpipeTrait DEFAULT_FIXPIPE_TRAIT; |


**表 2**  量化参数说明

| 参数名   | 描述                                                         |
| -------- | :----------------------------------------------------------- |
| quantPre | QuantMode_t是一个枚举类型，用于控制量化模式，默认值为QuantMode_t::NoQuant，即不使能量化功能。QuantMode_t取值如下：<br>NoQuant，不使能量化功能F322F16，float量化成half，量化结果支持INF_NAN模式<br/>F322BF16，float量化成bfloat16_t，量化结果支持INF_NAN模式<br/>DEQF16，int32_t量化成half, scalar量化，量化结果不支持INF_NAN模式<br/>VDEQF16，int32_t量化成half，tensor量化，量化结果不支持INF_NAN模式QF322B8_PRE，float量化成uint8_t/int8_t，scalar量化VQ<br/>F322B8_PRE，float量化成uint8_t/int8_t，tensor量化REQ8，int32_t量化成uint8_t/int8_t，scalar量化<br/>VREQ8，int32_t量化成uint8_t/int8_t，tensor量化<br/>QF322FP8_PRE，float量化成fp8_e4m3fn_t，scalar量化<br/>VQF322FP8_PRE，float量化成fp8_e4m3fn_t，tensor量化<br/>QF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，scalar量化<br/>VQF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，tensor量化<br/>QF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，scalar量化<br/>VQF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，tensor量化<br/>QS322BF16_PRE，int32_t量化成bfloat16_t，scalar量化<br/>VQS322BF16_PRE，int32_t量化成bfloat16_t，tensor量化<br/>QF322F16_PRE，float量化成half，scalar量化<br/>VQF322F16_PRE，float量化成half，tensor量化<br/>QF322BF16_PRE，float量化成bfloat16_t，scalar量化<br/>VQF322BF16_PRE，float量化成bfloat16_t，tensor量化<br/>QF322F32_PRE，float量化成float，scalar量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用AscendDeQuant高阶APIVQF322F32_PRE，float量化成float，tensor量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用AscendDeQuant高阶API |


**表 3**  参数说明

| 参数名 | 输入/输出 | 含义         |
| ------ | --------- | ------------ |
| dst    | 输出      | 目的操作数。 |
| src    | 输入      | 源操作数。   |
| quant  | 输入      | 量化参数     |


## 返回值说明<a name="section44801012174220"></a>

VerifyingFixpipeTemplate表达式为模板参数校验表达式，执行效果如下：

-   校验表达式成立，表明输入数据类型符合使用约束，Fixpipe函数表达式返回值为void。
-   校验表达式不成立，表明输入数据类型不符合使用约束时，Fixpipe函数声明失效，继而会导致编译错误。

## 约束说明<a name="section1333717617510"></a>

-   Tensor Layout相关约束：
    -   TensorTrait中的Layout表述，统一采用[层次化表述法](../struct/defenition/Layout和层次化表述法.md)。
    -   Shape、Stride只支持四个维度的数据配置，针对不同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[数据通路说明]。
    -   Shape、Stride等数值数据，仅支持size\_t类型和Std::Int类型。
    -   Fixpipe量化接口中，quant参数的Layout中每一行的长度必须能够被主块（512）和尾块（n % 512）整除

-   quant参数数据通路和dataCopy L1 -\> FB保持一致。

## 数据通路说明<a name="section840010692511"></a>

**表 4**  L0C Buffer -\> GM通路使用说明

| 项目                                         | 内容                                                         |
| -------------------------------------------- | ------------------------------------------------------------ |
| 模板参数T                                    | 数据类型为GlobalTensor<TensorTrait>。                        |
| 模板参数U                                    | 数据类型为LocalTensor<TensorTrait>。                         |
| 目的操作数TPosition取值范围                  | TPosition::GM                                                |
| 源操作数TPosition取值范围                    | TPosition::CO1                                               |
| 目的操作数的数据类型                         | float、int32_t                                               |
| 源操作数的数据类型                           | float、int32_t                                               |
| 源操作数和目的操作数基础数据类型是否要求一致 | 是                                                           |
| 源操作数数据对齐要求                         | 起始地址和数据长度要求64字节对齐。                           |
| 目的操作数数据对齐要求                       | 无对齐要求。                                                 |
| 数据排布要求                                 | NZ Layout -> NZ Layout : 源操作数和目的操作数仅支持连续排布和非连续排布。NZ Layout -> RowMajor Layout ：源操作数和目的操作数仅支持连续排布和非连续排布。<br>NZ Layout -> ColumnMajor Layout：源操作数和目的操作数仅支持连续排布和非连续排布。<br/>NZ Layout内层矩阵的Shape大小为16 * 16。 |


## 调用示例<a name="section163951002119"></a>

```
...
// CO1 -> GM
using SrcT = float;
using DstT = float;
size_t mLength = 128;
size_t kLength = 64;

auto dstLayout = AscendC::MakeRowMajorLayout<DstT>(mLength, kLength);
auto srcLayout = AscendC::MakeNZLayout<SrcT>(mLength, kLength);

auto srcTrait = AscendC::MakeTensorTrait<SrcT, TPosition::CO1>(srcLayout);
auto dstTrait = AscendC::MakeTensorTrait<DstT, TPosition::GM>(dstLayout);

using srcTraitType = decltype(srcTrait);
using dstTraitType = decltype(dstTrait);

LocalTensor<srcTraitType> L0CTrait;
L0CTrait.SetTensorTrait(srcTrait);

GlobalTensor<dstTraitType> GlobalTraits;
GlobalTraits.SetTensorTrait(dstTrait);
//不做量化
AscendC::Fixpipe(GlobalTraits, L0CTrait);
//从float量化为half
constexpr static AscendC::FixpipeTrait trait = {AscendC::QuantMode_t::F322F16};
uint64_t quant = 0;
AscendC::Fixpipe<trait>(GlobalTraits, L0CTrait, quant);
...
```

