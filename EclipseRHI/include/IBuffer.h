#pragma once

#include <iostream>
#include <memory>

#include "ICommandPool.h"
#include "BufferTypes.h"

namespace EnigmaRHI
{
	class VulkanBuffer;

    struct DescriptorBufferInfo
    {
        IBuffer* buffer;
        size_t offset;
        size_t range;
    };

    struct BufferDesc
    {
        size_t size = 0;
        EBufferUsage  usage = EBufferUsage::NONE;
        EMemoryAccess access = EMemoryAccess::GPU_ONLY;
        bool persistentMap = false;
    };

	class IBuffer
    {
    public:
        virtual ~IBuffer() = default;

        virtual void Create(IDevice* device, const BufferDesc& desc) = 0;
        virtual void Destroy(IDevice* device) = 0;

        virtual void* Map(IDevice* device) = 0;
        virtual void  Unmap(IDevice* device) = 0;
        virtual void  Write(IDevice* device, const void* data, size_t size, size_t offset = 0) = 0;

        const BufferDesc& GetDesc() const { return desc; }
        size_t GetSize() const { return desc.size; }

        virtual VulkanBuffer& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanBuffer"); }

    protected:
        BufferDesc desc{};
    };
}