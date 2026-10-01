# FireCandidateDecodeFusion 测试

测试套件按类别分层，各类别维护独立用例目录，数据生成、输出比较和工具自检各司其职。
数学参考依据 [本题说明](../FireCandidateDecodeFusion.md) 实现。

当前套件包含 Coverage 和 Correctness。官方性能 Case 尚未公布，因此没有 Mock
目录、官方 Case 编号映射、Profiler 或性能计时协议。这里的形状用于输入契约和数值
覆盖，不代表官方性能形状。

Python/NumPy 只用于测试端生成输入、计算参考和比较输出；实际算子仍须在 Ascend C
NPU Kernel 中执行。当前算子源码仍为模板，工具自检通过不代表设备正确性或性能通过。

## 分类与工程约定

| 类别 | 数量 | 定义文件 | 职责 |
| --- | ---: | --- | --- |
| Coverage | 55（22 合法、33 契约拒绝） | `coverage/cases.py` | DType、形状、布局、取值和输入边界 |
| Correctness | 28（全部合法） | `correctness/cases.py` | 公式、精度、共享 priors、候选顺序与输出转换 |

两个类别各自定义 `CASES`，各自生成数据，不维护合并用例目录。生成器、参考实现、
比较器及 `unittest` 工具自检不另算算子 Case。

```text
tests/
├── __main__.py              # 分类数量、合法输入类型与规模统计
├── README.md
├── requirements.txt         # NumPy；自检使用 Python 标准库 unittest
├── coverage/
│   ├── cases.py             # Coverage 独立声明
│   ├── generate.py          # Coverage 生成入口
│   └── test_cases.py        # 契约、生成器与布局序列化自检
├── correctness/
│   ├── cases.py             # Correctness 独立声明
│   ├── generate.py          # Correctness 生成入口
│   └── test_cases.py        # 独立标量公式及错误输出比较自检
└── common/
    ├── case_spec.py         # 不可变用例描述、通道顺序和题面容差
    ├── artifacts.py         # manifest、元数据与全部输入/参考文件校验
    ├── reference.py         # 输入物化、契约校验及 float32 数学参考
    ├── generate_data.py     # 输入/参考二进制、case.json 和 manifest.json
    ├── verify_result.py     # 设备输出比较、误差定位与退出码
    ├── replay.py            # 独立的 Coverage/Correctness NPU 执行入口
    ├── runner.cpp           # ACLNN 两段式调用、NPU 内存与输出保存
    ├── runner_abi/          # 仅供自检的接口声明，无可执行实现
    ├── test_artifacts.py    # 输入及元数据改动的回归检查
    ├── test_replay.py       # 设备编排、失败报告与 C++ 语法自检
    └── checks.py            # 共用声明与生成产物自检
```

用例名称按类别加前缀，种子、模式和覆盖目标在声明中记录。所有文件采用小端存储，
按 Batch → Candidate → Channel 的 C 顺序展开。生成目录必须不存在，禁止覆盖
已有数据。生成器先在临时目录完成用例校验和文件写入，再发布数据，最后写入
`manifest.json`；校验、写入或发布失败会清理本次产物，可以直接在同一路径重试。
`case.json` 记录张量形状、类型、格式、物理步长、存储大小、种子、容差及
各二进制文件的 SHA256；`manifest.json` 记录类别、数量、NumPy 版本和元数据 SHA256。

## Coverage

合法用例覆盖所有 `B=1…8`，两种输入类型的 `N=1/31/32/33/63/64/65/32768`，
以及 `N=127/129/4096/16384/32767`。包含最大输入 `[8,32768,6]`、大尾块、
共享 priors，以及正的非整数 stride、小 anchor 和非负网格值。

契约拒绝用例覆盖错误 Rank、通道宽度、B/N 越界、N 不匹配、错误 DType、非 ND、
非连续布局、输入 NaN/±Inf、非正 stride/anchor 和负网格坐标。
它们标记的是**测试端输入契约校验**，错误名称是本地标签，不是 ACLNN 返回码。
题面没有规定非法输入的设备错误码，也没有要求 Kernel 扫描非法数值；这些用例不
将该行为设为设备验收要求。非法输入不生成数值参考。

