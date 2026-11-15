#include "VulkanBindingLayout.h"
#include "VulkanDevice.h"
#include "VulkanShaderTypes.h"
#include "VulkanDescriptorTypes.h"
#include "RHILog.h"

void EnigmaRHI::VulkanBindingLayout::Create(IDevice* device, const BindingLayoutDesc& desc)
{
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	for (const auto& item : desc.items)
	{
		bindings.push_back(
			{
			.binding = item.slot,
			.descriptorType = Utilities::ToVkDescriptorType(item.type),
			.descriptorCount = item.count,
			.stageFlags = Utilities::ToVulkanShaderType(item.stages),
			}
		);
	}

	VkDescriptorSetLayoutCreateInfo info
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = static_cast<uint32_t>(bindings.size()),
		.pBindings = bindings.data(),
	};

	if (vkCreateDescriptorSetLayout(device->API_Vulkan().GetHandle(), &info, nullptr, &layout) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to create descriptor set layout !");
}

void EnigmaRHI::VulkanBindingLayout::Destroy(IDevice* device)
{
	if (layout == VK_NULL_HANDLE) 
		return;

	vkDestroyDescriptorSetLayout(device->API_Vulkan().GetHandle(), layout, nullptr);
	layout = VK_NULL_HANDLE;
}