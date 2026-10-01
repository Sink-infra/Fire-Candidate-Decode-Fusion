# SetSkipBufferWindowCopy<a name="ZH-CN_TOPIC_0000002120978198"></a>

## 功能说明<a name="section618mcpsimp"></a>

设置通信算法获取输入数据的位置。

## 函数原型<a name="section620mcpsimp"></a>

```
uint32_t SetSkipBufferWindowCopy(uint8_t skipBufferWindowCopy)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

<a name="table9646134355611"></a>
<table><thead align="left"><tr id="row964714433565"><th class="cellrowborder" valign="top" width="14.99%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0235751031_p20917673"><a name="zh-cn_topic_0235751031_p20917673"></a><a name="zh-cn_topic_0235751031_p20917673"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="12.02%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0235751031_p16609919"><a name="zh-cn_topic_0235751031_p16609919"></a><a name="zh-cn_topic_0235751031_p16609919"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="72.99%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0235751031_p59995477"><a name="zh-cn_topic_0235751031_p59995477"></a><a name="zh-cn_topic_0235751031_p59995477"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row106481443135617"><td class="cellrowborder" valign="top" width="14.99%" headers="mcps1.2.4.1.1 "><p id="p167361341213"><a name="p167361341213"></a><a name="p167361341213"></a>skipBufferWindowCopy</p>
</td>
<td class="cellrowborder" valign="top" width="12.02%" headers="mcps1.2.4.1.2 "><p id="p137362417119"><a name="p137362417119"></a><a name="p137362417119"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="72.99%" headers="mcps1.2.4.1.3 "><p id="p141451616122611"><a name="p141451616122611"></a><a name="p141451616122611"></a>表示通信算法获取输入数据的位置，uint8_t类型。</p>
<p id="p32561243597"><a name="p32561243597"></a><a name="p32561243597"></a>针对<span id="ph1292674871116"><a name="ph1292674871116"></a><a name="ph1292674871116"></a><term id="zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span>，参数取值如下：</p>
<a name="ul186251119268"></a><a name="ul186251119268"></a><ul id="ul186251119268"><li>0：通信输入不放在windows中。未调用该接口设置通信输入的位置时，默认通信输入不放在windows中。其中windows为其他卡可访问的共享缓冲区。</li><li>1：通信输入不放在windows中，当前该参数取值1与取值0的功能一致。</li><li>2：通信输入放在windows中，仅适用于AllReduce算法、AlltoAll算法。</li></ul>
<p id="p66101353172418"><a name="p66101353172418"></a><a name="p66101353172418"></a>针对<span id="ph690123416432"><a name="ph690123416432"></a><a name="ph690123416432"></a><term id="zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span>，该参数为预留字段，配置后不生效。</p>
</td>
</tr>
</tbody>
</table>

## 返回值说明<a name="section640mcpsimp"></a>

-   0表示设置成功。
-   非0表示设置失败。

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section1665082013318"></a>

本接口的调用示例请见[调用示例](SetOpType.md#section1665082013318)。

