#include "VulkanCommonTypes.h"

VkExtent2D EnigmaRHI::Utilities::ToVulkanExtent(Extent2D extent)
{
	return VkExtent2D(extent.width, extent.height);
}

VkExtent3D EnigmaRHI::Utilities::ToVulkanExtent(Extent3D extent)
{
	return VkExtent3D(extent.width, extent.height, extent.depth);
}

VkOffset2D EnigmaRHI::Utilities::ToVulkanOffset(Offset2D offset)
{
	return VkOffset2D(offset.x, offset.y);
}

VkOffset3D EnigmaRHI::Utilities::ToVulkanOffset(Offset3D offset)
{
	return VkOffset3D(offset.x, offset.y, offset.z);
}
