# LoadData>

## 产品支持情况>

|产品|是否支持|
|--|:-:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|x|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|x|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Atlas 200/300/500 推理产品|x|


## 功能说明>

LoadData接口支持Local Memory内部的数据搬运，能够完成L1 Buffer -\> L0A Buffer或L1 Buffer -\> L0B Buffer（A1到A2或B1到B2）的数据搬运。

## 函数原型>

```
template <constLoadDataTrait& trait = DEFAULT_LOAD_DATA_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingLoadDataTemplate<T, U>, void>::type LoadData(const T& dst, const U& src);
```

## 参数说明>

**表 1**  模板参数说明

|参数名|描述|
|--|--|
|T|目的操作数的数据类型，通过TensorTrait构造的LocalTensor类型。|
|U|源操作数的数据类型，通过TensorTrait构造的LocalTensor类型。|
|LoadDataTrait|LoadData相关配置参数，成员transposed用于指示A1/B1中的数据是否已转置，false表示未转置，true表示已转置。DEFAULT_LOAD_DATA_TRAIT为默认取值false，即A1/B1中的数据未经转置。struct LoadDataTrait {    bool transposed = false;};constexpr LoadDataTrait DEFAULT_LOAD_DATA_TRAIT = {};|


**表 2**  参数说明

|参数名|输入/输出|含义|
|--|--|--|
|dst|输出|目的操作数。|
|src|输入|源操作数。|


## 返回值说明>

VerifyingLoadDataTemplate表达式为模板参数校验表达式，执行效果如下：

-   校验表达式成立，表明输入数据类型符合使用约束，LoadData函数表达式返回值为void。
-   校验表达式不成立，表明输入数据类型不符合使用约束时，LoadData函数声明失效，继而会导致编译错误。

## 约束说明>

-   Tensor Layout相关约束：
    -   TensorTrait中的Layout表述，统一采用[层次化表述法](Layout和层次化表述法.md)(../struct/defenition/Layout和层次化表述法.md)同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[数据通路说明]。
    -   dst的Shape、Stride只支持两个维度的数据配置。
    -   Shape、Stride等数值数据，仅支持size\_t类型和Std::[Int]类型。

## 数据通路说明>

**表 3**  L1 Buffer -\> L0A Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor<TensorTrait>。|
|模板参数U|数据类型为LocalTensor<TensorTrait>。|
|目的操作数TPosition取值范围|TPosition::A2|
|源操作数TPosition取值范围|TPosition::A1|
|目的操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t|
|源操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求512字节对齐。|
|数据排布要求|NZ Layout -> NZ Layout : 源操作数和目的操作数均仅支持连续排布。<br>NZ Layout内层矩阵的Shape大小是16 * (32 / sizeof(T))。|


**表 4**  L1 Buffer -\> L0B Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor<TensorTrait>。|
|模板参数U|数据类型为LocalTensor<TensorTrait>。|
|目的操作数TPosition取值范围|TPosition::B2|
|源操作数TPosition取值范围|TPosition::B1|
|目的操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t|
|源操作数的数据类型|half、bfloat16_t、uint32_t、int32_t、float、uint8_t、int8_t、fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求512字节对齐。|
|数据排布要求|NZ Layout -> ZN Layout : 源操作数和目的操作数均仅支持连续排布。<br>NZ Layout内层矩阵的Shape大小是16 * (32 / sizeof(T))。ZN Layout内存矩阵的Shape大小是 (32 / sizeof(T)) * 16。|


## 调用示例>

```
...
// A1 -> A2
using SrcT = float;
using DstT = float;
size_t mLength = 128;
size_t kLength = 64;

auto dstLayout = AscendC::MakeNZLayout<DstT>(mLength, kLength);
auto srcLayout = AscendC::MakeNZLayout<SrcT>(mLength, kLength);

auto srcTrait = AscendC::MakeTensorTrait<SrcT, TPosition::A1>(srcLayout);
auto dstTrait = AscendC::MakeTensorTrait<DstT, TPosition::A2>(dstLayout);

using srcTraitType = decltype(srcTrait);
using dstTraitType = decltype(dstTrait);

LocalTensor<srcTraitType> SrcTraitTensor;
SrcTraitTensor.SetTensorTrait(srcTrait);

LocalTensor<dstTraitType> DstTraitTensor;
DstTraitTensor.SetTensorTrait(dstTrait);

AscendC::LoadData<DEFAULT_LOADDATA_TYPE_TRAITS, LocalTensor<dstTraitType>, LocalTensor<srcTraitType>>(DstTraitTensor, SrcTraitTensor);
...
```

