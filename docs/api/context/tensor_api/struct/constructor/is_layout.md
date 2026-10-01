# is\_layout

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

判断输入的数据结构是否为Layout数据结构，可通过检查其成员常量value的值来判断。当value为true时，表示输入的数据结构是Layout类型；反之则为非Layout类型。

## 函数原型

```
template <typename T> struct is_layout
```

## 参数说明

**表 1**  模板参数说明


<table><thead align="left"><tr id="zh-cn_topic_0000002078486173_zh-cn_topic_0000001576727153_zh-cn_topic_0000001389787297_row6223476444"><th class="cellrowborder" valign="top" width="20.34%" id="mcps1.2.3.1.1"><p id="p1085176175119">参数名</p>
</th>
<th class="cellrowborder" valign="top" width="79.66%" id="mcps1.2.3.1.2"><p id="p148519610515">描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1392614743211"><td class="cellrowborder" valign="top" width="20.34%" headers="mcps1.2.3.1.1 "><p id="p17843141016336">T</p>
</td>
<td class="cellrowborder" valign="top" width="79.66%" headers="mcps1.2.3.1.2 "><p id="p1192718714328">根据输入的数据类型，判断是否为Layout数据结构。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明

无

## 约束说明

无

## 调用示例

```
// 初始化Layout数据结构并判断其类型
AscendC::Shape<int,int,int> shape = AscendC::MakeShape(10, 20, 30);
AscendC::Stride<int,int,int> stride = AscendC::MakeStride(1, 100, 200);

auto layoutMake = AscendC::MakeLayout(shape, stride);
AscendC::Layout<AscendC::Shape<int, int, int>, AscendC::Stride<int, int, int>> layoutInit(shape, stride);

bool value = AscendC::is_layout<decltype(shape)>::value; //value = false
value = AscendC::is_layout<decltype(stride)>::value; //value = false

value = AscendC::is_layout<decltype(layoutMake)>::value;//value = true
value = AscendC::is_layout<decltype(layoutInit)>::value;//value = true
```

