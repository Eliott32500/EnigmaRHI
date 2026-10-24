#pragma once
#include <cinttypes>

namespace EnigmaRHI
{
    struct DeviceLimits
    {
        // Images
        uint32_t maxImageDimension1D = 0;
        uint32_t maxImageDimension2D = 0;
        uint32_t maxImageDimension3D = 0;
        uint32_t maxImageDimensionCube = 0;
        uint32_t maxImageArrayLayers = 0;

        // Samplers
        float    maxSamplerAnisotropy = 1.0f;
        float    maxSamplerLodBias = 0.0f;
        uint32_t maxSamplerAllocationCount = 0;

        // Buffers / alignements
        uint64_t minUniformBufferOffsetAlignment = 256;
        uint64_t minStorageBufferOffsetAlignment = 256;
        uint64_t optimalBufferCopyOffsetAlignment = 1;
        uint32_t maxUniformBufferRange = 0;
        uint32_t maxPushConstantsSize = 0;

        // Descriptors
        uint32_t maxBoundDescriptorSets = 0;
        uint32_t maxPerStageDescriptorSamplers = 0;

        // MSAA
        uint32_t maxColorSampleCount = 1;
        uint32_t maxDepthSampleCount = 1;

        // Divers
        uint32_t maxMemoryAllocationCount = 0;
        uint32_t maxColorAttachments = 0;
    };

    struct DeviceFeatures
    {
        bool samplerAnisotropy = true;
        bool dynamicRendering = true;
        bool synchronization2 = true;
        bool timelineSemaphore = true;
    };

    inline DeviceFeatures GetRequiredFeatures()
    {
        return {
            .samplerAnisotropy = true,
            .dynamicRendering = true,
            .synchronization2 = true,
            .timelineSemaphore = true,
        };
    }

    struct DeviceSpecifications
    {
        DeviceLimits   limits;
        DeviceFeatures features;
    };
}
