# Tensor API

Tensor API提供一套基于AscendC的C++模板抽象库，专为定义和操作层次化多维的数据布局而设计，用户可以专注于算法的逻辑表达而无需关心底层的内存布局问题，适用于昇腾AI处理器上的高性能计算场景，提供与业界一致的开发体验。

## 需要包含的头文件

> [!NOTE]说明 
> 包含tensor_api_impl.h文件来调用Tensor API相应接口。如无特殊说明，包含该头文件即可满足接口调用需求。
> 若API文档中有特殊说明，则应遵循API的具体说明。

```
#include "tensor_api/tensor_api_impl.h"
```

## API类型介绍

-  TensorTile基础结构：该类型API定义了Layout、Tensor基础结构，以及创建及相关操作。
    - [Layout](struct/defenition/Layout和层次化表述法.md)
    - Tensor
- TensorTile计算接口：该类型API包括搬运接口和矩阵计算接口，支持对数据结构的搬入搬出及矩阵计算操作。

## 关键字

| 名称           | 描述                                                         |
| -------------- | ------------------------------------------------------------ |
| \_\_gm\_\_     | 存储空间定义修饰符，表示被修饰的变量位于Global Memory地址空间。 |
| \_\_ubuf\_\_   | 存储空间定义修饰符，表示被修饰的变量位于Unified Buffer地址空间。 |
| \_\_aicore\_\_ | 执行空间限定符。表示该函数只能在Ai Core上执行。              |

## 流水类型



## API列表

| 目录                               |
| ---------------------------------- |
| [数据结构](tensor_api_list.md#数据结构) |
| [数据接口](tensor_api_list.md#数据接口) |
| [数据搬运](tensor_api_list.md#数据搬运) |
| [矩阵计算](tensor_api_list.md#矩阵计算) |
