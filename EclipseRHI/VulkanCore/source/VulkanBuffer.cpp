#include "VulkanBuffer.h"

void EnigmaRHI::VulkanBuffer::Create(IDevice* device, size_t size, uint32_t usage, size_t properties)
{
	VkBufferCreateInfo bufferInfo
	{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	};

	if (vkCreateBuffer(device->API_Vulkan().GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer!");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device->API_Vulkan().GetDevice(), buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = device->API_Vulkan().FindMemoryType(memRequirements.memoryTypeBits, static_cast<VkMemoryPropertyFlags>(properties)),
	};

	if (vkAllocateMemory(device->API_Vulkan().GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate buffer memory!");

	vkBindBufferMemory(device->API_Vulkan().GetDevice(), buffer, bufferMemory, 0);
}

void EnigmaRHI::VulkanBuffer::CreateDescriptorBufferInfo()
{
	bufferInfo.buffer = this;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(this);
}

void EnigmaRHI::VulkanBuffer::CopyBuffer(IDevice* device, ICommandPool* commandPool, IBuffer* dstBuffer, size_t size)
{
	VkCommandBuffer commandBuffer = commandPool->API_Vulkan().BeginSingleTimeCommands(&device->API_Vulkan());

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, this->buffer, dstBuffer->API_Vulkan().GetBuffer(), 1, &copyRegion);

	commandPool->API_Vulkan().EndSingleTimeCommands(&device->API_Vulkan(), commandBuffer);
}

void EnigmaRHI::VulkanBuffer::UploadData(IDevice* device, size_t offset, size_t size, const void* src, uint32_t flags)
{
	MapMemory(device, offset, size, flags);
	CopyData(src, size);
	UnMapMemory(device);
}

void EnigmaRHI::VulkanBuffer::Destroy(IDevice* device)
{
	vkDestroyBuffer(device->API_Vulkan().GetDevice(), buffer, nullptr);
	vkFreeMemory(device->API_Vulkan().GetDevice(), bufferMemory, nullptr);
}

void EnigmaRHI::VulkanBuffer::MapMemory(IDevice* device, size_t offset, size_t size, uint32_t flags)
{
	vkMapMemory(device->API_Vulkan().GetDevice(), bufferMemory, offset, size, flags, &data);
}

void EnigmaRHI::VulkanBuffer::CopyData(const void* src, size_t size)
{
	memcpy(data, src, size);
}

void EnigmaRHI::VulkanBuffer::UnMapMemory(IDevice* device)
{
	vkUnmapMemory(device->API_Vulkan().GetDevice(), bufferMemory);
}