# asc_sync_block_arrive

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

## 功能说明

和[asc_sync_block_wait](asc_sync_block_wait.md)配合使用（通过flagID关联），用于向核间同步寄存器发送同步信号，标识当前pipeline已到达flagID对应的同步点。每个flagID对应一个初始值为0的计数器，执行asc_sync_block_arrive后flagID对应的计数器增加1；执行asc_sync_block_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。

## 函数原型

```cpp
 __aicore__ inline void asc_sync_block_arrive(pipe_t pipe, uint8_t mode, int64_t flagID)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。|
| mode | 输入 | 核间同步模式，用于指定同步的行为类型。共有三种同步模式：<br/>mode 0：AI Core核间的同步控制。对于AIC场景，同步所有的AIC核，直到所有的AIC核都执行到asc_sync_block_arrive时，asc_sync_block_wait后续的指令才会执行；对于AIV场景，同步所有的AIV核，直到所有的AIV核都执行到asc_sync_block_arrive时，asc_sync_block_wait后续的指令才会执行。<br/>mode 1：AI Core内部，AIV核之间的同步控制。如果两个AIV核都运行了asc_sync_block_arrive，asc_sync_block_wait后续的指令才会执行。<br/>mode 2：AI Core内部，AIC与AIV之间的同步控制。在AIC核执行了asc_sync_block_arrive之后，两个AIV上的asc_sync_block_wait后续的指令才会继续执行，两个AIV核都执行了asc_sync_block_arrive之后，AIC上的asc_sync_block_wait后续的指令才能执行。|
| flagID | 输入 | 事件标号，用于标识同一组同步信号。取值范围为[0,10]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 使用该同步接口时，需要按照如下规则设置Kernel类型：
  - 在纯Vector/Cube场景下，需设置Kernel类型为KERNEL_TYPE_MIX_AIV_1_0或KERNEL_TYPE_MIX_AIC_1_0。
  - 对于Vector和Cube混合场景，需根据实际情况灵活配置Kernel类型。
- 不建议开发者同时使用该接口和Matmul高阶API，否则会有flagID冲突的风险。
- 同一flagID的计数器最多设置15次。

## 调用示例

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
uint8_t mode = 0;
int64_t flagID = 1;
asc_sync_block_arrive(PIPE_S, mode, flagID);
asc_sync_block_wait(flagID);  
```
