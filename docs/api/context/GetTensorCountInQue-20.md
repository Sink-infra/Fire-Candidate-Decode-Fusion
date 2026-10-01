# GetTensorCountInQue<a name="ZH-CN_TOPIC_0000001810874244"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="zh-cn_topic_0000002019600172_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002019600172_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002019600172_p1883113061818"><a name="zh-cn_topic_0000002019600172_p1883113061818"></a><a name="zh-cn_topic_0000002019600172_p1883113061818"></a><span id="zh-cn_topic_0000002019600172_ph20833205312295"><a name="zh-cn_topic_0000002019600172_ph20833205312295"></a><a name="zh-cn_topic_0000002019600172_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002019600172_p783113012187"><a name="zh-cn_topic_0000002019600172_p783113012187"></a><a name="zh-cn_topic_0000002019600172_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002019600172_row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002019600172_p48327011813"><a name="zh-cn_topic_0000002019600172_p48327011813"></a><a name="zh-cn_topic_0000002019600172_p48327011813"></a><span id="zh-cn_topic_0000002019600172_ph583230201815"><a name="zh-cn_topic_0000002019600172_ph583230201815"></a><a name="zh-cn_topic_0000002019600172_ph583230201815"></a><term id="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002019600172_p7948163910184"><a name="zh-cn_topic_0000002019600172_p7948163910184"></a><a name="zh-cn_topic_0000002019600172_p7948163910184"></a>√</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002019600172_row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002019600172_p14832120181815"><a name="zh-cn_topic_0000002019600172_p14832120181815"></a><a name="zh-cn_topic_0000002019600172_p14832120181815"></a><span id="zh-cn_topic_0000002019600172_ph1483216010188"><a name="zh-cn_topic_0000002019600172_ph1483216010188"></a><a name="zh-cn_topic_0000002019600172_ph1483216010188"></a><term id="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000002019600172_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002019600172_p19948143911820"><a name="zh-cn_topic_0000002019600172_p19948143911820"></a><a name="zh-cn_topic_0000002019600172_p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="section618mcpsimp"></a>

查询Que中已入队的Tensor数量。

## 函数原型<a name="section620mcpsimp"></a>

```
__aicore__ inline int32_t GetTensorCountInQue()
```

## 参数说明<a name="section622mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

该接口不支持Tensor原地操作，即TQue的depth设置为0的场景。

## 返回值说明<a name="section640mcpsimp"></a>

Que中已入队的Tensor数量

## 调用示例<a name="section642mcpsimp"></a>

```
// 通过GetTensorCountInQue查询que中已入队的Tensor数量，当前通过AllocTensor接口分配了内存，并加入que中，num为1。
AscendC::TPipe pipe;
AscendC::TQueBind<AscendC::TPosition::VECOUT, AscendC::TPosition::GM, 4> que;
int num = 4;
int len = 1024;
pipe.InitBuffer(que, num, len);
AscendC::LocalTensor<half> tensor1 = que.AllocTensor<half>();
que.EnQue(tensor1);// 将tensor加入VECOUT的Queue中
int32_t numb = que.GetTensorCountInQue();
```

