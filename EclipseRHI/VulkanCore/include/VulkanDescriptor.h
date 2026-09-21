#pragma once

#include "../../include/IDescriptor.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"

class VulkanDescriptor : public EclipseRHI::IDescriptor
{
public:

	void AddImageBinding(uint32_t binding) override;
	void AddBufferBinding(uint32_t binding, EclipseRHI::ShaderStage stageFlags) override;

	void AddImageInfo(EclipseRHI::IImage* imageInfo) override;
	void AddBufferInfo(std::vector<VkDescriptorBufferInfo> bufferInfo) { bufferInfos.push_back(bufferInfo); }

	void Create(EclipseRHI::IDevice* device) override;
	void Destroy(EclipseRHI::IDevice* device) override;

	void CreateDescriptorPool(VulkanDevice* device);

	void CreateDescriptorSets(EclipseRHI::IDevice* device, std::vector<EclipseRHI::IDescriptor::FrameDescriptorInfo> bufferInfos) override;

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