#pragma once

#include <vulkan/vulkan.h>
#include "RasterizationTypes.h"
#include "BlendTypes.h"
#include "DepthStencilTypes.h"
#include "PipelineDynamicState.h"
#include <vector>

namespace EnigmaRHI::Utilities
{
    VkCullModeFlags ToVulkanCullMode(ECullMode mode);
    VkPolygonMode ToVulkanPolygonMode(EPolygonMode mode);
    VkPrimitiveTopology ToVulkanTopology(EPrimitiveTopologyMode mode);
    VkFrontFace ToVulkanFrontFace(EFrontFaceMode mode);
    VkBlendFactor ToVulkanBlendFactor(EBlendFactor factor);
    VkBlendOp ToVulkanBlendOp(EBlendOp operation);
    VkDynamicState ToVulkanDynamicState(EDynamicState dynamicState);
    std::vector<VkDynamicState> ToVulkanDynamicStates(std::vector<EDynamicState> dynamicStates);
}