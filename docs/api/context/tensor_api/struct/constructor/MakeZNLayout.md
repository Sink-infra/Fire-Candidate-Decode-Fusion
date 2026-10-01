# MakeZNLayout

## 产品支持情况

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



## 功能说明

根据输入的行数和列数，生成[层次化表述法](../defenition/Layout和层次化表述法.md)的ZN Layout结构，该结构为连续的ZN Layout结构，即数据紧密排列。

```cpp
Layout = ((Shape) : (Stride))
Shape = ((Std::Int<32 / sizeof(T)>{}, row/ (32 / sizeof(T))), (Std::Int<16>{}, column / 16))
Stride = ((Std::Int<1>{}, 32 / sizeof(T) * column), (Std::Int<32 / sizeof(T)>{}, Std::Int<32 / sizeof(T) * 16>{}))
```

## 函数原型

```cpp
// 生成的Layout为静态数据
template <typename T, size_t row, size_t column> ZNLayout

// 生成的Layout为动态数据
template <typename T>
__aicore__ inline decltype(auto) MakeZNLayout(size_t row, size_t column)
```

## 参数说明

|参数名|描述|
|--|--|
|T|基础数据类型，例如half或者float。|
|row|把数据看做连续的二维数据时，对应的行数。|
|column|把数据看做连续的二维数据时，对应的列数。|


## 返回值说明

无

## 约束说明

-   基础数据类型的位宽，需大于等于1。
-   输入的row和column，应保证计算公式结果大于零，整除有效无余数。

## 调用示例

```cpp
// 生成的Layout为动态数据
using SrcT = half;
size_t mLength = 128;
size_t kLength = 64;
auto srcLayout = AscendC::MakeZNLayout<SrcT>(mLength, kLength);

// 生成的Layout为静态数据
ZNLayout<SrcT, 128, 64> srcLayout;
```

