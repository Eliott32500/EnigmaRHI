#pragma once

#include <iostream>
#include <memory>

#include "ICommandPool.h"

class VulkanBuffer;

namespace EnigmaRHI
{
	class IBuffer
	{
	public:

		struct DescriptorBufferInfo
		{
			IBuffer* buffer;
			size_t offset;
			size_t range;
		};

		virtual ~IBuffer() = default;

		virtual void Create(IDevice* device, size_t size, uint32_t usage,size_t properties) = 0;
		virtual void CreateDescriptorBufferInfo() = 0;
		virtual void CopyBuffer(IDevice* device, ICommandPool* commandPool, IBuffer* dstBuffer, size_t size) = 0;

		virtual void MapMemory(IDevice* device, size_t offset, size_t size, uint32_t flags) = 0;
		virtual void CopyData(const void* src, size_t size) = 0;
		virtual void UnMapMemory(IDevice* device) = 0;

		//Combine Map, Copy, and UnMap directly
		virtual void UploadData(IDevice* device, size_t offset, size_t size, const void* src, uint32_t flags) = 0;

		virtual void Destroy(IDevice* device) = 0;

		DescriptorBufferInfo bufferInfo{};

		virtual VulkanBuffer& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanBuffer"); }
	};
}