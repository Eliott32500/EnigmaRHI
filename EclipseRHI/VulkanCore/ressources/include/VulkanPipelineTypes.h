#pragma once

#include <vulkan/vulkan.h>
#include "RasterizationTypes.h"
#include "BlendTypes.h"
#include "DepthStencilTypes.h"
#include "PipelineDynamicState.h"
#include "IGraphicsPipeline.h"
#include "RenderTypes.h"
#include <vector>

namespace EnigmaRHI::Utilities
{
    struct VulkanColorBlendState
    {
        std::vector<VkPipelineColorBlendAttachmentState> attachments;
        VkPipelineColorBlendStateCreateInfo info{};
    };

    VkCullModeFlags ToVulkanCullMode(ECullMode mode);
    VkPolygonMode ToVulkanPolygonMode(EPolygonMode mode);
    VkPrimitiveTopology ToVulkanTopology(EPrimitiveTopologyMode mode);
    VkFrontFace ToVulkanFrontFace(EFrontFaceMode mode);
    VkBlendFactor ToVulkanBlendFactor(EBlendFactor factor);
    VkBlendOp ToVulkanBlendOp(EBlendOp operation);
    VkDynamicState ToVulkanDynamicState(EDynamicState dynamicState);
    std::vector<VkDynamicState> ToVulkanDynamicStates(std::vector<EDynamicState> dynamicStates);
    VkColorComponentFlags ToVulkanColorComponent(EColorMask colorComponents);

    VkPipelineRasterizationStateCreateInfo ToVulkanRasterizationState(RasterizerPipelineState rasterization);
    VkPipelineColorBlendAttachmentState ToVulkanColorBlendAttachment(ColorBlendAttachmentState colorBlendAttachment);
    VulkanColorBlendState ToVulkanColorBlendState(const std::vector<ColorBlendAttachmentState>& colorBlendAttachmentList);
    VkPipelineMultisampleStateCreateInfo ToVulkanMultisampleState(MultisamplePipelineState multisampleState);
    VkPipelineDepthStencilStateCreateInfo ToVulkanDepthStencilPipelineState(DepthStencilPipelineState depthStencilState);
}