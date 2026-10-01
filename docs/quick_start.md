# 快速入门

## 环境安装

### 使用显卡和Docker的场景

Docker安装环境以Atlas A2产品为例。

#### 前提条件

*   **Docker环境**：宿主机已安装Docker引擎（版本1.11.2及以上）。
*   **驱动与固件**：宿主机已安装昇腾NPU 24.1.0版本以上的[驱动与固件](https://www.hiascend.com/hardware/firmware-drivers/community?product=1&model=30&cann=8.0.RC3.alpha002&driver=1.0.26.alpha)Ascend HDK。安装指导详见《[CANN 软件安装指南](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850alpha002/softwareinst/instg/instg_0005.html?Mode=PmIns&OS=openEuler&Software=cannToolKit)》。
    
    > **注意**：使用`npu-smi info`查看对应的驱动与固件版本。

#### 下载镜像

拉取已预集成CANN软件包的镜像。

具体操作步骤如下：

1.  以root用户登录宿主机。
2.  执行拉取命令（请根据你的宿主机架构选择）：
    * ARM架构：
        ```bash
        docker pull --platform=arm64 swr.cn-south-1.myhuaweicloud.com/ascendhub/cann:8.5.0-910b-ubuntu22.04-py3.10-ops
        ```
    * X86架构：
        ```bash
        docker pull --platform=amd64 swr.cn-south-1.myhuaweicloud.com/ascendhub/cann:8.5.0-910b-ubuntu22.04-py3.10-ops
        ```
    > **注意**：正常网速下，镜像下载时间约为5-10分钟。

#### Docker运行

请根据以下命令运行docker：

```bash
docker run --name cann_container --device /dev/davinci0 --device /dev/davinci_manager --device /dev/devmm_svm --device /dev/hisi_hdc -v /usr/local/dcmi:/usr/local/dcmi -v /usr/local/bin/npu-smi:/usr/local/bin/npu-smi -v /usr/local/Ascend/driver/lib64/:/usr/local/Ascend/driver/lib64/ -v /usr/local/Ascend/driver/version.info:/usr/local/Ascend/driver/version.info -v /etc/ascend_install.info:/etc/ascend_install.info -it swr.cn-south-1.myhuaweicloud.com/ascendhub/cann:8.5.0-910b-ubuntu22.04-py3.10-ops bash
```
以下为用户需关注的参数说明：
| 参数 | 说明 | 注意事项 |
| :--- | :--- | :--- |
| `--name cann_container` | 指定容器名称，便于管理。 | 可自定义。 |
| `--device /dev/davinci0` | 核心：将宿主机的NPU设备卡映射到容器内，可指定映射多张NPU设备卡。 | 必须根据实际情况调整：`davinci0`对应系统中的第0张NPU卡。请先在宿主机执行 `npu-smi info`命令，根据输出显示的设备号（如`NPU 0`, `NPU 1`）来修改此编号。|
| `-v /usr/local/Ascend/driver/lib64/:/usr/local/Ascend/driver/lib64/` | 关键挂载：将宿主机的NPU驱动库映射到容器内。 | - |

#### 检查环境

进入容器后，验证环境和驱动是否正常。

-   **检查NPU设备**

    执行如下命令，若返回驱动相关信息说明已成功挂载。    
    ```bash    
    npu-smi info
    ```
-   **检查CANN包的安装**
    
    执行如下命令查看CANN Toolkit版本信息，是否为8.5.0版本。
    ```bash
    cat /usr/local/Ascend/cann/share/info/asc-devkit/version.info
    ```
至此，你已经拥有了一个“开箱即用”的开发环境。

### 其他场景

#### 前提条件

1. **安装依赖**

   以下所列仅为本开源仓源码编译用到的依赖，其中python、gcc、cmake的安装方法请参见配套版本的[用户手册](https://hiascend.com/document/redirect/CannCommunityInstDepend)，选择安装场景后，参见“安装CANN > 安装依赖”章节进行相关依赖的安装。

   - python >= 3.9.0

   - gcc >= 7.3.0

   - cmake >= 3.16.0

   - patch

     Ubuntu系统执行以下命令安装：
     ```
     sudo apt install patch
     ```
     EulerOS操作系统执行以下命令安装：
     ```
     sudo yum install patch
     ```

2. **安装驱动与固件（运行态依赖）**

   运行算子时必须安装驱动与固件，若仅编译算子，可跳过本操作，安装指导详见《[CANN 软件安装指南](https://www.hiascend.com/document/redirect/CannCommunityInstSoftware)》。

#### 环境准备<a name="prepare&install"></a>

本项目支持由源码编译，进行源码编译前，请根据如下步骤完成相关环境准备。

1. **安装社区CANN toolkit包**

    根据实际环境，下载对应`Ascend-cann-toolkit_${cann_version}_linux-${arch}.run`包，下载链接为[toolkit x86_64包](https://mirror-centralrepo.devcloud.cn-north-4.huaweicloud.com/artifactory/cann-run-release/software/master/20260211182015/x86_64/Ascend-cann-toolkit_9.0.0_linux-x86_64.run)、[toolkit aarch64包](https://mirror-centralrepo.devcloud.cn-north-4.huaweicloud.com/artifactory/cann-run-release/software/master/20260211182015/aarch64/Ascend-cann-toolkit_9.0.0_linux-aarch64.run)。

    ```bash
    # 确保安装包具有可执行权限
    chmod +x Ascend-cann-toolkit_${cann_version}_linux-${arch}.run
    # 安装命令
    ./Ascend-cann-toolkit_${cann_version}_linux-${arch}.run --full --force --install-path=${install_path}
    ```
    - \$\{cann\_version\}：表示CANN包版本号。
    - \$\{arch\}：表示CPU架构，如aarch64、x86_64。
    - \$\{install\_path\}：表示指定安装路径。
    - 缺省--install-path时， 则使用默认路径安装。
    若使用root用户安装，安装完成后相关软件存储在“/usr/local/Ascend/cann”路径下；若使用非root用户安装，安装完成后相关软件存储在“$HOME/Ascend/cann”路径下。

2. **安装社区版CANN ops包（运行态依赖）**

    运行算子前必须安装本包，若仅编译算子，可跳过本操作。

    根据产品型号和环境架构，下载对应`Ascend-cann-${soc_name}-ops_9.0.0_linux-${arch}.run`包，下载链接如下：

    - Atlas A2 训练系列产品/Atlas A2 推理系列产品：[ops x86_64包](https://mirror-centralrepo.devcloud.cn-north-4.huaweicloud.com/artifactory/cann-run-release/software/master/20260211182015/x86_64/Ascend-cann-910b-ops_9.0.0_linux-x86_64.run)、[ops aarch64包](https://mirror-centralrepo.devcloud.cn-north-4.huaweicloud.com/artifactory/cann-run-release/software/master/20260211182015/aarch64/Ascend-cann-910b-ops_9.0.0_linux-aarch64.run)。
    - Atlas A3 训练系列产品/Atlas A3 推理系列产品：[ops x86_64包](https://mirror-centralrepo.devcloud.cn-north-4.huaweicloud.com/artifactory/cann-run-release/software/master/20260211182015/x86_64/Ascend-cann-A3-ops_9.0.0_linux-x86_64.run)、[ops aarch64包](https://mirror-centralrepo.devcloud.cn-north-4.huaweicloud.com/artifactory/cann-run-release/software/master/20260211182015/aarch64/Ascend-cann-A3-ops_9.0.0_linux-aarch64.run)。

    ```bash
    # 确保安装包具有可执行权限
    chmod +x Ascend-cann-${soc_name}-ops_9.0.0_linux-${arch}.run
    # 安装命令
    ./Ascend-cann-${soc_name}-ops_9.0.0_linux-${arch}.run --install --install-path=${install_path}
    ```
    - \$\{soc\_name\}：表示NPU型号名称。
    - \$\{install\_path\}：表示指定安装路径，需要与toolkit包安装在相同路径，root用户默认安装在`/usr/local/Ascend`目录，非root用户默认安装在`$HOME/Ascend`目录。

3. **配置环境变量**

- 默认路径，root用户安装

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

- 默认路径，非root用户安装
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

- 指定路径安装
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

4. **下载源码**

    开发者可通过如下命令下载本仓源码：
    ```bash
    # 下载项目源码，以master分支为例
    git clone https://gitcode.com/cann/asc-devkit.git
    ``` 


## 编译安装<a name="compile&install"></a>

1. 编译

   本开源仓提供一键式编译安装能力。

   方式一：进入本开源仓代码根目录，执行如下命令：

   ```bash
   bash build.sh --pkg
   ```

   方式二：用户也可使用离线下载功能，手动下载[makeself源码包](https://gitcode.com/cann-src-third-party/makeself/releases/download/release-2.5.0-patch1.0/makeself-release-2.5.0-patch1.tar.gz)存放至自定义目录PATH_TO_DOWNLOAD，进入本开源仓代码根目录，执行如下命令：

   ```bash
   bash build.sh --pkg --cann_3rd_lib_path={PATH_TO_DOWNLOAD} # PATH_TO_DOWNLOAD为自定义下载目录
   ```

   编译完成后会在`build_out`目录下生成cann-asc-devkit_*<cann_version>*_linux-*\<arch\>*.run软件包。
2. 安装

   在开源仓根目录下执行下列命令，将编译生成的run包安装到默认路径（/usr/local/Ascend）；或安装到指定的CANN包的装包路径（${install_path}），同时会覆盖原CANN包中的Ascend C内容。

   ```bash
   # 切换到run包生成路径下
   cd build_out
   # 默认路径安装run包
   ./cann-asc-devkit_<cann_version>_linux-<arch>.run --full
   # 指定路径安装run包
   ./cann-asc-devkit_<cann_version>_linux-<arch>.run --full --install-path=${install_path}
   ```

## UT验证



### 安装依赖
- pytest >= 8.0.0

    执行以下命令安装：
    ```bash
    pip3 install pytest
    ```

- coverage >= 4.5.4

    执行以下命令安装：
    ```bash
    pip3 install coverage
    ```

- lcov >= 1.16(仅在执行覆盖率统计场景需要)

    下载[lcov源码](https://gitcode.com/cann-src-third-party/lcov/releases/download/v1.16/lcov-1.16.tar.gz)后，执行以下命令安装：
    ```bash
    tar -xf lcov-1.16.tar.gz
    cd lcov-1.16
    make install                         # root用户安装
    # sudo make install                  # 非root用户安装
    ```

### 执行
方式一：在开源仓根目录执行下列命令，将按各模块依次批跑tests目录下的用例，得到结果日志，用于看护编译是否正常。

```bash
bash build.sh --adv_test                         # 批跑tests目录下adv_api里的用例
bash build.sh --basic_test_one                   # 批跑tests目录下basic_api part-one里的用例
bash build.sh --basic_test_two                   # 批跑tests目录下basic_api part-two里的用例
bash build.sh --basic_test_three                 # 批跑tests目录下basic_api part-three里的用例
```

方式二：用户也可使用离线下载功能，手动下载[三方库源码包](#开源第三方软件依赖)存放至自定义目录PATH_TO_DOWNLOAD，在开源仓根目录执行下列命令，同时批跑执行各模块的用例。

```bash
# 以PATH_TO_DOWNLOAD为自定义下载目录为例
bash build.sh --adv_test --cann_3rd_lib_path={PATH_TO_DOWNLOAD}          # 批跑tests目录下adv_api里的用例
bash build.sh --basic_test_one --cann_3rd_lib_path={PATH_TO_DOWNLOAD}    # 批跑tests目录下basic_api part-one里的用例
bash build.sh --basic_test_two --cann_3rd_lib_path={PATH_TO_DOWNLOAD}    # 批跑tests目录下basic_api part-two里的用例
bash build.sh --basic_test_three --cann_3rd_lib_path={PATH_TO_DOWNLOAD}  # 批跑tests目录下basic_api part-three里的用例
```

### 开源第三方软件依赖

在执行ut时，依赖的第三方开源软件列表如下：

| 开源软件 | 版本 | 下载地址 |
|---|---|---|
| googletest | 1.14.0 | [googletest-1.14.0.tar.gz](https://gitcode.com/cann-src-third-party/googletest/releases/download/v1.14.0/googletest-1.14.0.tar.gz) |
| boost | 1.87.0 | [boost_1_87_0.tar.gz](https://gitcode.com/cann-src-third-party/boost/releases/download/v1.87.0/boost_1_87_0.tar.gz) |
| mockcpp | 2.7 | [makeself-release-2.5.0-patch1.tar.gz](https://gitcode.com/cann-src-third-party/mockcpp/releases/download/v2.7-h3/mockcpp-2.7.tar.gz) |
| mockcpp_patch | 2.7 | [mockcpp-2.7_py3-h3.patch](https://gitcode.com/cann-src-third-party/mockcpp/releases/download/v2.7-h3/mockcpp-2.7_py3-h3.patch) |