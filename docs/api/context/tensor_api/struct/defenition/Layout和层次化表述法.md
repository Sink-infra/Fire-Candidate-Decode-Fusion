# Layout和层次化表述法

## Layout

我们使用Shape和Stride来表达Layout排布格式，比如下图中的行优先和列优先排布：

Shape用于表达Tensor形状，Stride则用于区分不同的排布：

-   行优先:Shape \(2, 4\)  Stride \(4, 1\)
-   列优先:Shape \(2, 4\)  Stride \(1, 2\)

图中每个方格中的数字表示该位置元素在内存中按顺序排列时的下标。对于相同的矩阵位置，排布方式不同时，其在内存中的顺序可能是不同的，例如，矩阵坐标 \(1, 0\) 在行优先和列优先的情况下，对应元素在内存中的顺序分别是4和1。

**图 1**  行优先排布 
![](../../figure/行优先排布.png "行优先排布")

**图 2**  列优先排布
![](../../figure/列优先排布.png "列优先排布")

## 层次化表述法

通常Shape或者Stride中的元素是一个单独的整数（如上文的示例），但是遇到一些复杂的内存排布情况可能无法表达，所以采用[层次化表述法](https://dl.acm.org/doi/abs/10.1145/3582016.3582018)，该方式中，元素也可以是一个元组。元组的第一个元素用于描述不同层次中矩阵的行数，元组的第二个元素用于描述不同层次中矩阵的列数。

例如在下图中，Layout的Shape和Stride分别为：

-   Shape\(\(2, 3\), \(2, 4\)\)
-   Stride\(\(1, 4\), \(2, 12\)\)

**图 3**  层次化表述法
![](../../figure/层次化表述法.png "层次化表述法")

图中展示了两层矩阵：内层矩阵为内部用灰色线包裹的矩阵，外层矩阵为将内层矩阵视为一个元素时黑色线包裹的矩阵。

Shape的第一个元素描述行方向的形状，\(2，3\)表示内层矩阵和外层矩阵的行数分别为2和3；Shape的第二个元素描述的是列方向上的形状，\(2，4\)表示内存矩阵和外层矩阵的列数分别为2和4。

Stride中的每个元素与Shape中的元素对应，表示该对应维度下，相邻元素首地址在内存地址上的间隔，图片中用箭头表示了每个维度相邻元素的首地址间隔。

## 常用的Layout

在基于Ascend C进行矩阵编程场景下，会用到以下几种常用的Layout，这些格式都采用如上文介绍的层次化表述法来表达，要求有内外层两层矩阵，具体表达方式如下：

```
Layout = ((Shape): (Stride))
Shape = ((ShapeRow0, ShapeRow1), (ShapeColumn0, ShapeColumn1))
Stride = ((StrideRow0, StrideRow1), (StrideColumn0, StrideColumn1))
```

-   NZ Layout

    NZ Layout格式的ShapeRow0、ShapeColumn0为固定值，下文示例中内层矩阵的Shape为\(32字节 / sizeof\(T\)\) \* 16；StrideRow0、StrideColumn0也为固定值，即内层矩阵按照行优先方式Z字形连续存储。外层矩阵按照N字型存储，所以StrideRow1应该满足如下的对齐要求。

    ```
    Layout = ((Shape) : (Stride))
    Shape = ((Std::Int<16>{}, ShapeRow1), (Std::Int<32 / sizeof(T)>{}, ShapeColumn1))
    Stride = ((Std::Int<32 / sizeof(T)>{}, StrideRow1), (Std::Int<1>{}, StrideColumn1))
    // StrideRow1应满足：
    （StrideRow1 - ShapeRow0 * ShapeColumn0）% ShapeColumn0 = 0
    ```

    下面是一个连续的NZ Layout的示例。

    ```
    Layout = ((Shape) : (Stride))
    Shape = ((Std::Int<16>{}, row / 16), (Std::Int<32/ sizeof(T)>{}, column / (32 / sizeof(T))))
    Stride = ((Std::Int<32 / sizeof(T)>{}, Std::Int<32 / sizeof(T) * 16>{}), (Std::Int<1>{}, 32 / sizeof(T) * row))
    ```

    ![](../../figure/分形变换.png)

    通过[MakeNZLayout](../constructor/MakeNZLayout.md)接口可以快速的构造连续的NZ Layout格式，自行设置StrideRow1和StrideColum1可以构造非连续的NZ Layout格式。

    -   ZN Layout

        ZN Layout格式的ShapeRow0、ShapeColumn0为固定值，下文示例中内层矩阵的Shape为\(32字节 / sizeof\(T\)\) \* 16；StrideRow0、StrideColumn0也为固定值，即内层矩阵按照行优先方式N字形连续存储。外层矩阵按照Z字型存储，所以StrideRow1应该满足如下的对齐要求。

        ```
        Layout = ((Shape) : (Stride))
        Shape = ((Std::Int<32 / sizeof(T)>{}, ShapeRow1), (Std::Int<16>{}, ShapeColumn1))
        Stride = ((Std::Int<1>{}, StrideRow1), (Std::Int<32 / sizeof(T)>{}, StrideColumn1))
        // StrideRow1应满足：
        （StrideColumn1 - ShapeRow0 * ShapeColumn0）% ShapeRow0 = 0
        ```

        下面是一个连续的ZN Layout的示例。

        ```
        Layout = ((Shape) : (Stride))
        Shape = ((Std::Int<32 / sizeof(T)>{}, row / 32 / sizeof(T)), (Std::Int<16>{}, column / 16))
        Stride = ((Std::Int<1>{}, 32 / sizeof(T) * column), (Std::Int<32 / sizeof(T)>{}, Std::Int<32 / sizeof(T) * 16>{}))
        ```

        ![](../../figure/分形变换-0.png)

        通过[MakeZNLayout](../constructor/MakeZNLayout.md)接口可以快速的构造连续的ZN Layout格式，自行设置StrideRow1和StrideColum1可以构造非连续的ZN Layout格式。

-   RowMajor Layout，一种使用层次化表述法描述的二维行优先排布格式。

    RowMajor Layout格式的ShapeRow0和ShapeColumn0为固定值，即内层矩阵的Shape为1 \* 1，StrideRow0和StrideColumn0也为固定值0，内层矩阵只有一个元素，没有Stride的概念。StrideColumn1为1，表示外层矩阵列方向上是连续的。

    ```
    Layout = ((Shape) : (Stride))
    Shape = ((Std::Int<1>{}, ShapeRow1), (Std::Int<1>{}, ShapeColumn1))
    Stride = ((Std::Int<0>{}, StrideRow1), (Std::Int<0>{}, Std::Int<1>{}))
    ```

    下面是一个连续的RowMajor Layout示例。

    ```
    Layout = ((Shape) : (Stride))
    Shape = ((Std::Int<1>{}, row),(Std::Int<1>{}, column))
    Stride = ((Std::Int<0>{}, column),(Std::Int<0>{}, Std::Int<1>{}))
    ```

    ![](../../figure/分形变换-1.png)

    通过[MakeRowMajorLayout](../constructor/MakeRowMajorLayout.md)接口可以快速的构造连续的RowMajor Layout格式，自行设置StrideRow1可以构造非连续的RowMajor Layout格式。

-   ColumnMajor Layout，一种使用层次化表述法描述的二维列优先排布格式。

    ColumnMajor Layout格式的ShapeRow0和ShapeColumn0为固定值，即内层矩阵的Shape为1 \* 1，StrideRow0和StrideColumn0也为固定值0，内层矩阵只有一个元素，没有Stride的概念。StrideRow1为1，表示外层矩阵行方向上是连续的。

    ```
    Layout = ((Shape) : (Stride))
    Shape = ((Std::Int<1>{}, ShapeRow1), (Std::Int<1>{}, ShapeColumn1))
    Stride = ((Std::Int<0>{}, Std::Int<1>{}), (Std::Int<0>{}, StrideColumn1))
    ```

    下面是一个连续的ColumnMajor Layout示例。

    ```
    Layout = ((Shape) : (Stride))
    Shape = ((Std::Int<1>{}, row),(Std::Int<1>{}, column))
    Stride = ((Std::Int<0>{}, Std::Int<1>{}),(Std::Int<0>{}, row))
    ```

    ![](../../figure/分形变换-2.png)

    通过[MakeColumnMajorLayout](../constructor/MakeColumnMajorLayout.md)接口可以快速的构造连续的ColumnMajor Layout格式，自行设置StrideColumn1值可以构造非连续的ColumnMajor Layout格式。

