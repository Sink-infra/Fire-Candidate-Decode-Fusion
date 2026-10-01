#include "platform_context.h"
#include "acl/acl.h"
#include "platform/platform_info.h"
#include <map>
#include <string>

extern "C" std::uint32_t ConfigurePlatformLabel(
    std::uint32_t device, const char *physicalSoc, const char *canonicalLabel)
{
    if (physicalSoc == nullptr || canonicalLabel == nullptr) return 1;
    for (auto *manager : {&fe::PlatformInfoManager::Instance(), &fe::PlatformInfoManager::GeInstance()}) {
        auto status = manager->InitializePlatformInfo();
        if (status != 0) return status;
        fe::PlatFormInfos metadata;
        fe::OptionalInfos optional;
        status = manager->GetPlatformInfos(physicalSoc, metadata, optional);
        if (status != 0) return status;
        std::map<std::string, std::string> versions;
        if (!metadata.GetPlatformRes("version", versions)) return 1;
        versions["SoC_version"] = canonicalLabel;
        metadata.SetPlatformRes("version", versions);
        status = manager->UpdatePlatformInfos(physicalSoc, metadata);
        if (status != 0) return status;
        status = manager->InitRuntimePlatformInfos(physicalSoc);
        if (status != 0) return status;
        status = manager->UpdateRuntimePlatformInfosByDevice(device, metadata);
        if (status != 0) return status;
    }
    return 0;
}

extern "C" std::uint32_t ReadRuntimeDeviceResources(std::uint32_t device, std::int64_t *values)
{
    const aclrtDevAttr attributes[] = {ACL_DEV_ATTR_L2_CACHE_SIZE, ACL_DEV_ATTR_TOTAL_GLOBAL_MEM_SIZE,
        ACL_DEV_ATTR_CUBE_CORE_NUM, ACL_DEV_ATTR_VECTOR_CORE_NUM, ACL_DEV_ATTR_IS_VIRTUAL};
    std::uint32_t firstError = 0;
    for (std::uint32_t index = 0; index < 5; ++index) {
        values[index] = -1;
        const auto status = aclrtGetDeviceInfo(device, attributes[index], &values[index]);
        if (status != 0 && firstError == 0) firstError = status;
    }
    return firstError;
}
