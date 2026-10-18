#include "VulkanDepthStencilTypes.h"

VkCompareOp EnigmaRHI::Utilities::ToVulkanCompareOp(ECompareOp compareOp)
{
	switch (compareOp)
	{
	case ECompareOp::NEVER:
		return VK_COMPARE_OP_NEVER;

	case ECompareOp::LESS:
		return VK_COMPARE_OP_LESS;

	case ECompareOp::EQUAL:
		return VK_COMPARE_OP_EQUAL;

	case ECompareOp::LESS_OR_EQUAL:
		return VK_COMPARE_OP_LESS_OR_EQUAL;

	case ECompareOp::GREATER:
		return VK_COMPARE_OP_GREATER;

	case ECompareOp::NOT_EQUAL:
		return VK_COMPARE_OP_NOT_EQUAL;

	case ECompareOp::GREATER_OR_EQUAL:
		return VK_COMPARE_OP_GREATER_OR_EQUAL;

	case ECompareOp::ALWAYS:
		return VK_COMPARE_OP_ALWAYS;

	default:
		return VK_COMPARE_OP_ALWAYS;
	};
}
