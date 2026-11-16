#include "VulkanDepthStencilTypes.h"
#include "RHILog.h"

VkCompareOp EnigmaRHI::Utilities::ToVulkanCompareOp(ECompareOp compareOp)
{
	using E = ECompareOp;

	switch (compareOp)
	{
	case E::NEVER:					return VK_COMPARE_OP_NEVER;
	case E::LESS:					return VK_COMPARE_OP_LESS;
	case E::EQUAL:					return VK_COMPARE_OP_EQUAL;
	case E::LESS_OR_EQUAL:			return VK_COMPARE_OP_LESS_OR_EQUAL;
	case E::GREATER:				return VK_COMPARE_OP_GREATER;
	case E::NOT_EQUAL:				return VK_COMPARE_OP_NOT_EQUAL;
	case E::GREATER_OR_EQUAL:		return VK_COMPARE_OP_GREATER_OR_EQUAL;
	case E::ALWAYS:					return VK_COMPARE_OP_ALWAYS;
	};

	LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan compare operation !");
	return VK_COMPARE_OP_ALWAYS;
}

VkStencilFaceFlagBits EnigmaRHI::Utilities::ToVulkanStencilFace(EStencilFace face)
{
	switch (face)
	{
	case EStencilFace::FRONT:				return VK_STENCIL_FACE_FRONT_BIT;
	case EStencilFace::BACK:				return VK_STENCIL_FACE_BACK_BIT;
	case EStencilFace::FRONT_AND_BACK:		return VK_STENCIL_FACE_FRONT_AND_BACK;
	}

	LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan stencil face !");
	return VK_STENCIL_FACE_FRONT_AND_BACK;
}

VkStencilOp EnigmaRHI::Utilities::ToVulkanStencilOp(EStencilOp stencilOp)
{
	using E = EStencilOp;

	switch (stencilOp)
	{
	case E::KEEP:					return VK_STENCIL_OP_KEEP;
	case E::ZERO:					return VK_STENCIL_OP_ZERO;
	case E::REPLACE:				return VK_STENCIL_OP_REPLACE;
	case E::INCREMENT_AND_CLAMP:	return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
	case E::DECREMENT_AND_CLAMP:	return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
	case E::INVERT:					return VK_STENCIL_OP_INVERT;
	case E::INCREMENT_AND_WRAP:		return VK_STENCIL_OP_INCREMENT_AND_WRAP;
	case E::DECREMENT_AND_WRAP:		return VK_STENCIL_OP_DECREMENT_AND_WRAP;
	}

	LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan stencil operation !");
	return VK_STENCIL_OP_KEEP;
}
