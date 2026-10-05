#pragma once

#include "IDescriptor.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"

namespace EnigmaRHI
{
	class VulkanDescriptor : public IDescriptor
	{
	public:

		void AddImageBinding(uint32_t binding) override;
		void AddBufferBinding(uint32_t binding, ShaderStage stageFlags) override;

		void AddImageInfo(IImage* imageInfo) override;
		void AddBufferInfo(std::vector<VkDescriptorBufferInfo> bufferInfo) { bufferInfos.push_back(bufferInfo); }

		void Create(IDevice* device) override;
		void Destroy(IDevice* device) override;

		void CreateDescriptorPool(VulkanDevice* device);

		void CreateDescriptorSets(IDevice* device, std::vector<IDescriptor::FrameDescriptorInfo> bufferInfos) override;

		std::vector<VkDescriptorSet> GetDescriptorSets() { return descriptorSets; }
		VkDescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }
		VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }

		EnigmaRHI::VulkanDescriptor& API_Vulkan() override { return (*this); }

	private:

		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;

		std::vector<VkDescriptorSetLayoutBinding> bindings;

		std::vector<std::vector<VkDescriptorBufferInfo>> bufferInfos;
		std::vector<VkDescriptorImageInfo> imageInfos;
	};
}