#include "VulkanPipelineTypes.h"

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

VkFrontFace EnigmaRHI::Utilities::ToVulkanFrontFace(EFrontFaceMode mode)
{
	switch (mode)
	{
		case EFrontFaceMode::CLOCKWISE:
			return VK_FRONT_FACE_CLOCKWISE;
		case EFrontFaceMode::COUNTER_CLOCKWISE:
			return VK_FRONT_FACE_COUNTER_CLOCKWISE;
	}
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

VkCompareOp EnigmaRHI::Utilities::ToVulkanCompareOp(EDepthCompareOp operation)
{
	switch (operation)
	{
		case EDepthCompareOp::NEVER:
			return VK_COMPARE_OP_NEVER;
		case EDepthCompareOp::LESS:
			return VK_COMPARE_OP_LESS;
		case EDepthCompareOp::EQUAL:
			return VK_COMPARE_OP_EQUAL;
		case EDepthCompareOp::LESS_OR_EQUAL:
			return VK_COMPARE_OP_LESS_OR_EQUAL;
		case EDepthCompareOp::GREATER:
			return VK_COMPARE_OP_GREATER;
		case EDepthCompareOp::GREATER_OR_EQUAL:
			return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case EDepthCompareOp::NOT_EQUAL:
			return VK_COMPARE_OP_NOT_EQUAL;
		case EDepthCompareOp::ALWAYS:
			return VK_COMPARE_OP_ALWAYS;
	}
}
