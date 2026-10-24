#pragma once

#include <vulkan/vulkan.h>
#include "ImageTypes.h"

namespace EnigmaRHI::Utilities
{
	VkFormat ToVulkanFormat(EImageFormat format);
	VkImageType ToVulkanImageType(EImageType type);
	VkImageViewType ToVulkanImageViewType(EImageViewType viewType);
	VkFilter ToVulkanFilter(EFilter mode);
	VkSamplerMipmapMode ToVulkanMipmapMode(EMipmapMode mode);
	VkSamplerAddressMode ToVulkanWrapMode(EWrappingMode mode);
	VkImageUsageFlags ToVulkanImageUsage(EImageUsage imageUsage);
	VkBorderColor ToVulkanBorderColor(EBorderColor borderColor);

	EImageFormat FormatFromVulkan(VkFormat format);

	uint32_t BytesPerPixel(EImageFormat format);

	bool HasFlag(EImageUsage value, EImageUsage flag);
}