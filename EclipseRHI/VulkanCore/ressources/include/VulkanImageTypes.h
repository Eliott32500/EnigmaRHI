#pragma once

#include <vulkan/vulkan.h>
#include "ImageTypes.h"

namespace EnigmaRHI::Utilities
{
	VkFormat ToVulkanFormat(EImageFormat format);
	VkImageType ToVulkanImageType(EImageType type);
	VkFilter ToVulkanFilter(EFilter mode);
	VkSamplerMipmapMode ToVulkanMipmapMode(EMipmapMode mode);
	VkSamplerAddressMode ToVulkanWrapMode(EWrappingMode mode);

	EImageFormat FormatFromVulkan(VkFormat format);
}