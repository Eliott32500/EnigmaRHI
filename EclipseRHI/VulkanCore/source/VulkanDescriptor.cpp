#include "VulkanDescriptor.h"
#include "RHIConfig.h"

void VulkanDescriptor::AddImageBinding(uint32_t binding)
{
	VkDescriptorSetLayoutBinding descriptorBinding
	{
		.binding = binding,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
	};

	bindings.push_back(descriptorBinding);
}

void VulkanDescriptor::AddBufferBinding(uint32_t binding, EnigmaRHI::ShaderStage stageFlags)
{
	VkDescriptorSetLayoutBinding descriptorBinding
	{
		.binding = binding,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = UtilitiesVulkan::ShaderStageToVulkan(stageFlags),
	};

	bindings.push_back(descriptorBinding);
}

void VulkanDescriptor::AddImageInfo(EnigmaRHI::IImage* image)
{
	VkDescriptorImageInfo info
	{
		.sampler = image->API_Vulkan().GetSampler(),
		.imageView = image->API_Vulkan().GetImageView(),
		.imageLayout = image->API_Vulkan().GetImageLayout(),
	};

	imageInfos.push_back(info);
}

void VulkanDescriptor::Create(EnigmaRHI::IDevice* device)
{
	VkDescriptorSetLayoutCreateInfo layoutInfo
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = static_cast<uint32_t>(bindings.size()),
		.pBindings = bindings.data(),
	};

	if (vkCreateDescriptorSetLayout(device->API_Vulkan().GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor set layout!");

	CreateDescriptorPool(&device->API_Vulkan());
}

void VulkanDescriptor::Destroy(EnigmaRHI::IDevice* device)
{
	vkDestroyDescriptorPool(device->API_Vulkan().GetDevice(), descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(device->API_Vulkan().GetDevice(), descriptorSetLayout, nullptr);
}

void VulkanDescriptor::CreateDescriptorPool(VulkanDevice* device)
{
	std::vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.resize(bindings.size());

	for (int i = 0; i < bindings.size(); i++)
	{
		poolSizes[i].type = bindings[i].descriptorType;
		poolSizes[i].descriptorCount = static_cast<uint32_t>(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);
	}

	VkDescriptorPoolCreateInfo poolInfo
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = static_cast<uint32_t>(EnigmaRHI::MAX_FRAMES_IN_FLIGHT),
		.poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
		.pPoolSizes = poolSizes.data(),
	};

	if (vkCreateDescriptorPool(device->GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor pool!");
}

void VulkanDescriptor::CreateDescriptorSets(EnigmaRHI::IDevice* device, std::vector<EnigmaRHI::IDescriptor::FrameDescriptorInfo> bufferInfos)
{
	std::vector<VkDescriptorSetLayout> layouts(EnigmaRHI::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	 
	VkDescriptorSetAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = descriptorPool,
		.descriptorSetCount = static_cast<uint32_t>(EnigmaRHI::MAX_FRAMES_IN_FLIGHT),
		.pSetLayouts = layouts.data(),
	};
	
	descriptorSets.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);

	if (vkAllocateDescriptorSets(device->API_Vulkan().GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate descriptor sets!");
	
	for (size_t frame = 0; frame < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; frame++)
	{
		uint32_t bufferIndex = 0;
		uint32_t imageIndex = 0;

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		std::vector<VkDescriptorBufferInfo*> bufferInfo;

		for (auto& binding : bindings)
		{
			VkWriteDescriptorSet write
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = descriptorSets[frame],
				.dstBinding = binding.binding,
				.dstArrayElement = 0,
				.descriptorCount = binding.descriptorCount,
				.descriptorType = binding.descriptorType,
			};

			if (binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
			{
				VkDescriptorBufferInfo* buffer = new VkDescriptorBufferInfo();
				bufferInfo.push_back(buffer);

				bufferInfo[bufferIndex]->buffer = bufferInfos[frame].buffers[bufferIndex].buffer->API_Vulkan().GetBuffer();
				bufferInfo[bufferIndex]->offset = bufferInfos[frame].buffers[bufferIndex].offset;
				bufferInfo[bufferIndex]->range = bufferInfos[frame].buffers[bufferIndex].range;

				write.pBufferInfo = bufferInfo[bufferIndex];
				bufferIndex++;
			}

			else if (binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
				write.pImageInfo = &imageInfos[imageIndex];

			descriptorWrites.push_back(write);
		}
	
		vkUpdateDescriptorSets(device->API_Vulkan().GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		for (auto& buffer : bufferInfo)
			delete buffer;
	}
}