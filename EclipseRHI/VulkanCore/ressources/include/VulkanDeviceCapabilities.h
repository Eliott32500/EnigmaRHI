#pragma once

#include "DeviceCapabilities.h"
#include <vulkan/vulkan.h>
#include "volk.h"

namespace EnigmaRHI::Utilities
{
    uint32_t HighestSampleCount(VkSampleCountFlags flags);
    VkSampleCountFlagBits ToVulkanSampleCount(uint32_t count);
    DeviceLimits ToDeviceLimits(const VkPhysicalDeviceLimits& l);
}