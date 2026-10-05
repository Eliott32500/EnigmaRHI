#pragma once

#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "IBuffer.h"

class VulkanBuffer : public EnigmaRHI::IBuffer
{
public:

	void Create(EnigmaRHI::IDevice* device, size_t size, uint32_t usage, size_t properties) override;
	void CreateDescriptorBufferInfo() override;
	void CopyBuffer(EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool, EnigmaRHI::IBuffer* dstBuffer, size_t size) override;
	void MapMemory(EnigmaRHI::IDevice* device, size_t offset, size_t size, uint32_t flags) override;
	void CopyData(const void* src, size_t size) override;
	void UnMapMemory(EnigmaRHI::IDevice* device) override;
	//Combine Map, Copy, and UnMap directly
	void UploadData(EnigmaRHI::IDevice* device, size_t offset, size_t size, const void* src, uint32_t flags) override;
	void Destroy(EnigmaRHI::IDevice* vulkanDevice) override;

	VkBuffer GetBuffer() const { return buffer; };
	VkDeviceMemory GetBufferMemory() const { return bufferMemory; };

	VulkanBuffer& API_Vulkan() override { return (*this); }

private:

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;

	void* data;
};