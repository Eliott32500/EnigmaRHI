#pragma once
#include <cinttypes>

namespace EnigmaRHI
{
    struct DeviceLimits
    {
        float    maxSamplerAnisotropy = 1.0f;
        uint32_t maxImageDimension2D = 0;
        float    maxSamplerLodBias = 0.0f;
        uint64_t minUniformBufferOffsetAlignment = 256;
    };

    struct DeviceFeatures
    {
        bool samplerAnisotropy = true;
        bool dynamicRendering = true;
        bool synchronization2 = true;
        bool timelineSemaphore = true;
    };

    struct DeviceSpecifications
    {
        DeviceLimits   limits;
        DeviceFeatures features;
    };
}
