# FireCandidateDecodeFusion 测试

测试按 Coverage、Correctness、Mock Cases 三类组织。三份 JSON 声明案例参数，
共用模块提供 CaseSpec、分类加载、输入物化、参考公式、二进制数据生成和性能采样。
数学参考依据 [题目规范](../FireCandidateDecodeFusion.md)。

| 类别 | 案例文件 | 数量 | 用途 |
| --- | --- | ---: | --- |
| Coverage | [st/cases/coverage.json](st/cases/coverage.json) | 55（22 合法、33 契约拒绝） | 类型、形状、布局、取值和输入边界 |
| Correctness | [st/cases/correctness.json](st/cases/correctness.json) | 28 | 解码公式、float32 中间精度、共享 priors、候选顺序及输出转换 |
| Mock Cases | [st/cases/mock.json](st/cases/mock.json) | 12 | 固定 Case 编号、形状与类型映射、NPU 正确性和性能计时 |

三类合计 95 个 Case，各类独立选择和生成数据。NumPy 用于输入和数学参考，
算子实现位于 `code/`。

## 目录

```text
tests/
├── README.md
├── requirements.txt
├── __main__.py              # 三类 Case 数量、类型与规模统计
├── common/
│   ├── cases.py             # CaseSpec、JSON 加载、分类和标签选择
│   ├── reference.py         # 确定性输入、契约校验和 float32 参考公式
│   ├── tensor_io.py         # Coverage/Correctness 二进制、元数据和 manifest
│   ├── mock_cases.py        # 固定 Mock 输入及参考数据
│   └── profiling.py         # 执行配置、采样映射和进程结果汇总
├── st/
│   ├── cases/
│   │   ├── coverage.json
│   │   ├── correctness.json
│   │   └── mock.json
│   ├── runner/
│   │   └── fire_candidate_decode_fusion_runner.cpp  # ACLNN/NPU 执行
│   └── scripts/
│       ├── gen_data.py      # 分类查看与数据生成入口
│       └── profile_mock_workloads.py  # 当前算子包的 Mock 性能测试
└── ut/
    ├── helpers.py           # 独立标量公式和共用套件检查
    ├── test_case_matrix.py
    ├── test_reference.py
    ├── test_data_generation.py
    ├── test_mock_cases.py
    └── test_profile_mapping.py
```

JSON 顶层的 `category` 指定类别，`cases` 声明案例名称、输入 shape/dtype、数据模式、
seed、预期结果和 `covers` 标签。`common/cases.py` 加载不可变 CaseSpec；
标签和名称选择仅在给定类别内进行。

UT 按职责组织：

| 文件 | 内容 |
| --- | --- |
| `test_case_matrix.py` | 分类、输入契约、边界矩阵、JSON 配置与选择规则 |
| `test_reference.py` | 独立标量公式、float32 中间精度、候选配对和输出转换 |
| `test_data_generation.py` | 确定性输入、物理布局、序列化、元数据与生成入口 |
| `test_mock_cases.py` | 固定编号、形状/类型映射、合成数据和参考公式 |
| `test_profile_mapping.py` | 执行配置、Profiler 映射、进程采样汇总、失败处理和 runner 编译检查 |

## 案例选择与数据生成

在仓库根目录使用 Python 3.9 或更新版本：

```bash
python3 -m pip install -r tests/requirements.txt
python3 -m tests --details
python3 -m unittest discover -s tests/ut -t . -v

python3 -m tests.st.scripts.gen_data --suite coverage --list
python3 -m tests.st.scripts.gen_data --suite correctness --list
python3 -m tests.st.scripts.gen_data --suite mock --list
```

`--suite` 选择 `coverage`、`correctness` 或 `mock`；`--case` 可重复指定完整案例名称；
`--tag` 要求案例同时包含给定的所有 `covers` 标签。
`--expectation` 支持 `all`、`success`、`reject`。
`--list` 仅显示选中的案例参数。

