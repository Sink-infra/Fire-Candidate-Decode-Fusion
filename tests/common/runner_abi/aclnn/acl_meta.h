// Declaration-only unit-test contract; never passed to the device compiler.
#pragma once
#include <acl/acl.h>
struct aclTensor;
struct aclOpExecutor;
extern "C" {
aclTensor *aclCreateTensor(const std::int64_t *, std::uint64_t, aclDataType,
                           const std::int64_t *, std::int64_t, aclFormat,
                           const std::int64_t *, std::uint64_t, void *);
int aclDestroyTensor(const aclTensor *);
}
