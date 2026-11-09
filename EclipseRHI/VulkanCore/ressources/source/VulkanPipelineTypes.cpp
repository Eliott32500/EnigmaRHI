#include "VulkanPipelineTypes.h"
#include <algorithm>
#include "IDevice.h"
#include "RHIBitMask.h"
#include "VulkanDeviceCapabilities.h"
#include "VulkanDepthStencilTypes.h"

VkCullModeFlags EnigmaRHI::Utilities::ToVulkanCullMode(ECullMode mode)
{
	switch (mode)
	{
		case ECullMode::NONE:
			return VK_CULL_MODE_NONE;
		case ECullMode::FRONT:
			return VK_CULL_MODE_FRONT_BIT;
		case ECullMode::BACK:
			return VK_CULL_MODE_BACK_BIT;
		case ECullMode::FRONT_AND_BACK:
			return VK_CULL_MODE_FRONT_AND_BACK;
	}
}

VkPolygonMode EnigmaRHI::Utilities::ToVulkanPolygonMode(EPolygonMode mode)
{
	switch (mode)
	{
		case EPolygonMode::FILL:
			return VK_POLYGON_MODE_FILL;
		case EPolygonMode::LINE:
			return VK_POLYGON_MODE_LINE;
		case EPolygonMode::POINT:
			return VK_POLYGON_MODE_POINT;
	}
}

VkPrimitiveTopology EnigmaRHI::Utilities::ToVulkanTopology(EPrimitiveTopologyMode mode)
{
	switch (mode)
	{
	case EPrimitiveTopologyMode::POINT_LIST:
		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	case EPrimitiveTopologyMode::LINE_LIST:
		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case EPrimitiveTopologyMode::LINE_STRIP:
		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	case EPrimitiveTopologyMode::TRIANGLE_LIST:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	case EPrimitiveTopologyMode::TRIANGLE_STRIP:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	default:
		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}
}

VkFrontFace EnigmaRHI::Utilities::ToVulkanFrontFace(EFrontFaceMode mode)
{
	return mode == EFrontFaceMode::CLOCKWISE ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
}

VkBlendFactor EnigmaRHI::Utilities::ToVulkanBlendFactor(EBlendFactor factor)
{
	switch (factor)
	{
		case EBlendFactor::ZERO:
			return VK_BLEND_FACTOR_ZERO;
		case EBlendFactor::ONE:
			return VK_BLEND_FACTOR_ONE;
		case EBlendFactor::SRC_ALPHA:
			return VK_BLEND_FACTOR_SRC_ALPHA;
		case EBlendFactor::ONE_MINUS_SRC_ALPHA:
			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	}
}

VkBlendOp EnigmaRHI::Utilities::ToVulkanBlendOp(EBlendOp operation)
{
	switch (operation)
	{
		case EBlendOp::ADD:
			return VK_BLEND_OP_ADD;
		case EBlendOp::SUBTRACT:
			return VK_BLEND_OP_SUBTRACT;
		case EBlendOp::REVERSE_SUBTRACT:
			return VK_BLEND_OP_REVERSE_SUBTRACT;
		case EBlendOp::MIN:
			return VK_BLEND_OP_MIN;
		case EBlendOp::MAX:
			return VK_BLEND_OP_MAX;
	}
}

VkDynamicState EnigmaRHI::Utilities::ToVulkanDynamicState(EDynamicState dynamicState)
{
	switch (dynamicState)
	{
		case EDynamicState::VIEWPORT:
			return VK_DYNAMIC_STATE_VIEWPORT;

		case EDynamicState::SCISSOR:
			return VK_DYNAMIC_STATE_SCISSOR;

		case EDynamicState::LINE_WIDTH:
			return VK_DYNAMIC_STATE_LINE_WIDTH;

		case EDynamicState::DEPTH_BIAS:
			return VK_DYNAMIC_STATE_DEPTH_BIAS;

		case EDynamicState::BLEND_CONSTANTS:
			return VK_DYNAMIC_STATE_BLEND_CONSTANTS;

		case EDynamicState::DEPTH_BOUNDS:
			return VK_DYNAMIC_STATE_DEPTH_BOUNDS;

		case EDynamicState::STENCIL_COMPARE_MASK:
			return VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;

		case EDynamicState::STENCIL_WRITE_MASK:
			return VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;

		case EDynamicState::STENCIL_REFERENCE:
			return VK_DYNAMIC_STATE_STENCIL_REFERENCE;

		case EDynamicState::CULL_MODE:
			return VK_DYNAMIC_STATE_CULL_MODE;

		case EDynamicState::FRONT_FACE:
			return VK_DYNAMIC_STATE_FRONT_FACE;

		case EDynamicState::PRIMITIVE_TOPOLOGY:
			return VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY;

		case EDynamicState::DEPTH_TEST_ENABLE:
			return VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE;

		case EDynamicState::DEPTH_WRITE_ENABLE:
			return VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE;

		case EDynamicState::DEPTH_COMPARE_OP:
			return VK_DYNAMIC_STATE_DEPTH_COMPARE_OP;

		case EDynamicState::DEPTH_BOUNDS_TEST_ENABLE:
			return VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE;

		case EDynamicState::STENCIL_TEST_ENABLE:
			return VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE;

		case EDynamicState::STENCIL_OP:
			return VK_DYNAMIC_STATE_STENCIL_OP;

		case EDynamicState::RASTERIZER_DISCARD_ENABLE:
			return VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE;

		case EDynamicState::DEPTH_BIAS_ENABLE:
			return VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE;
	}
}

