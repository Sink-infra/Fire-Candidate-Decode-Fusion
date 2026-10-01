# Crd2Idx

## 产品支持情况


<table><thead align="left"><tr id="row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p1883113061818"><span id="ph20833205312295">产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p783113012187">是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p48327011813"><span id="ph583230201815"><term id="zh-cn_topic_0000001312391781_term1253731311225">Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115">Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7948163910184">√</p>
</td>
</tr>
<tr id="row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p14832120181815"><span id="ph1483216010188"><term id="zh-cn_topic_0000001312391781_term11962195213215">Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811">Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p19948143911820">√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明

Crd2Idx函数用于将多维坐标（Coordinate）通过布局（Layout）转换为内存位置索引（Index），这里的Layout包含了Shape和Stride信息。

对于一个布局Layout，其Shape为\(d<sub>0</sub>, d<sub>1</sub>, ..., d<sub>n</sub>\)，Stride为\(s<sub>0</sub>, s<sub>1</sub>, ..., s<sub>n</sub>\)，Coordinate为\(c<sub>0</sub>, c<sub>1</sub>, ..., c<sub>n</sub>\)到线性索引Index的转换公式为：

![](../../figures/zh-cn_formulaimage_0000002372135864.png)

例如，对于Shape \(3, 4, 5\)，Stride \(20, 5, 1\)和Coordinate \(1, 2, 3\)：

```
维度0: c₀ * s₀ = 1 * 20 = 20 
维度1: c₁ * s₁ = 2 * 5  = 10 
维度2: c₂ * s₂ = 3 * 1  = 3
Index = 20 + 10 + 3 = 33
```

当Coordinate维度和Stride维度不相同时，可以采用去线性化（delinearize）的方法，使得Coordinate维度和Stride维度相同，再使用上述公式计算得到最终结果。

去线性化的方法介绍如下：对于一个n维数组，形状为\(d<sub>0</sub>, d<sub>1</sub>, ..., d<sub>n</sub>\)，线性坐标c对应的多维坐标\(c<sub>0</sub>, c<sub>1</sub>, ..., c<sub>n</sub>\)，可以通过以下公式进行转换：

![](../../figures/zh-cn_formulaimage_0000002405659569.png)

例如：对于Shape \(\(2, 4\), \(3, 5\)\)，Stride\(\(3, 6\), \(1, 24\)\)，Layout \(\(2, 4\), \(3, 5\)\) : \(\(3, 6\), \(1, 24\)\)，Coordinate（11, 12），按照列优先原则，Crd2Idx的结果为：

```
crd2idx = delinearize(11, 12) * stride 
= ((11 % 2, 11 / 2), (12 % 3, 12 / 3)) *  ((3, 6), (1, 24))
= ((1, 5), (0, 4)) *  ((3, 6), (1, 24))
= 1 * 3 + 5 * 6 + 0 * 1 + 4 * 24 
= 129
```

总结上述过程，计算公式如下：

![](../../figures/zh-cn_formulaimage_0000002404172833.png)

![](../../figures/zh-cn_formulaimage_0000002426040106.png)

![](../../figures/zh-cn_formulaimage_0000002370455372.png)

![](../../figures/zh-cn_formulaimage_0000002370616000.png)

其中\(d<sub>0</sub>, d<sub>1</sub>, ..., d<sub>n</sub>\)为Shape，\(s<sub>0</sub>, s<sub>1</sub>, ..., s<sub>n</sub>\)为Stride，delinearize公式展开如下：

![](../../figures/zh-cn_formulaimage_0000002370590770.png)

## 函数原型

```
// Layout输入，Coordinate转换为Index
template <typename CoordType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto Crd2Idx(const CoordType& coord, const Layout<ShapeType, StrideType>& layout)

// Shape和Stride输入，Coordinate转换为Index
template <typename CoordType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto Crd2Idx(const CoordType& coord, const ShapeType& shape, const StrideType& stride)
```

## 参数说明


