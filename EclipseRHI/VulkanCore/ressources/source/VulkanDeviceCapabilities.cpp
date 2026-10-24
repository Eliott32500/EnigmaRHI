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

        return out;
    }
}