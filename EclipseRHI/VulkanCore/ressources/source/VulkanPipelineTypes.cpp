#include "VulkanPipelineTypes.h"
#include <algorithm>
#include "IDevice.h"
#include "RHIBitMask.h"
#include "VulkanDeviceCapabilities.h"
#include "VulkanDepthStencilTypes.h"
#include "RHILog.h"

VkCullModeFlags EnigmaRHI::Utilities::ToVulkanCullMode(ECullMode mode)
{
	switch (mode)
	{
		using E = ECullMode;

		case E::NONE:				return VK_CULL_MODE_NONE;
		case E::FRONT:				return VK_CULL_MODE_FRONT_BIT;
		case E::BACK:				return VK_CULL_MODE_BACK_BIT;
		case E::FRONT_AND_BACK:     return VK_CULL_MODE_FRONT_AND_BACK;
		default:					LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan cull mode !");
	}
}

VkPolygonMode EnigmaRHI::Utilities::ToVulkanPolygonMode(EPolygonMode mode)
{
	switch (mode)
	{
		using E = EPolygonMode;

		case E::FILL:		return VK_POLYGON_MODE_FILL;
		case E::LINE:		return VK_POLYGON_MODE_LINE;
		case E::POINT:		return VK_POLYGON_MODE_POINT;
		default:			LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan polygon mode !");
	}
}

VkPrimitiveTopology EnigmaRHI::Utilities::ToVulkanTopology(EPrimitiveTopologyMode mode)
{
	switch (mode)
	{
		using E = EPrimitiveTopologyMode;

		case E::POINT_LIST:			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case E::LINE_LIST:			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case E::LINE_STRIP:			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case E::TRIANGLE_LIST:		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case E::TRIANGLE_STRIP:		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		default:					LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan topology !");
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
		using E = EBlendFactor;

		case E::ZERO:					return VK_BLEND_FACTOR_ZERO;
		case E::ONE:					return VK_BLEND_FACTOR_ONE;
		case E::SRC_ALPHA:				return VK_BLEND_FACTOR_SRC_ALPHA;
		case E::ONE_MINUS_SRC_ALPHA:	return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		default:						LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan blend factor !");
	}
}

VkBlendOp EnigmaRHI::Utilities::ToVulkanBlendOp(EBlendOp operation)
{
	switch (operation)
	{
		using E = EBlendOp;

		case E::ADD:					return VK_BLEND_OP_ADD;
		case E::SUBTRACT:				return VK_BLEND_OP_SUBTRACT;
		case E::REVERSE_SUBTRACT:		return VK_BLEND_OP_REVERSE_SUBTRACT;
		case E::MIN:					return VK_BLEND_OP_MIN;
		case E::MAX:					return VK_BLEND_OP_MAX;
		default:						LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan blend operation !");
	}
}

VkDynamicState EnigmaRHI::Utilities::ToVulkanDynamicState(EDynamicState dynamicState)
{
	switch (dynamicState)
	{
		using E = EDynamicState;

		case E::VIEWPORT:						return VK_DYNAMIC_STATE_VIEWPORT;
		case E::SCISSOR:						return VK_DYNAMIC_STATE_SCISSOR;
		case E::LINE_WIDTH:						return VK_DYNAMIC_STATE_LINE_WIDTH;
		case E::DEPTH_BIAS:						return VK_DYNAMIC_STATE_DEPTH_BIAS;
		case E::BLEND_CONSTANTS:				return VK_DYNAMIC_STATE_BLEND_CONSTANTS;
		case E::DEPTH_BOUNDS:					return VK_DYNAMIC_STATE_DEPTH_BOUNDS;
		case E::STENCIL_COMPARE_MASK:			return VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
		case E::STENCIL_WRITE_MASK:				return VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
		case E::STENCIL_REFERENCE:				return VK_DYNAMIC_STATE_STENCIL_REFERENCE;
		case E::CULL_MODE:						return VK_DYNAMIC_STATE_CULL_MODE;
		case E::FRONT_FACE:						return VK_DYNAMIC_STATE_FRONT_FACE;
		case E::PRIMITIVE_TOPOLOGY:				return VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY;
		case E::DEPTH_TEST_ENABLE:				return VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE;
		case E::DEPTH_WRITE_ENABLE:				return VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE;
		case E::DEPTH_COMPARE_OP:				return VK_DYNAMIC_STATE_DEPTH_COMPARE_OP;
		case E::DEPTH_BOUNDS_TEST_ENABLE:		return VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE;
		case E::STENCIL_TEST_ENABLE:			return VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE;
		case E::STENCIL_OP:						return VK_DYNAMIC_STATE_STENCIL_OP;
		case E::RASTERIZER_DISCARD_ENABLE:		return VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE;
		case E::DEPTH_BIAS_ENABLE:				return VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE;
		default:								LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan dynamic state !");
	}
}

VkVertexInputRate EnigmaRHI::Utilities::ToVulkanVertexInputRate(EVertexInputRate rate)
{
	switch (rate)
	{
		using E = EVertexInputRate;

		case E::VERTEX:			return VK_VERTEX_INPUT_RATE_VERTEX;
		case E::INSTANCE:		return VK_VERTEX_INPUT_RATE_INSTANCE;
		default:				LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan vertex input rate !");
	}
}

VkFormat EnigmaRHI::Utilities::ToVulkanVertexFormat(EVertexFormat format)
{
	switch (format)
	{
		using E = EVertexFormat;

		//Float
		case E::FLOAT:						return VK_FORMAT_R32_SFLOAT;
		case E::FLOAT2:						return VK_FORMAT_R32G32_SFLOAT;
		case E::FLOAT3:						return VK_FORMAT_R32G32B32_SFLOAT;
		case E::FLOAT4:						return VK_FORMAT_R32G32B32A32_SFLOAT;
		//Integer
		case E::INT:						return VK_FORMAT_R32_SINT;
		case E::INT2:						return VK_FORMAT_R32G32_SINT;
		case E::INT3:						return VK_FORMAT_R32G32B32_SINT;
		case E::INT4:						return VK_FORMAT_R32G32B32A32_SINT;
		// Unsigned Integer
		case E::UINT:						return VK_FORMAT_R32_UINT;
		case E::UINT2:						return VK_FORMAT_R32G32_UINT;
		case E::UINT3:						return VK_FORMAT_R32G32B32_UINT;
		case E::UINT4:						return VK_FORMAT_R32G32B32A32_UINT;
		// Signed Byte
		case E::BYTE4:						return VK_FORMAT_R8G8B8A8_SINT;
		// Unsigned Byte
		case E::UBYTE4:						return VK_FORMAT_R8G8B8A8_UINT;
		// Normalized Unsigned Byte
		case E::UBYTE4_NORM:				return VK_FORMAT_R8G8B8A8_UNORM;
		default:							LOG_FATAL(LogCategory::Vulkan, "Unsupported vulkan vertex format !");
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