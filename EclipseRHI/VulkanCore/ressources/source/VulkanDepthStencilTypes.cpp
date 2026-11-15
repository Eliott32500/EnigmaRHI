#include "VulkanDepthStencilTypes.h"
#include "RHILog.h"

VkCompareOp EnigmaRHI::Utilities::ToVulkanCompareOp(ECompareOp compareOp)
{
	switch (compareOp)
	{
		using E = ECompareOp;

	case E::NEVER:					return VK_COMPARE_OP_NEVER;
	case E::LESS:					return VK_COMPARE_OP_LESS;
	case E::EQUAL:					return VK_COMPARE_OP_EQUAL;
	case E::LESS_OR_EQUAL:			return VK_COMPARE_OP_LESS_OR_EQUAL;
	case E::GREATER:				return VK_COMPARE_OP_GREATER;
	case E::NOT_EQUAL:				return VK_COMPARE_OP_NOT_EQUAL;
	case E::GREATER_OR_EQUAL:		return VK_COMPARE_OP_GREATER_OR_EQUAL;
	case E::ALWAYS:					return VK_COMPARE_OP_ALWAYS;
	default:						LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan compare operation !");
	};
}

VkStencilFaceFlagBits EnigmaRHI::Utilities::ToVulkanStencilFace(EStencilFace face)
{
	switch (face)
	{
	case EStencilFace::FRONT:				return VK_STENCIL_FACE_FRONT_BIT;
	case EStencilFace::BACK:				return VK_STENCIL_FACE_BACK_BIT;
	case EStencilFace::FRONT_AND_BACK:		return VK_STENCIL_FACE_FRONT_AND_BACK;
	}
}

VkStencilOp EnigmaRHI::Utilities::ToVulkanStencilOp(EStencilOp stencilOp)
{
	switch (stencilOp)
	{
		using E = EStencilOp;

	case E::KEEP:					return VK_STENCIL_OP_KEEP;
	case E::ZERO:					return VK_STENCIL_OP_ZERO;
	case E::REPLACE:				return VK_STENCIL_OP_REPLACE;
	case E::INCREMENT_AND_CLAMP:	return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
	case E::DECREMENT_AND_CLAMP:	return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
	case E::INVERT:					return VK_STENCIL_OP_INVERT;
	case E::INCREMENT_AND_WRAP:		return VK_STENCIL_OP_INCREMENT_AND_WRAP;
	case E::DECREMENT_AND_WRAP:		return VK_STENCIL_OP_DECREMENT_AND_WRAP;
	default:						LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan stencil operation !");
	}
}
