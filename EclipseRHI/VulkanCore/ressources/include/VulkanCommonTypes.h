#pragma once

#include <vulkan/vulkan.h>
#include <cinttypes>
#include "CommonTypes.h"

namespace EnigmaRHI::Utilities
{
	VkExtent2D ToVulkanExtent(Extent2D extent);
	VkExtent3D ToVulkanExtent(Extent3D extent);

	VkOffset2D ToVulkanOffset(Offset2D offset);
	VkOffset3D ToVulkanOffset(Offset3D offset);
}