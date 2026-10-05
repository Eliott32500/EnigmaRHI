#pragma once

#include "ICommandPool.h"
#include "VulkanCommandBuffer.h"

namespace EnigmaRHI
{
	class VulkanCommandPool : public ICommandPool
	{

	public:

		VulkanCommandPool() = default;

		void Create(IDevice* device, ISurface* surface) override;

		std::vector<VulkanCommandBuffer> CreateCommandBuffer(VulkanDevice* device, uint32_t size);
		void CreateCommandPool(VulkanDevice* device, VkSurfaceKHR surface);

		VkCommandBuffer BeginSingleTimeCommands(VulkanDevice* device);
		void EndSingleTimeCommands(VulkanDevice* device, VkCommandBuffer commandBuffer);

		void Destroy(IDevice* device) override;

		VkCommandPool GetCommandPool() const { return commandPool; }
		ICommandBuffer* GetCommandBuffer(uint32_t frame) override { return &commandBuffers[frame]; }
		VulkanCommandBuffer& GetVulkanCommandBuffer(uint32_t frame) { return commandBuffers[frame]; }

		VulkanCommandPool& API_Vulkan() override { return (*this); }

	private:

		VkCommandPool commandPool = VK_NULL_HANDLE;
		std::vector<VulkanCommandBuffer> commandBuffers;
	};
}