std::vector<VkDynamicState> EnigmaRHI::Utilities::ToVulkanDynamicStates(std::vector<EDynamicState> dynamicStates)
{
	std::vector<VkDynamicState> dS;

	for (int i = 0; i < dynamicStates.size(); i++)
		dS.push_back(ToVulkanDynamicState(dynamicStates[i]));

	return dS;
}

VkColorComponentFlags EnigmaRHI::Utilities::ToVulkanColorComponent(EColorMask colorComponents)
{
	VkColorComponentFlags result = 0;
	
	if (HasFlag(colorComponents, EColorMask::COLOR_R))	result |= VK_COLOR_COMPONENT_R_BIT;
	if (HasFlag(colorComponents, EColorMask::COLOR_G))	result |= VK_COLOR_COMPONENT_G_BIT;
	if (HasFlag(colorComponents, EColorMask::COLOR_B))	result |= VK_COLOR_COMPONENT_B_BIT;
	if (HasFlag(colorComponents, EColorMask::COLOR_A))	result |= VK_COLOR_COMPONENT_A_BIT;
	
	return result;
}

VkPipelineRasterizationStateCreateInfo EnigmaRHI::Utilities::ToVulkanRasterizationState(RasterizerPipelineState rasterization)
{
	DeviceLimits l = IDevice::GetCapabilities().limits;
	float lineLimits[2] = { l.lineWidthLimits[0] ,l.lineWidthLimits[1] };
	
	return VkPipelineRasterizationStateCreateInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,

		.polygonMode = ToVulkanPolygonMode(rasterization.polygonMode),
		.cullMode = ToVulkanCullMode(rasterization.cullMode),
		.frontFace = ToVulkanFrontFace(rasterization.frontFaceMode),

		.depthBiasEnable = rasterization.depthBiasEnable,
		.depthBiasConstantFactor = rasterization.depthBiasConstantFactor,
		.depthBiasClamp = rasterization.depthBiasClamp,
		.depthBiasSlopeFactor = rasterization.depthBiasSlopeFactor,

		.lineWidth = std::clamp(rasterization.lineWidth, lineLimits[0], lineLimits[1]),
	};
}

VkPipelineColorBlendAttachmentState EnigmaRHI::Utilities::ToVulkanColorBlendAttachment(ColorBlendAttachmentState colorBlendAttachment)
{
	return VkPipelineColorBlendAttachmentState
	{
		.blendEnable = colorBlendAttachment.blendEnable,
		.srcColorBlendFactor = Utilities::ToVulkanBlendFactor(colorBlendAttachment.srcColorBlendFactor),
		.dstColorBlendFactor = Utilities::ToVulkanBlendFactor(colorBlendAttachment.dstColorBlendFactor),
		.colorBlendOp = Utilities::ToVulkanBlendOp(colorBlendAttachment.colorBlendOp),
		.srcAlphaBlendFactor = Utilities::ToVulkanBlendFactor(colorBlendAttachment.srcAlphaBlendFactor),
		.dstAlphaBlendFactor = Utilities::ToVulkanBlendFactor(colorBlendAttachment.dstAlphaBlendFactor),
		.alphaBlendOp = Utilities::ToVulkanBlendOp(colorBlendAttachment.alphaBlendOp),
		.colorWriteMask = Utilities::ToVulkanColorComponent(colorBlendAttachment.colorMask)
	};
}

EnigmaRHI::Utilities::VulkanColorBlendState EnigmaRHI::Utilities::ToVulkanColorBlendState(
	const std::vector<ColorBlendAttachmentState>& colorBlendAttachmentList)
{
	EnigmaRHI::Utilities::VulkanColorBlendState result{};

	for (const auto& attachment : colorBlendAttachmentList)
		result.attachments.push_back(ToVulkanColorBlendAttachment(attachment));

	result.info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	result.info.logicOpEnable = VK_FALSE;
	result.info.logicOp = VK_LOGIC_OP_COPY;
	result.info.attachmentCount = static_cast<uint32_t>(result.attachments.size());
	result.info.pAttachments = result.attachments.data();
	result.info.blendConstants[0] = 0.f;
	result.info.blendConstants[1] = 0.f;
	result.info.blendConstants[2] = 0.f;
	result.info.blendConstants[3] = 0.f;

	return result;
}

VkPipelineMultisampleStateCreateInfo EnigmaRHI::Utilities::ToVulkanMultisampleState(MultisamplePipelineState multisampleState)
{
	return VkPipelineMultisampleStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = ToVulkanSampleCount(multisampleState.sampleCount),
		.sampleShadingEnable = multisampleState.sampleShadingEnable,
		.minSampleShading = multisampleState.minSampleShading,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = multisampleState.alphaToCoverageEnable,
		.alphaToOneEnable = multisampleState.alphaToOneEnable,
	};
}

VkPipelineDepthStencilStateCreateInfo EnigmaRHI::Utilities::ToVulkanDepthStencilPipelineState(DepthStencilPipelineState depthStencilState)
{
	return VkPipelineDepthStencilStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = depthStencilState.depthTestEnable,
		.depthWriteEnable = depthStencilState.depthWriteEnable,
		.depthCompareOp = Utilities::ToVulkanCompareOp(depthStencilState.depthCompareOp),
		.depthBoundsTestEnable = depthStencilState.depthBoundsTestEnable,
		.stencilTestEnable = depthStencilState.stencilTestEnable,
		.front = {}, // Optional
		.back = {}, // Optional
		.minDepthBounds = 0.0f, // Optional
		.maxDepthBounds = 1.0f, // Optional
	};
}
