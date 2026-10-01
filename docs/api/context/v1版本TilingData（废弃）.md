# v1版本TilingData（废弃）<a name="ZH-CN_TOPIC_0000001940699904"></a>

> [!NOTE]说明 
>该结构体废弃，并将在后续版本移除，请不要使用该结构体。无需直接对该结构体中的成员进行设置，统一使用HCCL Tiling提供的接口设置即可。
>对于本节介绍的TilingData结构体，当构建通信计算融合算子时，通算融合算子的TilingData结构体中，计算Tiling结构体部分必须在本节的通信Tiling结构体后追加。
>对于v1和v2两个版本的TilingData，Tiling结构体的第一个uint32\_t字段用于区分两个版本，即[v1版本](#table4835205712588)的preparePosition字段，[v2版本](v2版本TilingData（废弃）.md#table4835205712588)的version字段。若使用v2版本的Tiling结构体，则必须设置version=2；若使用v1版本的Tiling结构体，则设置preparePosition=1。用户使用任意版本的TilingData时，都必须严格按照对应版本的Tiling结构体，将其作为算子TilingData结构体的组成部分。

## 功能说明<a name="section77005157231"></a>

AI CPU启动下发通信任务前，需获取固定的通信配置[Mc2Msg](#table4835205712588)。在算子实现中，由Tiling组装通信配置项，通过配置固定参数和固定参数顺序的Tiling Data，将通信配置信息在调用AI CPU通信接口时传递给AI CPU。

## 参数说明<a name="section146744150261"></a>

**表 1**  Mc2Msg参数说明

<a name="table4835205712588"></a>
<table><thead align="left"><tr id="row118356578583"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="p48354572582"><a name="p48354572582"></a><a name="p48354572582"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p583535795817"><a name="p583535795817"></a><a name="p583535795817"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row1835857145817"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p122027792718"><a name="p122027792718"></a><a name="p122027792718"></a>preparePosition</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p386253782517"><a name="p386253782517"></a><a name="p386253782517"></a>设置服务端组装任务的方式，用户需要在Tiling中显式赋值，uint32_t类型，当前支持的取值如下：</p>
<p id="p1919419451692"><a name="p1919419451692"></a><a name="p1919419451692"></a>1：AI CPU与AI Core通过通信任务机制实现消息传递和任务下发；由AI Core侧通过消息通知时设置为1，即算子中使用<a href="HCCL-Kernel侧接口.md">HCCL</a>时设置为1。</p>
</td>
</tr>
<tr id="row18835145716587"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1383515717581"><a name="p1383515717581"></a><a name="p1383515717581"></a>sendOff</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p189653101104"><a name="p189653101104"></a><a name="p189653101104"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row14855142316314"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p64008366310"><a name="p64008366310"></a><a name="p64008366310"></a>recvOff</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p42861147172219"><a name="p42861147172219"></a><a name="p42861147172219"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row10918154010310"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p720815561636"><a name="p720815561636"></a><a name="p720815561636"></a>tailSendOff</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p61755217103"><a name="p61755217103"></a><a name="p61755217103"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row18564154310314"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1537831043"><a name="p1537831043"></a><a name="p1537831043"></a>tailRecvOff</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1088504892216"><a name="p1088504892216"></a><a name="p1088504892216"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row1373912464310"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p204822812411"><a name="p204822812411"></a><a name="p204822812411"></a>sendCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p675711257108"><a name="p675711257108"></a><a name="p675711257108"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row677514491431"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p353017121945"><a name="p353017121945"></a><a name="p353017121945"></a>recvCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p5571727161016"><a name="p5571727161016"></a><a name="p5571727161016"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row58011214842"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p18399722544"><a name="p18399722544"></a><a name="p18399722544"></a>tailSendCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p3847122920109"><a name="p3847122920109"></a><a name="p3847122920109"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row2896316644"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p77126142"><a name="p77126142"></a><a name="p77126142"></a>tailRecvCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p33062528225"><a name="p33062528225"></a><a name="p33062528225"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row1394313331241"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p0544421549"><a name="p0544421549"></a><a name="p0544421549"></a>totalCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p19740534161010"><a name="p19740534161010"></a><a name="p19740534161010"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row1368418363417"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p388864612415"><a name="p388864612415"></a><a name="p388864612415"></a>turnNum</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p59954538226"><a name="p59954538226"></a><a name="p59954538226"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row233316546413"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p58931871058"><a name="p58931871058"></a><a name="p58931871058"></a>tailNum</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p9246105552213"><a name="p9246105552213"></a><a name="p9246105552213"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row823310313517"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p138511218518"><a name="p138511218518"></a><a name="p138511218518"></a>stride</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1096355582216"><a name="p1096355582216"></a><a name="p1096355582216"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row138201181051"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p15482024153"><a name="p15482024153"></a><a name="p15482024153"></a>workspaceOff</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p272765613224"><a name="p272765613224"></a><a name="p272765613224"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row4804115353"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p428462913516"><a name="p428462913516"></a><a name="p428462913516"></a>notifyOff</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p46851257132218"><a name="p46851257132218"></a><a name="p46851257132218"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row873914315519"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1989813424511"><a name="p1989813424511"></a><a name="p1989813424511"></a>notifyBeginCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p9514205842219"><a name="p9514205842219"></a><a name="p9514205842219"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row5911933454"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p11391348259"><a name="p11391348259"></a><a name="p11391348259"></a>notifyEndCnt</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p12042059172212"><a name="p12042059172212"></a><a name="p12042059172212"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row1971211365519"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1316124261814"><a name="p1316124261814"></a><a name="p1316124261814"></a>useBufferType</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p362513191862"><a name="p362513191862"></a><a name="p362513191862"></a>设置通信算法获取输入数据的位置，uint8_t类型，参数取值如下：</p>
<a name="ul186251119268"></a><a name="ul186251119268"></a><ul id="ul186251119268"><li>0：默认值，默认通信输入不放在windows中，其中windows为其他卡可访问的共享缓冲区。</li><li>1：通信输入不放在windows中，当前该参数取值1与取值0的功能一致。</li><li>2：通信输入放在windows中，仅适用于AllReduce算法。</li></ul>
</td>
</tr>
<tr id="row18821124501818"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1224895817182"><a name="p1224895817182"></a><a name="p1224895817182"></a>funID</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p116211604231"><a name="p116211604231"></a><a name="p116211604231"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row14390048111818"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1852153191915"><a name="p1852153191915"></a><a name="p1852153191915"></a>dataType</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p18310611237"><a name="p18310611237"></a><a name="p18310611237"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row472965216183"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p16278208141916"><a name="p16278208141916"></a><a name="p16278208141916"></a>groupNum</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p20900913230"><a name="p20900913230"></a><a name="p20900913230"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row073410101192"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p7347161841914"><a name="p7347161841914"></a><a name="p7347161841914"></a>reuseMode</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p641023132317"><a name="p641023132317"></a><a name="p641023132317"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row184116136194"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1086072218195"><a name="p1086072218195"></a><a name="p1086072218195"></a>commType</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p910715442312"><a name="p910715442312"></a><a name="p910715442312"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row187059283191"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p2611534191912"><a name="p2611534191912"></a><a name="p2611534191912"></a>reduceOp</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1677614418233"><a name="p1677614418233"></a><a name="p1677614418233"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row1759283613191"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p137591340201920"><a name="p137591340201920"></a><a name="p137591340201920"></a>commOrder</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p73731156232"><a name="p73731156232"></a><a name="p73731156232"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row26861343141914"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p975620475192"><a name="p975620475192"></a><a name="p975620475192"></a>waitPolicy</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1867126132312"><a name="p1867126132312"></a><a name="p1867126132312"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row670911504199"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p26605541199"><a name="p26605541199"></a><a name="p26605541199"></a>rspPolicy</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p26906610230"><a name="p26906610230"></a><a name="p26906610230"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row736813219203"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1714128202019"><a name="p1714128202019"></a><a name="p1714128202019"></a>exitPolicy</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p241117182314"><a name="p241117182314"></a><a name="p241117182314"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row04198108203"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1416221415206"><a name="p1416221415206"></a><a name="p1416221415206"></a>commAlg</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1571952423615"><a name="p1571952423615"></a><a name="p1571952423615"></a>设置具体通信算法，用户需要在Tiling中显式赋值，uint8_t类型，当前支持的取值如下：</p>
<p id="p34441520123912"><a name="p34441520123912"></a><a name="p34441520123912"></a>1：FullMesh算法，即NPU之间的全连接，任意两个NPU之间可以直接进行数据收发。</p>
</td>
</tr>
<tr id="row9902101682017"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1847792110209"><a name="p1847792110209"></a><a name="p1847792110209"></a>taskType</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p6746134831113"><a name="p6746134831113"></a><a name="p6746134831113"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row20622102513205"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1398153482010"><a name="p1398153482010"></a><a name="p1398153482010"></a>debugMode</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1691085031112"><a name="p1691085031112"></a><a name="p1691085031112"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row16623386203"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p12051455208"><a name="p12051455208"></a><a name="p12051455208"></a>stepSize</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p108841910236"><a name="p108841910236"></a><a name="p108841910236"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row209131473204"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p20931254172011"><a name="p20931254172011"></a><a name="p20931254172011"></a>sendArgIndex</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1367453552317"><a name="p1367453552317"></a><a name="p1367453552317"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row1721155711201"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1228919122118"><a name="p1228919122118"></a><a name="p1228919122118"></a>recvArgIndex</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p17866124714235"><a name="p17866124714235"></a><a name="p17866124714235"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row431104102120"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1240919872117"><a name="p1240919872117"></a><a name="p1240919872117"></a>commOutArgIndex</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p118671846247"><a name="p118671846247"></a><a name="p118671846247"></a>预留参数，不可配置。</p>
</td>
</tr>
<tr id="row326751232115"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p55261916132110"><a name="p55261916132110"></a><a name="p55261916132110"></a>hasCommOut</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p14152163919816"><a name="p14152163919816"></a><a name="p14152163919816"></a>本卡的通信算法的计算结果是否输出到recvBuf（目的数据buffer地址）。仅AllGather算法与AlltoAll算法支持配置该参数。uint8_t类型，参数取值如下：</p>
<a name="ul425920531911"></a><a name="ul425920531911"></a><ul id="ul425920531911"><li>0：不输出本卡通信算法的计算结果。在无需输出通信结果时，配置参数值为0，此时不会拷贝本卡的通信结果数据，可提升算子性能。例如，在8卡场景下，本卡只取其他卡的部分数据，这时可配置本参数为0。</li><li>1：输出本卡通信算法的计算结果。</li></ul>
</td>
</tr>
<tr id="row13178151572711"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p12178161515274"><a name="p12178161515274"></a><a name="p12178161515274"></a>reserve</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1217814154275"><a name="p1217814154275"></a><a name="p1217814154275"></a>保留字段。</p>
</td>
</tr>
<tr id="row1929961210272"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1329971217279"><a name="p1329971217279"></a><a name="p1329971217279"></a>reserve2</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1329911121271"><a name="p1329911121271"></a><a name="p1329911121271"></a>保留字段。</p>
</td>
</tr>
</tbody>
</table>

## 约束说明<a name="section633mcpsimp"></a>

-   算子的Tiling Data结构需要按顺序完整包含[Mc2Msg参数](#table4835205712588)。
-   AI CPU需获取固定数据结构的通信配置，算子注册Tiling Data时保持该结构的一致性。
-   Atlas A3 训练系列产品/Atlas A3 推理系列产品暂不支持该版本TilingData。

## 调用示例<a name="section9414023616"></a>

以自定义算子AllGatherMatmulCustom为例，如下为该算子的算子原型，"gather\_out"为通信任务AllGather的输出。

```
[
    {
        "op": "AllGatherMatmulCustom",
        "input_desc": [
            {
                "name": "x1",
                "param_type": "required",
                "format": [
                    "ND",
		    "ND"
                ],
                "type": [
                    "float16",
                    "bfloat16"
                ]
            },
            {
                "name": "x2",
                "param_type": "required",
                "format": [
                    "ND",
		    "ND"
                ],
                "type": [
                    "float16",
                    "bfloat16"
                ]
            },
            {
                "name": "bias",
                "param_type": "optional",
                "format": [
                    "ND",
		    "ND"
                ],
                "type": [
                    "float16",
                    "bfloat16"
                ]
            }
        ],
        "output_desc":[
            {
                "name": "y",
                "param_type": "required",
                "format": [
                    "ND",
		    "ND"
                ],
                "type": [
                    "float16",
                    "bfloat16"
                ]
            },
            {
                "name": "gather_out",
                "param_type": "required",
                "format": [
                    "ND",
		    "ND"
                ],
                "type": [
                    "float16",
                    "bfloat16"
                ]
            }
        ],
        "attr": [
            {
                "name": "group",
                "type": "string",
                "default_value":"",
                "param_type":"required"
            },
            {
                "name": "rank_size",
                "type": "int",
                "default_value":0,
                "param_type":"optional"
            },
            {
                "name": "is_gather_out",
                "type": "bool",
                "default_value":true,
                "param_type":"optional"
            }
        ]
    }
]
```

算子的Tiling Data结构需要按顺序完整包含Mc2Msg参数，如下为算子Tiling Data代码示例。

```
// 声明Mc2Msg结构
BEGIN_TILING_DATA_DEF(Mc2Msg)
    TILING_DATA_FIELD_DEF(uint32_t, preparePosition);
    TILING_DATA_FIELD_DEF(uint32_t, sendOff); 
    TILING_DATA_FIELD_DEF(uint32_t, recvOff);
    TILING_DATA_FIELD_DEF(uint32_t, tailSendOff);
    TILING_DATA_FIELD_DEF(uint32_t, tailRecvOff);
    TILING_DATA_FIELD_DEF(uint64_t, sendCnt);
    TILING_DATA_FIELD_DEF(uint32_t, recvCnt);
    TILING_DATA_FIELD_DEF(uint32_t, tailSendCnt);
    TILING_DATA_FIELD_DEF(uint32_t, tailRecvCnt);
    TILING_DATA_FIELD_DEF(uint32_t, totalCnt);
    TILING_DATA_FIELD_DEF(uint32_t, turnNum);
    TILING_DATA_FIELD_DEF(uint32_t, tailNum);
    TILING_DATA_FIELD_DEF(uint32_t, stride);
    TILING_DATA_FIELD_DEF(uint32_t, workspaceOff);
    TILING_DATA_FIELD_DEF(uint32_t, notifyOff);
    TILING_DATA_FIELD_DEF(uint16_t, notifyBeginCnt);
    TILING_DATA_FIELD_DEF(uint16_t, notifyEndCnt);
    TILING_DATA_FIELD_DEF(uint8_t, useBufferType);
    TILING_DATA_FIELD_DEF(uint8_t, funID);
    TILING_DATA_FIELD_DEF(uint8_t, dataType);
    TILING_DATA_FIELD_DEF(uint8_t, groupNum);
    TILING_DATA_FIELD_DEF(uint8_t, reuseMode);
    TILING_DATA_FIELD_DEF(uint8_t, commType);
    TILING_DATA_FIELD_DEF(uint8_t, reduceOp);
    TILING_DATA_FIELD_DEF(uint8_t, commOrder);
    TILING_DATA_FIELD_DEF(uint8_t, waitPolicy);
    TILING_DATA_FIELD_DEF(uint8_t, rspPolicy);
    TILING_DATA_FIELD_DEF(uint8_t, exitPolicy);
    TILING_DATA_FIELD_DEF(uint8_t, commAlg);
    TILING_DATA_FIELD_DEF(uint8_t, taskType);
    TILING_DATA_FIELD_DEF(uint8_t, debugMode);
    TILING_DATA_FIELD_DEF(uint8_t, stepSize);
    TILING_DATA_FIELD_DEF(uint8_t, sendArgIndex);
    TILING_DATA_FIELD_DEF(uint8_t, recvArgIndex);
    TILING_DATA_FIELD_DEF(uint8_t, commOutArgIndex);
    TILING_DATA_FIELD_DEF(uint8_t, hasCommOut);
    TILING_DATA_FIELD_DEF(uint8_t, reserve);
    TILING_DATA_FIELD_DEF(uint32_t, reserve2);
END_TILING_DATA_DEF;
REGISTER_TILING_DATA_CLASS(Mc2MsgOp, Mc2Msg)

BEGIN_TILING_DATA_DEF(AllGatherMatmulCustomTilingData)
    TILING_DATA_FIELD_DEF_STRUCT(Mc2Msg, msg);
END_TILING_DATA_DEF;
```

```
// 配置Mc2Msg
AllGatherMatmulCustomTilingData tiling;
tiling.msg.set_preparePosition(1);
tiling.msg.set_commAlg(1);
tiling.msg.set_useBufferType(1);
tiling.msg.set_hasCommOut(1);
```

