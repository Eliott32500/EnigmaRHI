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

	bool HasStencilComponent(EImageFormat format);
	bool HasDepthComponent(EImageFormat format);

	VkAttachmentLoadOp ToVulkanLoadOp(ELoadOp loadOp);
	VkAttachmentStoreOp ToVulkanStoreOp(EStoreOp storeOp);
	VkImageLayout ToVulkanImageLayout(EImageLayout imageLayout);

	VkAccessFlags ToVulkanAccessFlag(EAccessFlag accessFlag);
	VkPipelineStageFlagBits2 ToVulkanPipelineFlag(EPipelineStageFlag pipelineStateFlag);
}