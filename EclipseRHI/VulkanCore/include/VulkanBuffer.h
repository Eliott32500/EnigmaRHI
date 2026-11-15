#pragma once

#include "VulkanDevice.h"
#include "VulkanCommandPool.h"
#include "IBuffer.h"

namespace EnigmaRHI
{
    class VulkanBuffer : public IBuffer
    {
    public:
        void Create(IDevice* device, const BufferDesc& desc) override;
        void Destroy(IDevice* device) override;

        void* Map(IDevice* device) override;
        void  Unmap(IDevice* device) override;
        void  Write(IDevice* device, const void* data, size_t size, size_t offset = 0) override;

        void CopyTo(VulkanDevice* device, VulkanCommandPool* commandPool, VulkanBuffer& dst, size_t size, size_t srcOffset = 0, size_t dstOffset = 0);

        VkBuffer       GetHandle() const { return buffer; }
        VkDeviceMemory GetMemory() const { return memory; }
        VkDescriptorBufferInfo GetDescriptorInfo() const { return { buffer, 0, desc.size }; }

        VulkanBuffer& API_Vulkan() override { return *this; }

    private:
        VkBuffer       buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        void* mapped = nullptr;
    };
}