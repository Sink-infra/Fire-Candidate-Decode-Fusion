# 预留接口<a name="ZH-CN_TOPIC_0000001724165365"></a>

本章节列出的接口均为预留接口，后续有可能变更或废弃，不建议开发者使用，开发者无需关注。

## 内存管理和同步控制<a name="section89791646101413"></a>

-   TPipe
    -   GetAbsAddr

-   \_\_aicore\_\_ constexpr Hardware GetPhyType\(TPosition pos\);
-   template <typename T, TPosition pos\> \_\_aicore\_\_ inline bool PopStackBuffer\(LocalTensor<T\>& popLocal\);

## 矩阵计算<a name="section124371048171211"></a>

-   template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG\_ROW\_MAJOR\> void Fixpipe\(const LocalTensor<DstT\>& dstLocal, const LocalTensor<SrcT\>& srcLocal, const FixpipeParamsV220& intriParams\);
-   template <typename DstT, typename SrcT, const FixpipeConfig& config = CFG\_ROW\_MAJOR\> void Fixpipe\(const LocalTensor<DstT\>& dstLocal, const LocalTensor<SrcT\>& srcLocal, const LocalTensor<uint64\_t\>& cbufWorkspace, const FixpipeParamsV220& intriParams\);
-   template <typename T, typename U\> \_\_aicore\_\_ inline \_\_inout\_pipe\_\_\(V\) void BroadCastVecToMM\(const LocalTensor<T\> &dstLocal, const LocalTensor<U\> &srcLocal, const int32\_t blockCount, const uint8\_t blockLen, const uint8\_t srcGap, const uint8\_t dstGap\);

## 其他<a name="section67681482467"></a>

-   template <typename T\> \_\_aicore\_\_ inline \_\_in\_pipe\_\_\(V\) \_\_out\_pipe\_\_\(MTE3\) void InitOutput\(GlobalTensor<T\> gmWorkspaceAddr, uint32\_t size, T value = 0\);

