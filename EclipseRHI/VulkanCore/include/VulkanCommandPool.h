#pragma once

#include "../../include/ICommandPool.h"
#include "VulkanCommandBuffer.h"

class VulkanCommandPool : public EclipseRHI::ICommandPool
{

public:

	VulkanCommandPool() = default;

	void Create(EclipseRHI::IDevice* device, EclipseRHI::ISurface* surface) override;

	std::vector<VulkanCommandBuffer> CreateCommandBuffer(VulkanDevice* device, uint32_t size);
	void CreateCommandPool(VulkanDevice* device, VkSurfaceKHR surface);

	VkCommandBuffer BeginSingleTimeCommands(VulkanDevice* device);
	void EndSingleTimeCommands(VulkanDevice* device, VkCommandBuffer commandBuffer);

	void Destroy(EclipseRHI::IDevice* device) override;

	VkCommandPool GetCommandPool() const { return commandPool; }
	EclipseRHI::ICommandBuffer* GetCommandBuffer(uint32_t frame) override { return &commandBuffers[frame]; }
	VulkanCommandBuffer& GetVulkanCommandBuffer(uint32_t frame) { return commandBuffers[frame]; }

	VulkanCommandPool& API_Vulkan() override { return (*this); }

private:

    VkCommandPool commandPool = VK_NULL_HANDLE;
	std::vector<VulkanCommandBuffer> commandBuffers;
};