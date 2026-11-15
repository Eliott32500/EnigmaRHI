#include "VulkanDescriptorTypes.h"
#include "RHILog.h"

VkDescriptorType EnigmaRHI::Utilities::ToVkDescriptorType(EBindingType type)
{
	switch (type)
	{
	case EBindingType::UNIFORM_BUFFER:				return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	case EBindingType::COMBINED_IMAGE_SAMPLER:		return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	case EBindingType::STORAGE_BUFFER:				return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	default:										LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan descriptor type !");
	}
}
