#pragma once

#include "ICommandPool.h"
#include "VulkanCommandBuffer.h"

namespace EnigmaRHI
{
	class VulkanCommandPool : public ICommandPool
	{

	public:

		VulkanCommandPool() = default;

		void Create(IDevice* device) override;
		void Destroy(IDevice* device) override;

		void Reset(IDevice* device) override;

		void CreateCommandPool(VulkanDevice* device);

		void AllocateCommandBuffers(VulkanDevice* device, uint32_t count);

		VkCommandBuffer BeginSingleTimeCommands(VulkanDevice* device, VkCommandBuffer commandBuffer);
		void EndSingleTimeCommands(VulkanDevice* device, VkCommandBuffer commandBuffer);


		VkCommandPool GetHandle() const { return commandPool; }
		ICommandBuffer* GetCommandBuffer(uint32_t index) override { return &commandBuffers.at(index); }
		uint32_t GetCommandBufferCount() const override { return static_cast<uint32_t>(commandBuffers.size()); }
		VulkanCommandBuffer& GetVulkanCommandBuffer(uint32_t index) { return commandBuffers.at(index); }

		VulkanCommandPool& API_Vulkan() override { return (*this); }

	private:

		VkCommandPool commandPool = VK_NULL_HANDLE;
		std::vector<VulkanCommandBuffer> commandBuffers;
	};
}