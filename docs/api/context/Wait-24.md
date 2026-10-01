# Wait<a name="ZH-CN_TOPIC_0000001960384820"></a>

## 产品支持情况<a name="section1550532418810"></a>

<a name="zh-cn_topic_0000001965821852_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001965821852_row20831180131817"><th class="cellrowborder" valign="top" width="57.95%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000001965821852_p1883113061818"><a name="zh-cn_topic_0000001965821852_p1883113061818"></a><a name="zh-cn_topic_0000001965821852_p1883113061818"></a><span id="zh-cn_topic_0000001965821852_ph20833205312295"><a name="zh-cn_topic_0000001965821852_ph20833205312295"></a><a name="zh-cn_topic_0000001965821852_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42.05%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000001965821852_p783113012187"><a name="zh-cn_topic_0000001965821852_p783113012187"></a><a name="zh-cn_topic_0000001965821852_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001965821852_row220181016240"><td class="cellrowborder" valign="top" width="57.95%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000001965821852_p48327011813"><a name="zh-cn_topic_0000001965821852_p48327011813"></a><a name="zh-cn_topic_0000001965821852_p48327011813"></a><span id="zh-cn_topic_0000001965821852_ph583230201815"><a name="zh-cn_topic_0000001965821852_ph583230201815"></a><a name="zh-cn_topic_0000001965821852_ph583230201815"></a><term id="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.05%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000001965821852_p7948163910184"><a name="zh-cn_topic_0000001965821852_p7948163910184"></a><a name="zh-cn_topic_0000001965821852_p7948163910184"></a>x</p>
</td>
</tr>
<tr id="zh-cn_topic_0000001965821852_row173226882415"><td class="cellrowborder" valign="top" width="57.95%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000001965821852_p14832120181815"><a name="zh-cn_topic_0000001965821852_p14832120181815"></a><a name="zh-cn_topic_0000001965821852_p14832120181815"></a><span id="zh-cn_topic_0000001965821852_ph1483216010188"><a name="zh-cn_topic_0000001965821852_ph1483216010188"></a><a name="zh-cn_topic_0000001965821852_ph1483216010188"></a><term id="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001965821852_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42.05%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000001965821852_p19948143911820"><a name="zh-cn_topic_0000001965821852_p19948143911820"></a><a name="zh-cn_topic_0000001965821852_p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000001526206862_section212607105720"></a>

阻塞该AIV需要等待Arrive组全部完成任务，再开始执行任务。

## 函数原型<a name="section765814724715"></a>

```
__aicore__ inline void Wait(uint32_t waitIndex)
```

## 参数说明<a name="zh-cn_topic_0000001526206862_section129451113125413"></a>

**表 1**  接口参数说明

<a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_table111938719446"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_row6223476444"><th class="cellrowborder" valign="top" width="17.22%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p10223674448"><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p10223674448"></a><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p10223674448"></a>参数</p>
</th>
<th class="cellrowborder" valign="top" width="15.340000000000002%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p645511218169"><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p645511218169"></a><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p645511218169"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="67.44%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p1922337124411"><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p1922337124411"></a><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p1922337124411"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_row152234713443"><td class="cellrowborder" valign="top" width="17.22%" headers="mcps1.2.4.1.1 "><p id="p2055524619119"><a name="p2055524619119"></a><a name="p2055524619119"></a>waitIndex</p>
</td>
<td class="cellrowborder" valign="top" width="15.340000000000002%" headers="mcps1.2.4.1.2 "><p id="p19741912147"><a name="p19741912147"></a><a name="p19741912147"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="67.44%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p2684123934216"><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p2684123934216"></a><a name="zh-cn_topic_0000001526206862_zh-cn_topic_0000001389783361_p2684123934216"></a>该AIV在Wait组的序号。范围为[0, waitSize - 1]。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

无。

## 约束说明<a name="zh-cn_topic_0000001526206862_section65498832"></a>

该接口支持在循环中使用，但是受限于多核间通信效率要求，循环最大次数不超过1,048,575次。

## 调用示例<a name="zh-cn_topic_0000001526206862_section97001499599"></a>

```
if (id >= 0 && id < ARRIVE_NUM) {
  //各种Vector计算逻辑，用户自行实现
  barA.Arrive(id);
} else if(id >= ARRIVE_NUM && id < ARRIVE_NUM + WAIT_NUM){
  barA.Wait(id - ARRIVE_NUM);                            // Wait组的6个AIV中的AIV需要等待Arrive组AIV做完任务
  // 各种Vector计算逻辑，用户自行实现
}
```

