# DataCopy>

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

DataCopy提供数据搬运功能，该接口支持Local Memory与Global Memory之间的数据搬运和Local Memory内部的数据搬运。

## 函数原型>

```
template <const DataCopyTrait& trait = DEFAULT_DATA_COPY_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingDataCopyTemplate<T, U>, void>::type DataCopy(const T& dst, const U& src)
```

## 参数说明>

**表 1**  模板参数说明

|参数名|描述|
|--|--|
|T|目的操作数的数据类型，通过TensorTrait构造的LocalTensor类型。|
|U|源操作数的数据类型，通过TensorTrait构造的GlobalTensor或LocalTensor类型。|
|DataCopyTrait|预留参数，保持默认值即可。|


**表 2**  参数说明

|参数名|输入/输出|含义|
|--|--|--|
|dst|输出|目的操作数。|
|src|输入|源操作数。|


## 返回值说明>

VerifyingDataCopyTemplate表达式为模板参数T和U的校验表达式，执行效果如下：

-   校验表达式成立，表明输入数据类型符合使用约束，DataCopy函数表达式返回值为void。
-   校验表达式不成立，表明输入数据类型不符合使用约束时，DataCopy函数声明失效，继而会导致编译错误。

## 约束说明>

-   Tensor Layout相关约束：
    -   TensorTrait中的Layout表述，统一采用[层次化表述法](../struct/defenition/Layout和层次化表述法.md)。
    -   Shape、Stride只支持四个维度的数据配置，针对不同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[数据通路说明]。
    -   Shape、Stride等数值数据，仅支持size\_t类型和Std::Int类型。

-   L1 Buffer -\> Fixpipe Buffer通路每次最多搬运4KB数据。

## 数据通路说明>

下文的数据通路均通过逻辑位置TPosition来表达，并注明了对应的物理通路。TPosition与物理内存的映射关系见[表1]。

**表 3**  GM-\>L1 Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor<TensorTrait>。|
|模板参数U|数据类型为GlobalTensor<TensorTrait>。|
|目的操作数TPosition取值范围|TPosition::A1、TPosition::B1、TPosition::C1|
|源操作数TPosition取值范围|TPosition::GM|
|目的操作数的数据类型|bfloat16_t、 half、 float、 int16_t、 int32_t、 int8_t、 uint16_t、 uint32_t、 uint8_t|
|源操作数的数据类型|bfloat16_t、 half、 float、 int16_t、 int32_t、 int8_t、 uint16_t、 uint32_t、 uint8_t|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|数据排布要求|NZ Layout -> NZ Layout : 源操作数和目的操作数均支持连续和非连续排布。ND Layout -> ND Layout : 源操作数和目的操作数均支持连续和非连续排布。RowMajor Layout -> NZ Layout ：源操作数和目的操作数仅支持连续排布。ColumnMajor Layout ->  NZ Layout：源操作数和目的操作数仅支持连续排布。NZ Layout内层矩阵的Shape大小是16 * (32 / sizeof(T))。非连续搬运时，外层矩阵列方向要保证连续，行方向可以跳跃。即：// StrideColumn1应满足：StrideColumn1 = StrideRow1 * ShapeRow1|


**表 4**  L1 Buffer -\> BiasTable Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor<TensorTrait>。|
|模板参数U|数据类型为LocalTensor<TensorTrait>。|
|目的操作数TPosition取值范围|TPosition::C2|
|源操作数TPosition取值范围|TPosition::C1|
|目的操作数的数据类型|half、bfloat16_t、int32_t、float|
|源操作数的数据类型|half、bfloat16_t、int32_t、float|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求64字节对齐。|
|数据排布要求|RowMajor Layout -> RowMajor Layout：源操作数和目的操作数均支持连续和非连续排布。非连续时仅支持以行为单位进行跳跃。|


**表 5**  L1 Buffer -\> Fixpipe Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor<TensorTrait>。|
|模板参数U|数据类型为LocalTensor<TensorTrait>。|
|目的操作数TPosition取值范围|TPosition::C2PIPE2GM|
|源操作数TPosition取值范围|TPosition::C1|
|目的操作数的数据类型|bool、int8_t、uint8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64|
|源操作数的数据类型|bool、int8_t、uint8_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求128字节对齐。|
|数据排布要求|RowMajor Layout -> RowMajor Layout：源操作数和目的操作数均支持连续和非连续排布。非连续时仅支持以行为单位进行跳跃。|


## 调用示例>

```
...
// GM -> A1
using SrcT = half;
size_t mLength = 128;
size_t kLength = 64;

auto srcLayout = AscendC::MakeRowMajorLayout<SrcT>(mLength, kLength);
auto dstLayout = AscendC::MakeNZLayout<SrcT>(mLength, kLength);

auto srcTrait = AscendC::MakeTensorTrait<SrcT, TPosition::GM>(srcLayout);
auto dstTrait = AscendC::MakeTensorTrait<SrcT, TPosition::A1>(dstLayout);

using srcTraitType = decltype(srcTrait);
using dstTraitType = decltype(dstTrait);

GlobalTensor<srcTraitType> aGlobalTrait;
aGlobalTrait.SetTensorTrait(srcTrait);

LocalTensor<dstTraitType> leftMatrixTrait;
leftMatrixTrait.SetTensorTrait(dstTrait);

AscendC::DataCopy(leftMatrixTrait, aGlobalTrait);
...
```

