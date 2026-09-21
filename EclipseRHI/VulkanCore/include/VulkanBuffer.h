#pragma once

#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "../../include/IBuffer.h"

class VulkanBuffer : public EclipseRHI::IBuffer
{
public:

	void Create(EclipseRHI::IDevice* device, size_t size, uint32_t usage, size_t properties) override;
	void CreateDescriptorBufferInfo() override;
	void CopyBuffer(EclipseRHI::IDevice* device, EclipseRHI::ICommandPool* commandPool, EclipseRHI::IBuffer* dstBuffer, size_t size) override;
	void MapMemory(EclipseRHI::IDevice* device, size_t offset, size_t size, uint32_t flags) override;
	void CopyData(const void* src, size_t size) override;
	void UnMapMemory(EclipseRHI::IDevice* device) override;
	//Combine Map, Copy, and UnMap directly
	void UploadData(EclipseRHI::IDevice* device, size_t offset, size_t size, const void* src, uint32_t flags) override;
	void Destroy(EclipseRHI::IDevice* vulkanDevice) override;

	VkBuffer GetBuffer() const { return buffer; };
	VkDeviceMemory GetBufferMemory() const { return bufferMemory; };

	VulkanBuffer& API_Vulkan() override { return (*this); }

private:

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;

	void* data;
};