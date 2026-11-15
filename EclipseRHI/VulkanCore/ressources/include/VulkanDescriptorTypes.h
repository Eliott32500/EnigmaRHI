#pragma once

#include "DescriptorTypes.h"
#include "vulkan/vulkan.h"

namespace EnigmaRHI::Utilities
{
	VkDescriptorType ToVkDescriptorType(EBindingType type);
}