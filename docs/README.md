# 项目文档

## 目录说明
关键目录结构如下：
```
├── api                            # API文档目录
│   ├── context                    # 存放每个API的说明文档
│   └── README.md                  # 存放Ascend C API列表
├── figures                        # 图片目录
├── asc_adv_api_contributing.md    # Ascend C 高阶API贡献指南
├── asc_basic_api_contributing.md  # Ascend C 基础API贡献指南
├── asc_c_api_contributing.md      # Ascend C C API贡献指南
├── quick_start.md                 # 快速入门文档
└── README.md
```

## 文档说明
为方便开发者快速熟悉本项目，可按需获取对应文档，文档内容包括：

| 文档  | 面向对象  | 内容介绍  |
|---|---|---|
| [API列表](./api/README.md) | 基于Ascend C开源仓，开发定制化API或算子的用户。 | 介绍项目包含的所有API。  |
| [高阶API贡献指南](./asc_adv_api_contributing.md) | 基于Ascend C开源仓，开发定制化API的用户。 | 介绍如何扩展或开发Ascend C高阶API。高阶API基于单核对常见算法进行抽象和封装，实现常用的计算算法，旨在提高算子开发效率。  |
| [基础API贡献指南](./asc_basic_api_contributing.md) | 基于Ascend C开源仓，开发定制化API的用户。 | 介绍如何扩展或开发Ascend C基础API。基础API实现对硬件能力的抽象，开放芯片的能力，保证完备性和兼容性。  |
| [C API贡献指南](./asc_c_api_contributing.md) | 基于Ascend C开源仓，开发定制化API的用户。 | 介绍如何扩展或开发Ascend C C API。C API提供纯C风格的接口，符合C语言算子开发习惯，开放芯片完备编程能力。  |
| [Ascend C编程指南](https://hiascend.com/document/redirect/CannCommunityOpdevAscendC) |基于昇腾AI硬件，使用Ascend C编写算子程序，开发自定义算子的开发者。 | Ascend C是CANN针对算子开发场景推出的编程语言，原生支持C和C++标准规范，兼具开发效率和运行性能。使用Ascend C编写算子程序，运行在昇腾AI处理器上，实现自定义的创新算法。 |
| [Ascend C最佳实践](https://hiascend.com/document/redirect/CannCommunityAscendCBestPractice) | 基于已完成开发的Ascend C算子，需要进一步优化算子性能的开发者。 | 异构计算的特点、算子功能的调试方法以及算子性能的优化策略。通过介绍Ascend C编程中的调试与优化思路，结合多种性能优化手段，并辅以具体案例，旨在帮助开发者实现高性能算子的开发。|


## 附录
除了上述系统性介绍的相关开发文档外，您还可以根据实际场景和开发阶段，有选择性地快速了解相应的专题内容。
- 技术文章  
  - 基础入门
    - [Ascend C编程介绍](https://www.hiascend.com/zh/developer/techArticles/20230830-1)
    - [Ascend C快速入门](https://www.hiascend.com/zh/developer/techArticles/20230830-2)
    - [Ascend C孪生调试](https://www.hiascend.com/zh/developer/techArticles/20231215-2)
    - [Ascend C算子的调用方式](https://www.hiascend.com/zh/developer/techArticles/20240523-1)
  - 概念原理
    - [Ascend C非对齐数据的处理方案](https://www.hiascend.com/zh/developer/techArticles/20250627-1)
    - [深度理解多核并行/流水计算/dobule buffer技术](https://www.hiascend.com/zh/developer/techArticles/20230807-1)
  - 问题案例
    - [Ascend C算子开发常见问题案例](https://www.hiascend.com/zh/developer/techArticles/20240106-1) 
    - [含有Matmul高阶API的算子精度问题定位](https://www.hiascend.com/zh/developer/techArticles/20250107-1)
  - 性能优化
    - [Ascend C算子性能优化实用技巧01——流水优化](https://www.hiascend.com/zh/developer/techArticles/20240819-1)
    - [Ascend C算子性能优化实用技巧02——内存优化](https://www.hiascend.com/zh/developer/techArticles/20240823-1)
    - [Ascend C算子性能优化实用技巧03——搬运优化](https://www.hiascend.com/zh/developer/techArticles/20240906-1)
    - [Ascend C算子性能优化实用技巧04——Tiling优化](https://www.hiascend.com/zh/developer/techArticles/20240920-1)
    - [Ascend C算子性能优化实用技巧05——API使用优化](https://www.hiascend.com/zh/developer/techArticles/20241107-1)
  - 优秀实践
    - [基于Ascend C的Matmul算子性能优化最佳实践](https://www.hiascend.com/zh/developer/techArticles/20240816-1)
    - [基于Ascend C的FlashAttention算子性能优化最佳实践](https://www.hiascend.com/zh/developer/techArticles/20240607-1)


- 培训视频
  - [Ascend C系列教程（入门）](https://www.hiascend.com/developer/courses/detail/1691696509765107713)
  - [Ascend C系列教程（进阶）](https://www.hiascend.com/zh/developer/courses/detail/1696414606799486977)
  - [Ascend C系列教程（高级）](https://www.hiascend.com/zh/developer/courses/detail/1696690858236694530)