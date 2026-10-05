#pragma once

#include <vulkan/vulkan.h>
#include "ShaderTypes.h"

namespace EnigmaRHI::Utilities
{
	VkShaderStageFlags ToVulkanShaderType(EShaderType type);
}