```bash
python3 -m tests.st.scripts.gen_data --suite coverage --output build/tests/data/coverage
python3 -m tests.st.scripts.gen_data --suite correctness --output build/tests/data/correctness
python3 -m tests.st.scripts.gen_data --suite mock --output build/tests/data/mock

python3 -m tests.st.scripts.gen_data --suite coverage --expectation success --output build/tests/data/coverage-valid
python3 -m tests.st.scripts.gen_data --suite correctness --tag float32_intermediates --output build/tests/data/fp32-intermediates
python3 -m tests.st.scripts.gen_data --suite mock --case mock_12 --output build/tests/data/mock-tail
```

生成目录必须不存在。二进制采用小端存储，按 Batch → Candidate → Channel 的 C 顺序
展开。生成数据置于被 Git 忽略的 `build/` 下。

## Coverage

合法用例覆盖 `B=1…8`、float16/float32、`N=1/31/32/33/63/64/65/32768`，
以及 `N=127/129/4096/16384/32767`。包含最大输入 `[8,32768,6]`、大尾块、
跨 Batch 共享 priors、非整数 stride、小 anchor 和非负网格值。

契约拒绝用例覆盖错误 Rank、通道宽度、B/N 越界、N 不匹配、错误 DType、非 ND、
非连续布局、输入 NaN/±Inf、非正 stride/anchor 和负网格坐标。
`expected_error` 是测试端契约标签。拒绝用例只生成输入和元数据；非连续输入保留
带填充的物理存储与实际 strides。

## Correctness 与参考精度

用例覆盖题面数值示例、尾块、六个输入通道和五个输出通道、宽高平方及 anchor/stride
缩放、置信度乘积、负中心坐标、低分候选、Sigmoid 饱和、float16 舍入和跨 Batch 布局。
`fp16_cancellation`、`fp16_width_underflow`、`fp16_large_float32_priors` 分别覆盖中心坐标
抵消、宽高平方下溢和超出 float16 范围的 priors。

参考计算先按声明类型量化输入，再将 raw_pred 提升为 float32，与 float32 priors
完成全部公式，最后转换到输出类型。输出顺序为 `[x,y,w,h,fire_score]`，形状 `[B,N,5]`。

| 输出类型 | 参考目标 | atol | rtol |
| --- | --- | ---: | ---: |
| float32 | float32 参考结果 | 1e-5 | 1e-5 |
| float16 | float32 参考结果最终转换到 float16 | 3e-3 | 3e-3 |

比较条件为 `abs(actual-expected) <= atol + rtol*abs(expected)`。
有限输入可能导致 float32 计算或最终 float16 转换溢出；相应参考保留 Infinity 的位置
和符号。

## 固定十二例 Mock

| float32 Case | float16 Case | raw_pred shape | priors shape |
| --- | --- | --- | --- |
| 1 | 7 | [1,32,6] | [32,5] |
| 2 | 8 | [2,33,6] | [33,5] |
| 3 | 9 | [4,4096,6] | [4096,5] |
| 4 | 10 | [4,4097,6] | [4097,5] |
| 5 | 11 | [8,32768,6] | [32768,5] |
| 6 | 12 | [8,32769,6] | [32769,5] |

名称为 `mock_01`～`mock_12`。priors 固定为 float32；decoded shape 为 `[B,N,5]`，
类型与 raw_pred 一致。第 6/12 例的 `N=32769` 超过题面 `N<=32768` 的范围，
Mock 按此形状生成数据，Coverage 的题面契约校验将其判为 `N_RANGE`。

每例使用 `NumPy default_rng(140)`。raw_pred 从 `[-4,4]` 均匀采样后转换到声明类型；
grid 为非负网格，stride=8，anchor 从 `[1,4]` 均匀采样并保存为 float32。
输入是固定种子的合成数据。参考按题面公式在 float32 中计算，最后转换到 decoded 类型，
容差与上表一致。

