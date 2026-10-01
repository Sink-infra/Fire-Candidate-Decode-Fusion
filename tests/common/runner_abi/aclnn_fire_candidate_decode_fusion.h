// Expected generated operator signature for the declaration-only syntax check.
#pragma once
#include <aclnn/acl_meta.h>
extern "C" {
int aclnnFireCandidateDecodeFusionGetWorkspaceSize(const aclTensor *, const aclTensor *,
                                                  const aclTensor *, std::uint64_t *, aclOpExecutor **);
int aclnnFireCandidateDecodeFusion(void *, std::uint64_t, aclOpExecutor *, aclrtStream);
}
