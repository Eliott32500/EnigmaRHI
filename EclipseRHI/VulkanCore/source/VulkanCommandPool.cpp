#include "../include/VulkanCommandPool.h"

void VulkanCommandPool::Create(EnigmaRHI::IDevice* device, EnigmaRHI::ISurface* surface)
{
	CreateCommandPool(&device->API_Vulkan(), surface->API_Vulkan().GetSurface());
	commandBuffers = CreateCommandBuffer(&device->API_Vulkan(), device->MAX_FRAMES_IN_FLIGHT);
}

std::vector<VulkanCommandBuffer> VulkanCommandPool::CreateCommandBuffer(VulkanDevice* device, uint32_t size)
{
	std::vector<VulkanCommandBuffer> commandBuffers(size);
	
	for(uint32_t i = 0; i < size; i++)
	{
		commandBuffers[i].Create(device, this);
	}
	
	return commandBuffers;
}

void VulkanCommandPool::CreateCommandPool(VulkanDevice* device, VkSurfaceKHR surface)
{
	QueueFamilyIndices queueFamilyIndices = device->FindQueueFamilies(device->GetPhysicalDevice(), surface);
	
	VkCommandPoolCreateInfo poolInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
	};
	
	if (vkCreateCommandPool(device->GetDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool!");
}

VkCommandBuffer VulkanCommandPool::BeginSingleTimeCommands(VulkanDevice* device)
{
	VkCommandBufferAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	
	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device->GetDevice(), &allocInfo, &commandBuffer);
	
	VkCommandBufferBeginInfo beginInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	
	return commandBuffer;
}

void VulkanCommandPool::EndSingleTimeCommands(VulkanDevice* device, VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);
	
	VkSubmitInfo submitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &commandBuffer,
	};
	
	vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(device->GetGraphicsQueue());
	
	vkFreeCommandBuffers(device->GetDevice(), commandPool, 1, &commandBuffer);
}

void VulkanCommandPool::Destroy(EnigmaRHI::IDevice* device)
{
	vkDestroyCommandPool(device->API_Vulkan().GetDevice(), commandPool, nullptr);
}
