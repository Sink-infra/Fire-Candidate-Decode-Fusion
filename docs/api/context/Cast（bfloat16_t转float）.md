# Cast（bfloat16\_t转float）<a name="ZH-CN_TOPIC_0000001623525812"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="table38301303189"></a>
<table><thead align="left"><tr id="row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="p1883113061818"><a name="p1883113061818"></a><a name="p1883113061818"></a><span id="ph20833205312295"><a name="ph20833205312295"></a><a name="ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="p783113012187"><a name="p783113012187"></a><a name="p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="row1272474920205"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p17301775812"><a name="p17301775812"></a><a name="p17301775812"></a><span id="ph2272194216543"><a name="ph2272194216543"></a><a name="ph2272194216543"></a>Ascend 950PR/Ascend 950DT</span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p37256491200"><a name="p37256491200"></a><a name="p37256491200"></a>√</p>
</td>
</tr>
<tr id="row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p48327011813"><a name="p48327011813"></a><a name="p48327011813"></a><span id="ph583230201815"><a name="ph583230201815"></a><a name="ph583230201815"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p7948163910184"><a name="p7948163910184"></a><a name="p7948163910184"></a>√</p>
</td>
</tr>
<tr id="row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="p14832120181815"><a name="p14832120181815"></a><a name="p14832120181815"></a><span id="ph1483216010188"><a name="ph1483216010188"></a><a name="ph1483216010188"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="p19948143911820"><a name="p19948143911820"></a><a name="p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

将输入数据转换为float类型。

## 函数原型<a name="section620mcpsimp"></a>

-   bfloat16\_t类型转换为float类型

    ```
    __aicore__ inline float Cast(const bfloat16_t& bVal)
    ```

-   支持多种数据类型转换为float类型

    ```
    template <typename T>
    __aicore__ constexpr inline float Cast(const T& bVal)
    ```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001429830437_row118356578583"><th class="cellrowborder" valign="top" width="16.28%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000001429830437_p48354572582"><a name="zh-cn_topic_0000001429830437_p48354572582"></a><a name="zh-cn_topic_0000001429830437_p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="83.72%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000001429830437_p583535795817"><a name="zh-cn_topic_0000001429830437_p583535795817"></a><a name="zh-cn_topic_0000001429830437_p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001429830437_row1835857145817"><td class="cellrowborder" valign="top" width="16.28%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000001429830437_p5835457165816"><a name="zh-cn_topic_0000001429830437_p5835457165816"></a><a name="zh-cn_topic_0000001429830437_p5835457165816"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="83.72%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000001429830437_p168351657155818"><a name="zh-cn_topic_0000001429830437_p168351657155818"></a><a name="zh-cn_topic_0000001429830437_p168351657155818"></a>操作数数据类型。</p>
<p id="p33891341206"><a name="p33891341206"></a><a name="p33891341206"></a><span id="ph18507114311268"><a name="ph18507114311268"></a><a name="ph18507114311268"></a>Ascend 950PR/Ascend 950DT</span>，支持的数据类型为：fp4x2_e2m1_t、fp4x2_e1m2_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、bfloat16_t。</p>
<p id="p378823974412"><a name="p378823974412"></a><a name="p378823974412"></a>fp4x2_e1m2_t和fp4x2_e2m1_t类型为了满足1byte数据大小，构造时由两个四位标量数据拼接生成。转换时，被转换的是fp4x2_e1m2_t和fp4x2_e2m1_t标量数据中低位4bit的值，即[0:3]位。</p>
</td>
</tr>
</tbody>
</table>

**表 2**  接口参数说明

<a name="table18368155193919"></a>
<table><thead align="left"><tr id="row1036805543911"><th class="cellrowborder" valign="top" width="16.38163816381638%" id="mcps1.2.4.1.1"><p id="p1836835511393"><a name="p1836835511393"></a><a name="p1836835511393"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="10.861086108610861%" id="mcps1.2.4.1.2"><p id="p10368255163915"><a name="p10368255163915"></a><a name="p10368255163915"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.75727572757276%" id="mcps1.2.4.1.3"><p id="p436875573911"><a name="p436875573911"></a><a name="p436875573911"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="row1436825518395"><td class="cellrowborder" valign="top" width="16.38163816381638%" headers="mcps1.2.4.1.1 "><p id="p941862411595"><a name="p941862411595"></a><a name="p941862411595"></a>bVal</p>
</td>
<td class="cellrowborder" valign="top" width="10.861086108610861%" headers="mcps1.2.4.1.2 "><p id="p941792465918"><a name="p941792465918"></a><a name="p941792465918"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.75727572757276%" headers="mcps1.2.4.1.3 "><p id="p14161124195918"><a name="p14161124195918"></a><a name="p14161124195918"></a>待转换的标量数据。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

转换后的float类型标量数据。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section19372434133520"></a>

```
void CalcFunc(bfloat16_t n)
{
	int dataLen = 32;
	AscendC::TPipe pipe;
	AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueSrcVecIn;
	AscendC::TQue<AscendC::TPosition::VECOUT, 1> inQueueDstVecIn;
	pipe.InitBuffer(inQueueDstVecIn, 1, dataLen * sizeof(bfloat16_t));
	pipe.InitBuffer(inQueueSrcVecIn, 1, dataLen * sizeof(float));
	AscendC::LocalTensor<bfloat16_t> dstLocal = inQueueDstVecIn.AllocTensor<bfloat16_t>();
	AscendC::LocalTensor<float> srcLocal = inQueueSrcVecIn.AllocTensor<float>();
	float t = AscendC::Cast(n);// 对标量进行加法，不支持bfloat16_t，需要先转换成float
	PipeBarrier<PIPE_ALL>();
	AscendC::Duplicate(srcLocal, float(4.0f), dataLen);
	PipeBarrier<PIPE_ALL>();
	Adds(srcLocal, srcLocal, t, dataLen);
	PipeBarrier<PIPE_ALL>();
	// 做加法运算后，输出bfloat16_t类型tensor
	Cast(dstLocal, srcLocal, AscendC::RoundMode::CAST_ROUND, dataLen);
	// ……
}
```

```
// 如下是一个输入类型fp4x2_e1m2_t的示例：
float fValue = AscendC::Cast<T>(srcLocal.GetValue(0));
AscendC::Duplicate(dstLocal, fValue, bufferSize);

// 输入数据(srcLocal)（二进制表达）: [0b10001111 0b10001111 0b10001111 ...]
// 输出数据(dstLocal): [-1.75 -1.75 -1.75 ...]
```

