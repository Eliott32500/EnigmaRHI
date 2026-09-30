#pragma once

#include "../../include/IDescriptor.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"

class VulkanDescriptor : public EnigmaRHI::IDescriptor
{
public:

	void AddImageBinding(uint32_t binding) override;
	void AddBufferBinding(uint32_t binding, EnigmaRHI::ShaderStage stageFlags) override;

	void AddImageInfo(EnigmaRHI::IImage* imageInfo) override;
	void AddBufferInfo(std::vector<VkDescriptorBufferInfo> bufferInfo) { bufferInfos.push_back(bufferInfo); }

	void Create(EnigmaRHI::IDevice* device) override;
	void Destroy(EnigmaRHI::IDevice* device) override;

	void CreateDescriptorPool(VulkanDevice* device);

	void CreateDescriptorSets(EnigmaRHI::IDevice* device, std::vector<EnigmaRHI::IDescriptor::FrameDescriptorInfo> bufferInfos) override;

	std::vector<VkDescriptorSet> GetDescriptorSets() { return descriptorSets; }
	VkDescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }
	VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }

	VulkanDescriptor& API_Vulkan() override { return (*this); }

private:

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkDescriptorSetLayoutBinding> bindings;

	std::vector<std::vector<VkDescriptorBufferInfo>> bufferInfos;
	std::vector<VkDescriptorImageInfo> imageInfos;
};