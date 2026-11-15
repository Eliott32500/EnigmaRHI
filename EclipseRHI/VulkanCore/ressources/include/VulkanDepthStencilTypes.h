#pragma once

#include <vulkan/vulkan.h>
#include "DepthStencilTypes.h"

namespace EnigmaRHI::Utilities
{
	VkCompareOp ToVulkanCompareOp(ECompareOp compareOp);
	VkStencilOp ToVulkanStencilOp(EStencilOp stencilOp);
	VkStencilFaceFlagBits ToVulkanStencilFace(EStencilFace face);
}