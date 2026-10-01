# asc_mmad

## 产品支持情况

| 产品     | 是否支持 |
| :----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

完成矩阵乘加操作。计算公式如下：

$$
c_{matrix} = (a_{matrix} * b_{matrix}) + c_{matrix}
$$

## 函数原型

- 常规计算
    ```cpp
    __aicore__ inline void asc_mmad_s4(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_s4(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix,  uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    
    ```
- 同步计算
    ```cpp
    __aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c_matrix, __ca__ int4b_t* a_matrix, __cb__ int4b_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix,  uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool k_direction_align, bool c_matrix_source, bool c_matrix_init_val);
    __aicore__ inline void asc_mmad_sync(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool k_direction_align, bool is_weight_offset, bool c_matrix_source, bool c_matrix_init_val);
    ```

## 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| c_matrix      | 输出    | 	目的操作数，结果矩阵。            |
| a_matrix      | 输入    |   源操作数，左矩阵A。|
| b_matrix      | 输入    | 	源操作数，右矩阵B。      |
| left_height   | 输入    | 	左矩阵height ，取值范围为[0,4095]。      |
| n_dim         | 输入    | 	左矩阵width、右矩阵height，取值范围为[0,4095]。      |
| right_width   | 输入    | 	右矩阵width，取值范围为[0,4095]。      |
| unit_flag     | 输入    | 	unit_flag是一种asc_mmad接口和Fixpipe指令细粒度的并行，使能该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于L0C Buffer累加的场景。取值说明如下： <br>&bull; 0：保留值； <br>&bull; 2：使能unit_flag，硬件执行完指令后，不会关闭unit_flag功能；<br>&bull; 3：使能unit_flag，硬件执行完指令后，会关闭unit_flag功能。<br> 使能该功能时，矩阵计算的unit_flag在最后一个分形设置为3，其余分形计算设置为2即可。     |
| k_direction_align  | 输入    | 当源操作数和目的操作数为float时，L0A和L0B中的矩阵在right_width方向上按ceil(right_width/16)*16方式都对齐到48，对于right_width=44，L0A/L0B中的所有12个分形都会被读取到Cube中，而对于right_width=36，只有L0A/L0B中的10个分形会被读取到Cube中。 |
| is_weight_offset  | 输入    | 使能weight matrix offset。|
| c_matrix_source       | 输入    | 	配置C矩阵初始值是否来源于C2（存放Bias的硬件缓存区）。取值说明如下：  <br>&bull; true：来源于C2。 <br>&bull; false：来源于CO1(L0C)。 |
| c_matrix_init_val      | 输入    | 	配置C矩阵初始值是否为0。取值说明如下：  <br>&bull; true：C矩阵初始值为0。 <br>&bull; false：C矩阵初始值通过c_matrix_source参数进行配置。     |


## 返回值说明

无

## 流水类型

PIPE_M

## 约束说明

- 当left_height、right_width、n_dim中的任意一个值为0时，该指令不会被执行。
- 操作数地址对齐约束请参考[通用地址对齐约束](../general_instruction.md#通用地址对齐约束)。

## 调用示例

无
