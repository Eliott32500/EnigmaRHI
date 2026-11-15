#include "VulkanBindingSet.h"
#include "VulkanBindingLayout.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "RHILog.h"

void EnigmaRHI::VulkanBindingSet::Create(IDevice* device, IBindingLayout* layout, const BindingSetDesc& desc)
{
	VulkanDevice& dev = device->API_Vulkan();
	VkDescriptorSetLayout vkLayout = layout->API_Vulkan().GetHandle();

	VkDescriptorSetAllocateInfo alloc
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = dev.GetDescriptorPool(),
		.descriptorSetCount = 1,
		.pSetLayouts = &vkLayout,
	};

	if (vkAllocateDescriptorSets(dev.GetHandle(), &alloc, &set) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to allocate descriptor set !");

	std::vector<VkDescriptorBufferInfo> bufferInfos(desc.items.size());
	std::vector<VkDescriptorImageInfo>  imageInfos(desc.items.size());
	std::vector<VkWriteDescriptorSet>   writes;

	for (size_t i = 0; i < desc.items.size(); i++)
	{
		const BindingSetItem& item = desc.items[i];

		VkWriteDescriptorSet write
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = set,
			.dstBinding = item.slot,
			.descriptorCount = 1,
		};

		if (item.type == EBindingType::COMBINED_IMAGE_SAMPLER)
		{
			VulkanImage& img = item.image->API_Vulkan();
			imageInfos[i] = { img.GetSampler(), img.GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
			write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.pImageInfo = &imageInfos[i];
		}
		else
		{
			VulkanBuffer& buf = item.buffer->API_Vulkan();
			bufferInfos[i] = { buf.GetHandle(), item.offset, item.range ? item.range : buf.GetSize() };
			write.descriptorType = (item.type == EBindingType::STORAGE_BUFFER)
				? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write.pBufferInfo = &bufferInfos[i];
		}

		writes.push_back(write);
	}

	vkUpdateDescriptorSets(dev.GetHandle(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
}

void EnigmaRHI::VulkanBindingSet::Destroy(IDevice* device)
{
	if (set == VK_NULL_HANDLE) 
		return;

	VulkanDevice& dev = device->API_Vulkan();
	vkFreeDescriptorSets(dev.GetHandle(), dev.GetDescriptorPool(), 1, &set);
	set = VK_NULL_HANDLE;
}