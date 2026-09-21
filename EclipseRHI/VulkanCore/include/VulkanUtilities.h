#pragma once

#include "../../include/IFormat.h"
#include <vulkan/vulkan.h>

struct UtilitiesVulkan
{
	static VkFormat FormatToVulkan(EclipseRHI::ImageFormat format);
	static VkShaderStageFlags ShaderStageToVulkan(EclipseRHI::ShaderStage stage);
	static EclipseRHI::ImageFormat FormatFromVulkan(VkFormat format);
};
