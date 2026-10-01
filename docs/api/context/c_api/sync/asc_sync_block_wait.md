# asc_sync_block_wait

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

## 功能说明

和[asc_sync_block_arrive](asc_sync_block_arrive.md)配合使用（通过flagID关联），用于等待所有同步对象到达flagID对应的同步点。

## 函数原型

```cpp
__aicore__ inline void asc_sync_block_wait(int64_t flagID)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| flagID | 输入 | 事件标号，取值范围[0,10]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 使用该同步接口时，需要按照如下规则设置Kernel类型：
  - 在纯Vector/Cube场景下，需设置Kernel类型为KERNEL_TYPE_MIX_AIV_1_0或KERNEL_TYPE_MIX_AIC_1_0。
  - 对于Vector和Cube混合场景，需根据实际情况灵活配置Kernel类型。
- asc_sync_block_wait必须与[asc_sync_block_arrive](asc_sync_block_arrive.md)接口配合使用，避免计算核一直处于阻塞阶段。
- 如果执行asc_sync_block_wait时该flagID的计数器的值为0，则asc_sync_block_wait之后的所有指令都将被阻塞，直到该flagID的计数器的值不为0。同一个flagID的计数器最多设置15次。

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
