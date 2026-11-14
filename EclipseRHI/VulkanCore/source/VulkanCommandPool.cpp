#include "VulkanCommandPool.h"
#include "RHIConfig.h"
#include "RHILog.h"

void EnigmaRHI::VulkanCommandPool::Create(IDevice* device)
{
	VulkanDevice* vulkanDevice = &device->API_Vulkan();

	CreateCommandPool(vulkanDevice);

	AllocateCommandBuffers(
		vulkanDevice,
		EnigmaRHI::MAX_FRAMES_IN_FLIGHT
	);
}

void EnigmaRHI::VulkanCommandPool::CreateCommandPool(VulkanDevice* device)
{
	VkCommandPoolCreateInfo poolInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = device->GetGraphicsQueue().FamilyIndex(),
	};
	
	if (vkCreateCommandPool(device->GetHandle(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to create command pool !");
}

void EnigmaRHI::VulkanCommandPool::Reset(IDevice* device)
{
	if (vkResetCommandPool(device->API_Vulkan().GetHandle(), commandPool, 0) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to reset command pool !");
}

VkCommandBuffer EnigmaRHI::VulkanCommandPool::BeginSingleTimeCommands(VulkanDevice* device, VkCommandBuffer commandBuffer)
{
	VkCommandBufferAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	
	if(vkAllocateCommandBuffers(device->GetHandle(), &allocInfo, &commandBuffer) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to allocate command buffer !");
	
	VkCommandBufferBeginInfo beginInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};
	
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	
	return commandBuffer;
}

void EnigmaRHI::VulkanCommandPool::AllocateCommandBuffers(VulkanDevice* device, uint32_t count)
{
	commandBuffers.resize(count);

	for (auto& commandBuffer : commandBuffers)
	{
		commandBuffer.Create(device, this);
	}
}

void EnigmaRHI::VulkanCommandPool::EndSingleTimeCommands(VulkanDevice* device, VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);
	
	VkSubmitInfo submitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &commandBuffer,
	};
	
	vkQueueSubmit(device->GetGraphicsQueue().Handle(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(device->GetGraphicsQueue().Handle());
	
	vkFreeCommandBuffers(device->GetHandle(), commandPool, 1, &commandBuffer);
}

void EnigmaRHI::VulkanCommandPool::Destroy(IDevice* device)
{
	if (commandPool == VK_NULL_HANDLE)
		return;

	vkDestroyCommandPool(device->API_Vulkan().GetHandle(), commandPool, nullptr);

	commandPool = VK_NULL_HANDLE;
	commandBuffers.clear();
}
