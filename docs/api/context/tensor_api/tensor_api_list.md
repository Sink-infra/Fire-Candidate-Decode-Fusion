# Tensor API

Tensor API文档目录，整体使用时可以引入tensor_api_impl.h，Tensor API列表如下：

## 数据结构

| 结构名 | 说明 |
| ----------------------- | ----------------------- |
| [Shape] | 定义Shape |
| [Stride] | 定义Stride |
| [Coord] | 定义Coord |
| [Layout](struct/defenition/Layout简介.md) | 定义Layout |
| [Tensor] | 定义Tensor |
| [Pointer] | 定义迭代器 |

## 数据接口

基础结构类API，可以高效地管理内存，进行复杂的张量操作。此类API列表如下：

| API名称 | 说明 |
| ---------- | ----------- |
| [MakeShape] | 构造Shape |
| [MakeStride] | 构造Stride |
| [GetShape](struct/constructor/GetShape.md) | 构造Stride |
| [GetStride](struct/constructor/GetStride.md) | 构造Stride |
| [MakeCoord] | 构造Coord |
| [MakeTile] | 构造Tile |
| [MakeLayout](struct/constructor/MakeLayout.md) | 构造Layoute |
| [MakeNZLayout](struct/constructor/MakeNZLayout.md) | 构造NZ Layoute |
| [MakeZNLayout](struct/constructor/MakeZNLayout.md) | 构造Layoute |
| [is_Layout](struct/constructor/is_layout.md) | 判断Layoute |
| [Get] | 构造子Layout |
| [Select] | 构造子Layout |
| [GetShape] | 返回Shape |
| [GetStride](struct/constructor/GetStride.md)  | 返回Stride |
| [GetLayout](struct/constructor/GetLayout.md)  | 返回Layout对象 |
| [GetStride](struct/constructor/MakeZNLayout.md)  | 返回Stride |
| [Size] | 返回Shape的总大小 |
| [Rank] | 返回Layout中的秩 |
| [Coshape] | 返回实际Shape空间 |
| [Cosize] | 返回实际占用的内存 |
| [Crd2idx] | Coordinate转Index |
| [MakeTensor] | 构造Tensor |
| [ZippedDivided] | 通过Tile切分Tensor，返回Tensor |
| [InnerPartition] | 通过Tile切分Tensor，返回Tensor |
| [LocalTile] | 通过Tile切分Tensor，返回Tensor |
| [MakeGlobalPtr] | 构造GlobalPointer迭代器 |
| [RecastlPtr] | Ptr强制转换 |
| [MakeL1Ptr] | 构造L1Pointer迭代器 |
| [MakeL0APtr] | 构造L0APointer迭代器 |
| [MakeL0BPtr] | 构造L0BPointer迭代器 |
| [MakeL0CPtr] | 构造L0CPointer迭代器 |

## 数据搬运

数据搬运类API，单独使用时可以引入tensor_tile_arch.h，此类API列表如下：

|   API名称   |   说明   |
|----------|-----------|
| [DataCopy] | CUBE计算，支持GM2L1搬运处理 |
| [TileCopy] | CUBE计算，支持L12L0搬运 |
| [FixPipe] | FixPIPE搬出接口 |
| [LoadData]| L12L0加载接口 |


## 矩阵计算

标量操作类API，单独使用时可以引入tensor_tile_arch.h，此类API列表如下：

|   API名称   |   说明   |
|----------|-----------|
| [Mmad] | 完成矩阵乘加操作。 |


