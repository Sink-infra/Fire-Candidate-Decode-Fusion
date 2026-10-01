# GetStride

## 产品支持情况

|产品|是否支持|
|--|:-:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|√|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|√|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Atlas 200/300/500 推理产品|x|



## 功能说明

获取描述内存访问步长的Stride对象。

## 函数原型

```
__aicore__ inline decltype(auto) GetStride() const
```

## 参数说明

无

## 返回值说明

描述内存访问步长的Stride对象。

## 约束说明

无

## 调用示例

见[5.13.1.4-调用示例](GetLayout.md)。

