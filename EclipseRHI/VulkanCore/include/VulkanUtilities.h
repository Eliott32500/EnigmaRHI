#pragma once

#include "IFormat.h"
#include <vulkan/vulkan.h>

namespace EnigmaRHI
{
	struct UtilitiesVulkan
	{
		static VkFormat FormatToVulkan(ImageFormat format);
		static VkShaderStageFlags ShaderStageToVulkan(ShaderStage stage);
		static ImageFormat FormatFromVulkan(VkFormat format);
	};
}