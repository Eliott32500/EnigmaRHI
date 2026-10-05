#pragma once

#include <vulkan/vulkan.h>
#include "RasterizationTypes.h"
#include "BlendTypes.h"
#include "DepthStencilTypes.h"

namespace EnigmaRHI::Utilities
{
    VkCullModeFlags ToVulkanCullMode(ECullMode mode);
    VkPolygonMode ToVulkanPolygonMode(EPolygonMode mode);
    VkFrontFace ToVulkanFrontFace(EFrontFaceMode mode);
    VkBlendFactor ToVulkanBlendFactor(EBlendFactor factor);
    VkBlendOp ToVulkanBlendOp(EBlendOp operation);
    VkCompareOp ToVulkanCompareOp(EDepthCompareOp operation);
}