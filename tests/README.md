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
│   ├── build.py             # 编译产物身份与校验
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
│   │   ├── fire_candidate_decode_fusion_runner.cpp  # ACLNN/NPU 执行
│   │   ├── platform_context.h
│   │   └── platform_context.cpp  # 进程内平台标签
│   └── scripts/
│       ├── build.py         # Bootstrap、Host、Final 构建
│       ├── gen_data.py      # 分类查看与数据生成入口
│       └── profile_mock_workloads.py  # 当前算子包的 Mock 性能测试
└── ut/
    ├── helpers.py           # 独立标量公式和共用套件检查
    ├── test_build.py
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
| `test_build.py` | 构建身份、共享配置和采样统计 |
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

每例使用 `NumPy default_rng(140)`。raw_pred 的合成元素从 `[-4,4]` 均匀采样后
转换到声明类型；grid 为非负网格，stride=8，anchor 从 `[1,4]` 均匀采样并保存为 float32。

C1/C2/C5/C6/C10/C11/C12 的 `observed_raw_pred_bits` 声明观测位置的 IEEE 位模式。
索引按 Batch → Candidate → Channel 展开；float16 使用四位十六进制，float32 使用八位。
生成器先合成数组，再填入声明位置的精确位模式，其余位置保留固定种子的合成值。
观测元素可以超出合成范围。原始输入、priors 和参考输出均保留文件哈希。

参考依据生成后的完整输入，按题面公式在 float32 中计算，最后转换到 decoded 类型；
容差与上表一致。

## Mock 构建与性能测试

执行环境为 ARM Linux、CANN 9.0.0，算子编译目标为 `ascend910b`。
算子 Host 使用 GNU 11.4.0、C++11、ABI=0；编译标准由构建命令传入。
默认 SDK 路径为 `/usr/local/Ascend/cann-9.0.0`，设备编号为 0。
Runner 使用 `aclrtGetSocName()` 识别实际 SoC；`--soc` 可指定完整型号约束。
共享配置中的 `platform_label_aliases` 规范化 Tiling 元数据中的名称标签；
实际设备名称和资源参数保持原值。该配置不校正或缩放计时。

构建入口从本地 Git 中的精确 Commit 导出 `code/`，在独立目录中编译。
Bootstrap、Host、Final 各自使用不存在的目录；构建并行度默认 28。
Host 目标生成 Tiling 库，Final 目标生成全部 Kernel 模板和 ACLNN 库。

```bash
source /usr/local/Ascend/cann-9.0.0/set_env.sh
python3 -m tests.st.scripts.build --target bootstrap --build-dir build/mock-bootstrap
python3 -m tests.st.scripts.build --target host --build-dir build/mock-host
python3 -m tests.st.scripts.build --target final --build-dir build/mock-final --jobs 28
python3 -m tests.st.scripts.profile_mock_workloads \
  --build build/mock-final --output build/tests/mock-performance
```

`--source-commit <完整 SHA>` 选择已存在于本地的源码 Commit。构建和测试入口均支持该参数；
测试入口要求构建记录中的 Commit 与所选 Commit 完全一致。
`--package-layout all` 导出全部源码文件，`legacy-seven` 导出七文件接口包。
两种方式均直接使用 Git 中的 CMake 文件。

`build-manifest.json` 记录 Commit、code 树、源码包范围、SDK、构建命令和实际退出码，
并绑定 Host 库、Kernel 对象、注册库、自动生成的接口头文件及 vendor 配置的 SHA256。
测试在设备运行前后核对这些产物。缺少最终算子库、构建失败、源码身份不符或产物变化均不能通过。

`st/cases/mock.json` 的 `runtime` 是十二例共用的执行配置，Case 不单独覆盖配置。
设备为 20 AIC / 40 AIV；采样要求 DeviceProf，Current/Rated Freq 均为 1650。
十二例的错误率显示均使用 `(1 - (total - bad) / total) * 100`，按十进制 half-up 保留两位小数。
默认使用 3 个独立进程，每个进程对每例调用 5 次；`msprof op` 使用 kernel 重放模式，每轮预热 5 次；省略指标选项以采用默认指标集。
计时读取 DeviceProf 的 Task Duration(us)，内存使用大页优先分配。
`--processes`、`--repeat` 的覆盖值同样应用于所有选中 Case；WarmUp 固定为 5。

计时来自完整设备 Kernel 的 `Task Duration(us)`。逐例报告全部原始样本、
中位数、均值、IQR、MAD、标准差及最小/最大值；`elapsed_us` 表示算术平均值，
对应官方逐例耗时的汇总口径，`median_us` 保留中位数。性能判断同时报告中位数和离散度。
输入生成、参考计算、编译、Host 初始化、内存分配和输入输出搬运不计入该指标。

每次调用前将输出预填为 NaN，调用后保存完整输出并按题面容差比较。
逐次结果保留错误元素数量、总元素数量、错误百分比和实际输出哈希。
C1/C2/C5/C6/C10/C11/C12 的 `error_ratio_mode` 为 `complement-half-up`：
错误百分比采用 `(1 - 正确元素数 / 总元素数) * 100`，以两位小数半入显示。
是否通过始终取决于原始错误元素数，百分比显示为零不能消除数值错误。
`Pass` 要求全部输出正确、采样完整且执行成功；
WA、RE、运行时错误码、后续未执行用例、超时和采样错误分别保留。
Profiler 根据实际 launch 顺序、Kernel 名和 Kernel 内编号关联样本。

```bash
python3 -m tests.st.scripts.profile_mock_workloads \
  --build build/mock-final --case mock_12 \
  --processes 3 --repeat 5 --warmup 5 \
  --output build/tests/mock-performance-tail
```

输出目录必须不存在。`results.json` 记录实际 SoC、共享测试配置、输入哈希、
构建身份、运行时 L2/全局内存容量、核数、虚拟化标志、资源查询状态、逐例统计和每个进程的状态；`process-*/` 保存命令、Profiler 原始日志及每次实际输出。
`--case` 可重复选择 Case，`--device`、`--toolkit`、`--timeout` 指定运行环境。
测试通过返回 0，测试失败返回 1，配置或构建错误返回 2。

测试套件位于 `tests/`；构建产物、输入数据、运行记录和独立验收记录置于
Git 忽略的 `build/` 或仓库外目录。

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
