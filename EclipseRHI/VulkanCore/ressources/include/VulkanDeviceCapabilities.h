#pragma once

#include "DeviceCapabilities.h"
#include <vulkan/vulkan.h>
#include "volk.h"

namespace EnigmaRHI::Utilities
{
    uint32_t HighestSampleCount(VkSampleCountFlags flags);
    DeviceLimits ToDeviceLimits(const VkPhysicalDeviceLimits& l);
}