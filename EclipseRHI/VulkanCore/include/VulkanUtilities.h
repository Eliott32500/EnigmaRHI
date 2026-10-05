#pragma once

#include "IFormat.h"
#include <vulkan/vulkan.h>

struct UtilitiesVulkan
{
	static VkFormat FormatToVulkan(EnigmaRHI::ImageFormat format);
	static VkShaderStageFlags ShaderStageToVulkan(EnigmaRHI::ShaderStage stage);
	static EnigmaRHI::ImageFormat FormatFromVulkan(VkFormat format);
};