非连续拒绝用例的二进制保留带填充的物理 backing，元数据记录逻辑 shape 与实际
strides，可按元数据恢复非连续视图，而不是把非连续输入偷偷压紧为连续数据。

## Correctness 与精度

28 个用例检查题面的三个数值示例及 `[B=3,N=33]` 尾块、六个输入通道和五个输出
通道、宽高平方与 anchor/stride 缩放、置信度乘积、负中心坐标、低分候选保留、
Sigmoid 饱和、微小 logit、float32 priors、float16 舍入、跨 Batch 的候选布局，
以及 `N=4097` 的大奇数布局。中心坐标接近零的用例能够暴露提前用 float16 计算的问题。

两个新增用例针对宽高与 priors 的计算精度，均使用 `[B=2,N=33]`：

- `fp16_width_underflow`：`tw=th=-12`、`anchor_w=anchor_h=1e8`、`stride=1`。
  正确宽高约为 `0.01510`；提前使用 float16 的 Sigmoid/平方会变成零，超出题面容差。
- `fp16_large_float32_priors`：`tw=th=-8`，使用相同的大 anchor。
  正确 float16 宽高为 `44.96875`；将 priors 降为 float16 会产生 Infinity。

这两个用例配套错误计算路径的回归自检，保持题面规定的容差。

参考流程为：先将输入量化到其声明类型，再将 raw_pred 提升为 float32，与 float32
priors 完成 Sigmoid、坐标解码、平方和乘法，最后一次性转换到输出类型。输出依次为
`[x,y,w,h,fire_score]`，形状 `[B,N,5]`。没有过滤、NMS、TopK 或排序。

| 输出类型 | 比较目标 | atol | rtol |
| --- | --- | ---: | ---: |
| float32 | float32 参考结果 | 1e-5 | 1e-5 |
| float16 | float32 参考结果最终转换到 float16 | 3e-3 | 3e-3 |

逐元素判断 `abs(actual-expected) <= atol + rtol*abs(expected)`。比较器默认使用题面
上限，允许通过参数收紧，不能放宽。比较前必须存在父目录的 `manifest.json`，
并检查 manifest 条目与数量、元数据 SHA256、输入/参考文件的字节数及 SHA256，
以及 shape、dtype 与物理布局的一致性。输入或元数据被改动后，旧输出会被拒绝。
数值比较同时检查非有限值
位置及符号和实际 `fire_score ∈ [0,1]`，报告第一个错误的 `[b,n,channel]`、
各通道错误数量和最大有限绝对误差。

题面没有给 priors 设置数值上界；有限输入仍可能在 float32 乘法或最终 float16
转换时溢出。两个合法诊断用例单独检查这两种情况，按对应参考中的 Infinity 位置
和符号比较。它们与禁止输入 NaN/±Inf 的契约拒绝用例分开。

## 使用

在仓库根目录执行，使用 Python 3.9 或更新版本。若环境尚未安装 NumPy：

```bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r tests/requirements.txt
```

查看用例规模并执行工具自检：

```bash
python3 -m tests --details
python3 -m unittest discover -s tests -t . -v
```

分别生成两个类别的输入与参考。下列输出目录必须不存在：

```bash
python3 -m tests.coverage.generate --output build/tests/data/coverage
python3 -m tests.correctness.generate --output build/tests/data/correctness
```

可只选择合法用例，或按完整用例名称选择一例；`--case` 支持重复传入：

```bash
python3 -m tests.coverage.generate --expectation success --output build/tests/data/coverage-valid
python3 -m tests.correctness.generate --case correctness_fp16_cancellation --output build/tests/data/cancellation
```

每个合法用例目录包含：

- `raw_pred.bin`：float16 或 float32 输入。
- `priors.bin`：float32 输入。
- `reference_decoded.bin`：按 decoded 类型存储的最终比较目标。
- `reference_decoded_fp32.bin`：最终转换前的 float32 结果，用于诊断。
- `case.json`：输入、输出、布局、比较规则及文件校验信息。

