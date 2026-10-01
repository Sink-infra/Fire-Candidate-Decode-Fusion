// Declaration-only test contract for C++ syntax checks without a CANN SDK.
// These declarations have no implementation and cannot execute an operator.
#pragma once
#include <cstddef>
#include <cstdint>
using aclError = int;
using aclrtStream = void *;
enum aclDataType { ACL_FLOAT, ACL_FLOAT16 };
enum aclFormat { ACL_FORMAT_ND };
enum aclrtMemMallocPolicy { ACL_MEM_MALLOC_HUGE_FIRST };
enum aclrtMemcpyKind { ACL_MEMCPY_HOST_TO_DEVICE, ACL_MEMCPY_DEVICE_TO_HOST };
constexpr aclError ACL_SUCCESS = 0;
extern "C" {
aclError aclInit(const char *);
aclError aclFinalize();
aclError aclrtSetDevice(std::int32_t);
aclError aclrtResetDevice(std::int32_t);
aclError aclrtCreateStream(aclrtStream *);
aclError aclrtDestroyStream(aclrtStream);
aclError aclrtSynchronizeStream(aclrtStream);
aclError aclrtMalloc(void **, std::size_t, aclrtMemMallocPolicy);
aclError aclrtFree(void *);
aclError aclrtMemcpy(void *, std::size_t, const void *, std::size_t, aclrtMemcpyKind);
aclError aclrtMemset(void *, std::size_t, int, std::size_t);
}
