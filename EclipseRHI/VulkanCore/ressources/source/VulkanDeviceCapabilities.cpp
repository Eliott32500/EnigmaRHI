#include "VulkanDeviceCapabilities.h"

namespace EnigmaRHI::Utilities
{
    uint32_t HighestSampleCount(VkSampleCountFlags flags)
    {
        for (uint32_t c = 64; c > 1; c >>= 1)
        {
            if (flags & c) 
                return c;
        }
        return 1;
    }

    VkSampleCountFlagBits ToVulkanSampleCount(uint32_t count)
    {
        switch (count)
        {
        case 1:     return VK_SAMPLE_COUNT_1_BIT;
        case 2:     return VK_SAMPLE_COUNT_2_BIT;
        case 4:     return VK_SAMPLE_COUNT_4_BIT;
        case 8:     return VK_SAMPLE_COUNT_8_BIT;
        case 16:    return VK_SAMPLE_COUNT_16_BIT;
        case 32:    return VK_SAMPLE_COUNT_32_BIT;
        case 64:    return VK_SAMPLE_COUNT_64_BIT;
        default:    return VK_SAMPLE_COUNT_1_BIT;
        }
    }

    DeviceLimits ToDeviceLimits(const VkPhysicalDeviceLimits& l)
    {
        return DeviceLimits
        {
            .maxImageDimension1D = l.maxImageDimension1D,
            .maxImageDimension2D = l.maxImageDimension2D,
            .maxImageDimension3D = l.maxImageDimension3D,
            .maxImageDimensionCube = l.maxImageDimensionCube,
            .maxImageArrayLayers = l.maxImageArrayLayers,

            .maxSamplerAnisotropy = l.maxSamplerAnisotropy,
            .maxSamplerLodBias = l.maxSamplerLodBias,
            .maxSamplerAllocationCount = l.maxSamplerAllocationCount,

            .minUniformBufferOffsetAlignment = l.minUniformBufferOffsetAlignment,
            .minStorageBufferOffsetAlignment = l.minStorageBufferOffsetAlignment,
            .optimalBufferCopyOffsetAlignment = l.optimalBufferCopyOffsetAlignment,
            .maxUniformBufferRange = l.maxUniformBufferRange,
            .maxPushConstantsSize = l.maxPushConstantsSize,

            .maxBoundDescriptorSets = l.maxBoundDescriptorSets,
            .maxPerStageDescriptorSamplers = l.maxPerStageDescriptorSamplers,

            .maxColorSampleCount = HighestSampleCount(l.framebufferColorSampleCounts),
            .maxDepthSampleCount = HighestSampleCount(l.framebufferDepthSampleCounts),

            .maxMemoryAllocationCount = l.maxMemoryAllocationCount,
            .maxColorAttachments = l.maxColorAttachments,

            .lineWidthLimits = {l.lineWidthRange[0], l.lineWidthRange[1]},
        };
    }
}