## Mock 性能测试

执行配置为 **ARM Linux、CANN 9.0.0**，当前算子编译目标为 `ascend910b`。
默认 SDK 路径为 `/usr/local/Ascend/cann-9.0.0`，设备编号为 0。
入口核对系统架构、SDK 版本、算子构建的 SDK 路径和计算单元；runner 在 Kernel 启动前
通过 `aclrtGetSocName()` 自动识别实际设备型号，默认不限定具体 SoC。
设备仍需与算子的 `ascend910b` 编译目标兼容。

在对应设备环境的仓库根目录执行：

```bash
source /usr/local/Ascend/cann-9.0.0/set_env.sh
cmake -S code -B build/mock-operator -DASCEND_COMPUTE_UNIT=ascend910b
cmake --build build/mock-operator -j8
python3 -m tests.st.scripts.profile_mock_workloads \
  --build build/mock-operator --output build/tests/mock-performance
```

每次测试按 Case 编号顺序启动真实 ACLNN 算子。默认启动 **3 个独立进程**，
每个进程对每例调用 **5 次**，`msprof op` 使用 `BasicInfo` 和 **5 次预热**。
逐例时延为全部 **15 个 `Task Duration(us)` 的算术平均值**。
该指标测量设备 Kernel 任务，输入生成、参考计算、编译、Host 初始化、内存分配和
输入输出搬运不计入该时延。

每次调用前将输出预填为 NaN，调用后保存输出并与对应参考按题面容差比较。
所有输出正确、采样数量完整且执行成功时，该例状态为 `Pass`。
缺失或重复采样、错误输出、执行错误、超时、输入变化和配置不匹配均返回失败。
Profiler 以实际 launch 顺序、Kernel 名和 Kernel 内编号关联样本。

`--case` 可重复选择案例，`--device` 指定设备编号，`--soc` 可选指定期望的完整 SoC 名称
（例如 `--soc Ascend910B4`）；指定时型号不匹配会在 Kernel 启动前失败，省略时自动识别。
`--toolkit` 指定 CANN 9.0.0 的
安装路径。`--processes`、`--repeat`、`--warmup` 和 `--timeout` 设置采样参数，并记录在
本次结果中。

```bash
python3 -m tests.st.scripts.profile_mock_workloads \
  --build build/mock-operator --case mock_12 \
  --processes 3 --repeat 5 --warmup 5 \
  --output build/tests/mock-performance-tail
```

输出目录必须不存在。根目录的 `results.json` 通过 `target.soc` 记录实际设备型号，
通过 `expected_soc` 记录可选的型号约束；各进程的设备型号必须一致。
结果还包含配置、输入身份、逐例原始样本、
平均时延和各进程状态；`process-*/` 包含命令、Profiler 日志及每次调用的实际输出。
生成结果位于 Git 忽略的 `build/` 下。通过返回 0，测试失败返回 1，配置或构建错误返回 2。

## 数据文件

Coverage/Correctness 每例目录以完整案例名称命名，包含 `raw_pred.bin`、`priors.bin`
和 `case.json`；合法用例另有 `reference_decoded.bin` 和输出转换前的
`reference_decoded_fp32.bin`。套件根目录的 `manifest.json` 记录案例清单；元数据包含
shape、dtype、布局、seed、容差与数据文件哈希。

Mock 的根目录包含 `cases.json`，记录编号、名称、形状、类型、seed 和容差。
逐例目录以两位编号命名，例如 `01/`、`12/`，包含：

| 文件 | 内容 |
| --- | --- |
| `raw_pred.bin` | float16 或 float32 输入 |
| `priors.bin` | float32 先验参数 |
| `reference.bin` | 与 decoded 类型一致的参考结果 |
| `case.conf` | Batch、候选数量和输入类型 |
| `fixture.json` | 三个二进制文件的 SHA256 |

案例配置随源码维护，输入与参考二进制由数据生成入口按需生成。
