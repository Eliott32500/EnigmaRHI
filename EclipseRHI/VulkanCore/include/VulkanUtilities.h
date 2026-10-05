#pragma once

#include "ImageTypes.h"
#include "ShaderTypes.h"
#include <vulkan/vulkan.h>

namespace EnigmaRHI
{
	struct UtilitiesVulkan
	{
		static VkFormat FormatToVulkan(EImageFormat format);
		static VkShaderStageFlags ShaderStageToVulkan(EShaderType stage);
		static EImageFormat FormatFromVulkan(VkFormat format);
	};
}