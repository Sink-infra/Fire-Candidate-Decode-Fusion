# GetLayout

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

获取Layout实例化对象。

## 函数原型

```
__aicore__ inline decltype(auto) GetLayout() const
```

## 参数说明

无

## 返回值说明

Layout实例化对象。

## 约束说明

无

## 调用示例

```
AscendC::Shape<size_t,size_t,size_t> shape = AscendC::MakeShape(10, 20, 30);
AscendC::Stride<size_t,size_t,size_t> stride = AscendC::MakeStride(1, 100, 200);

auto layoutMake = AscendC::MakeLayout(shape, stride);
auto tensorTraitMake = AscendC::MakeTensorTrait<size_t, AscendC::TPosition::VECIN>(layoutMake);

AscendC::GlobalTensor<decltype(tensorTraitMake)> gTensor;
gTensor.SetTensorTrait(tensorTraitMake);

auto gLayout = gTensor.GetLayout(); // Layout<Shape<10,20,30>, Stride<1,100,200>>

size_t shapeValue = AscendC::Std::get<0>(gTensor.GetShape()); // shapeValue = 10
shapeValue = AscendC::Std::get<1>(gTensor.GetShape()); // shapeValue = 20
shapeValue = AscendC::Std::get<2>(gTensor.GetShape()); // shapeValue = 30

size_t strideValue = AscendC::Std::get<0>(gTensor.GetStride()); // strideValue = 1
strideValue = AscendC::Std::get<1>(gTensor.GetStride()); // strideValue = 100
strideValue = AscendC::Std::get<2>(gTensor.GetStride()); // strideValue = 200
```