拒绝用例只包含输入和元数据。参考文件只生成一次；在实际 NPU 执行或未来计时循环中
复用输入，不将参考计算放入设备调用、预热或计时路径。

也可以比较其他设备工具保存的实际 decoded。将输出按输出类型、小端 C 顺序保存为
`/tmp/decoded.bin`，保留完整的输入套件目录及父目录 manifest，然后执行：

```bash
python3 -m tests.common.verify_result --case-dir build/tests/data/correctness/correctness_fp32_documented_zero --actual /tmp/decoded.bin
```

可选收紧容差：

```bash
python3 -m tests.common.verify_result --case-dir build/tests/data/correctness/correctness_fp32_documented_zero --actual /tmp/decoded.bin --atol 1e-6 --rtol 1e-6
```

比较通过返回退出码 `0`，数值不符返回 `1`，文件或参数错误返回 `2`。
工具自检会故意构造错序、漏写尾块、错误坐标/分数、非法分数范围、错误 Infinity、
截断文件、被改动的参考文件和 float16 中间计算，确认这些错误能够被发现。

## NPU 正确性执行

在安装 CANN、具有 NPU 的 Linux 环境初始化实际 CANN `set_env.sh`，并构建算子。
需要兼容 C++17 的编译器、`build/libcust_opapi.so`、
`build/autogen/aclnn_fire_candidate_decode_fusion.h` 和构建生成的 custom OPP 目录。
入口优先使用构建记录的 `ASCEND_CANN_PACKAGE_PATH`，也支持 `--toolkit` 显式指定 SDK。

```bash
cmake -S code -B build
cmake --build build -j
python3 -m tests.common.replay --build build --suite all --device 0 --output build/tests/device-run
```

该入口独立于 Mock，分别维护和生成 Coverage/Correctness 数据，每个合法用例只调用
一次实际 ACLNN 算子，不采集性能时间。参考计算在设备调用前完成；C++ runner 只负责
输入搬运、NPU 调用及输出保存。它先用 NaN 填充输出缓冲区，使遗漏写回能被发现。
非法输入作为本地契约用例记录为 `Skipped`，不要求设备扫描非法数值或返回指定错误码。

只运行某个类别或具体用例：

```bash
python3 -m tests.common.replay --build build --suite correctness --case correctness_fp16_width_underflow --device 0 --output build/tests/device-width-underflow
```

复用已生成的数据时，其目录下应包含所选类别目录与完整 manifest。所选用例缺失、
输入或元数据损坏都会在编译/执行前报错，避免旧数据遗漏新增用例：

```bash
python3 -m tests.common.replay --build build --suite all --data-root build/tests/data --device 0 --output build/tests/device-reuse
```

输出目录必须不存在。`results.json` 保存逐例 `Pass`、`Wrong Answer`、`Runtime Error`、
`Timeout`、`Data Error` 或 `Skipped`，记录输入 manifest、算子库、生成头文件、runner
源码及二进制的 SHA256。逐例目录保存执行输入、命令、原始日志和实际 `decoded.bin`；
执行前后都检查输入副本及原始套件，编译失败也保留失败报告和日志。
`--timeout` 设置每例超时秒数，默认 300。通过返回 `0`，用例失败返回 `1`，
环境、构建或参数错误返回 `2`。

工具自检中使用的过程替身和 `runner_abi` 声明只检查编排与 C++ 语法，不提供 CPU
算子实现，也不进入实际设备编译的 include 路径。工具自检不等同于 CANN SDK 编译
或 NPU 实测，设备验证需要运行上述入口并检查报告。

## 提交级复现

设备验证前记录当前完整提交号与工作区状态：

```bash
git status --porcelain
git rev-parse HEAD
```

按提交复现时使用完整 SHA，并记录实际加载的算子包 SHA256、设备/CANN 版本及输入
`manifest.json`。工作区有改动时同时保存补丁；只有提交号不能代表未提交的源码。

NumPy 是测试端唯一第三方依赖，使用 BSD-3-Clause 许可证。目录组织参考上述仓库，
算子数学参考和本题用例均按本题契约实现。