<table><thead align="left"><tr id="zh-cn_topic_0000002078486173_zh-cn_topic_0000001576727153_zh-cn_topic_0000001389787297_row6223476444"><th class="cellrowborder" valign="top" width="17.22%" id="mcps1.1.4.1.1"><p id="p1085176175119">参数名</p>
</th>
<th class="cellrowborder" valign="top" width="15.340000000000002%" id="mcps1.1.4.1.2"><p id="p1851763519">输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="67.44%" id="mcps1.1.4.1.3"><p id="p148519610515">描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row2050791017251"><td class="cellrowborder" valign="top" width="17.22%" headers="mcps1.1.4.1.1 "><p id="p1650781022510">coord</p>
</td>
<td class="cellrowborder" valign="top" width="15.340000000000002%" headers="mcps1.1.4.1.2 "><p id="p1750771018252">输入</p>
</td>
<td class="cellrowborder" valign="top" width="67.44%" headers="mcps1.1.4.1.3 "><p id="p13692185110277"><a href="容器函数.md">Std::tuple结构类型，用于表示张量在不同维度上的坐标值。</p>
<p id="p1329915004219">输入的数据类型支持size_t和Std::<a href="integral_constant.md">Int。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002078486173_zh-cn_topic_0000001576727153_zh-cn_topic_0000001389787297_row152234713443"><td class="cellrowborder" valign="top" width="17.22%" headers="mcps1.1.4.1.1 "><p id="p18419104127">layout</p>
</td>
<td class="cellrowborder" valign="top" width="15.340000000000002%" headers="mcps1.1.4.1.2 "><p id="p1641874129">输入</p>
</td>
<td class="cellrowborder" valign="top" width="67.44%" headers="mcps1.1.4.1.3 "><p id="p14181041623">输入的<a href="Layout.md">Layout对象。</p>
<p id="p31813716554">输入的数据类型支持size_t和Std::<a href="integral_constant.md">Int。</p>
</td>
</tr>
<tr id="row462694915242"><td class="cellrowborder" valign="top" width="17.22%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002078486173_zh-cn_topic_0000001576727153_zh-cn_topic_0000001389787297_p8563195616313">shape</p>
</td>
<td class="cellrowborder" valign="top" width="15.340000000000002%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002078486173_zh-cn_topic_0000001576727153_zh-cn_topic_0000001389787297_p15663137127">输入</p>
</td>
<td class="cellrowborder" valign="top" width="67.44%" headers="mcps1.1.4.1.3 "><p id="p823866165711"><span id="ph715020184014"><a href="容器函数.md">Std::tuple结构类型，用于定义数据的逻辑形状，例如二维矩阵的行数和列数或多维张量的各维度大小。</span></p>
<p id="p37122225552">输入的数据类型支持size_t和Std::<a href="integral_constant.md">Int。</p>
</td>
</tr>
<tr id="row34935466244"><td class="cellrowborder" valign="top" width="17.22%" headers="mcps1.1.4.1.1 "><p id="p139261676324">stride</p>
</td>
<td class="cellrowborder" valign="top" width="15.340000000000002%" headers="mcps1.1.4.1.2 "><p id="p19272713213">输入</p>
</td>
<td class="cellrowborder" valign="top" width="67.44%" headers="mcps1.1.4.1.3 "><p id="p64517398452"><span id="ph1582113012"><a href="容器函数.md">Std::tuple结构类型，用于定义各维度在内存中的步长，即同维度相邻元素在内存中的间隔，间隔的单位为元素，与Shape的维度信息一一对应。</span></p>
<p id="p4409152415551">输入的数据类型支持size_t和Std::<a href="integral_constant.md">Int。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明

返回根据Coordinate信息转换之后的索引值。

## 约束说明

输入参数需满足对应的数据类型要求。

## 调用示例

```
// Layout形式入参计算索引值
constexpr int M = 11;
constexpr int N = 12;
constexpr int blockM = 13;
constexpr int blockN = 14;

auto coord = AscendC::MakeCoord(AscendC::Std::Int<20>{}, AscendC::Std::Int<30>{});
auto shape = AscendC::MakeShape(AscendC::MakeShape(AscendC::Std::Int<blockM>{}, AscendC::Std::Int<M/blockM>{}), AscendC::MakeShape(AscendC::Std::Int<blockN>{}, AscendC::Std::Int<N/blockN>{}));
auto stride = AscendC::MakeStride(AscendC::MakeStride(AscendC::Std::Int<blockN>{}, AscendC::Std::Int<blockM*blockN>{}),AscendC::MakeStride(AscendC::Std::Int<1>{}, AscendC::Std::Int<M*blockN>{}));

auto layout = AscendC::MakeLayout(shape, stride);
auto index = layout(coord); // decltype(index)::value = 590
index = AscendC::Crd2Idx(coord, layout);  // decltype(index)::value = 590

// Shape和Stride形式入参计算索引值
auto blockCoordM    = AscendC::Std::Int<11>{};
auto blockCoordN    = AscendC::Std::Int<12>{};
auto baseShapeM     = AscendC::Std::Int<13>{};
auto baseShapeN     = AscendC::Std::Int<14>{};
auto basestrideM    = AscendC::Std::Int<15>{};
auto basestrideN    = AscendC::Std::Int<16>{};
auto coord = AscendC::MakeCoord(AscendC::Std::Int<0>{}, blockCoordN);
auto shape = AscendC::MakeShape(AscendC::MakeShape(baseShapeM, baseShapeM), AscendC::MakeShape(baseShapeN, baseShapeN));
auto stride = AscendC::MakeStride(AscendC::MakeStride(basestrideM, basestrideM),AscendC::MakeStride(basestrideN, basestrideN));

auto index = AscendC::Crd2Idx(coord, shape, stride); // decltype(index)::value = 192
```

