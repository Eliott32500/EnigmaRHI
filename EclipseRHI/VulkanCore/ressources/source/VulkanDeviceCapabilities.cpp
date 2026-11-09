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
            case 1:  
                return VK_SAMPLE_COUNT_1_BIT;
            case 2:  
                return VK_SAMPLE_COUNT_2_BIT;
            case 4:  
                return VK_SAMPLE_COUNT_4_BIT;
            case 8:  
                return VK_SAMPLE_COUNT_8_BIT;
            case 16: 
                return VK_SAMPLE_COUNT_16_BIT;
            case 32: 
                return VK_SAMPLE_COUNT_32_BIT;
            case 64: 
                return VK_SAMPLE_COUNT_64_BIT;
            default: 
                return VK_SAMPLE_COUNT_1_BIT;
        }
    }

    DeviceLimits ToDeviceLimits(const VkPhysicalDeviceLimits& l)
    {
        DeviceLimits out;

        out.maxImageDimension1D = l.maxImageDimension1D;
        out.maxImageDimension2D = l.maxImageDimension2D;
        out.maxImageDimension3D = l.maxImageDimension3D;
        out.maxImageDimensionCube = l.maxImageDimensionCube;
        out.maxImageArrayLayers = l.maxImageArrayLayers;

        out.maxSamplerAnisotropy = l.maxSamplerAnisotropy;
        out.maxSamplerLodBias = l.maxSamplerLodBias;
        out.maxSamplerAllocationCount = l.maxSamplerAllocationCount;

        out.minUniformBufferOffsetAlignment = l.minUniformBufferOffsetAlignment;
        out.minStorageBufferOffsetAlignment = l.minStorageBufferOffsetAlignment;
        out.optimalBufferCopyOffsetAlignment = l.optimalBufferCopyOffsetAlignment;
        out.maxUniformBufferRange = l.maxUniformBufferRange;
        out.maxPushConstantsSize = l.maxPushConstantsSize;

        out.maxBoundDescriptorSets = l.maxBoundDescriptorSets;
        out.maxPerStageDescriptorSamplers = l.maxPerStageDescriptorSamplers;

        out.maxColorSampleCount = HighestSampleCount(l.framebufferColorSampleCounts);
        out.maxDepthSampleCount = HighestSampleCount(l.framebufferDepthSampleCounts);

        out.maxMemoryAllocationCount = l.maxMemoryAllocationCount;
        out.maxColorAttachments = l.maxColorAttachments;

        out.lineWidthLimits[0] = l.lineWidthRange[0];
        out.lineWidthLimits[1] = l.lineWidthRange[1];

        return out;
    }
}