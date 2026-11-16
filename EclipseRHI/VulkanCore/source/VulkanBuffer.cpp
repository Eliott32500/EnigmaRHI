#include "VulkanBuffer.h"
#include "RHILog.h"
#include "VulkanBufferTypes.h"

void EnigmaRHI::VulkanBuffer::Create(IDevice* device, const BufferDesc& d)
{
    VulkanDevice& dev = device->API_Vulkan();
    VkDevice vk = dev.GetHandle();
    desc = d;

    VkBufferCreateInfo info
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = d.size,
        .usage = Utilities::ToVkBufferUsage(d.usage),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (vkCreateBuffer(vk, &info, nullptr, &buffer) != VK_SUCCESS)
        LOG_FATAL(ELogCategory::VULKAN, "Failed to create buffer !");

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(vk, buffer, &req);

    VkMemoryAllocateInfo alloc{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = req.size,
        // adapte au nom réel de ta fonction de recherche de memory type
        .memoryTypeIndex = dev.GetPhysicalDevice().FindMemoryType(req.memoryTypeBits, Utilities::ToVkMemory(d.access)),
    };
    if (vkAllocateMemory(vk, &alloc, nullptr, &memory) != VK_SUCCESS)
        LOG_FATAL(ELogCategory::VULKAN, "Failed to allocate buffer memory !");

    vkBindBufferMemory(vk, buffer, memory, 0);

    if (d.persistentMap && d.access != EMemoryAccess::GPU_ONLY)
        vkMapMemory(vk, memory, 0, VK_WHOLE_SIZE, 0, &mapped);
}

void EnigmaRHI::VulkanBuffer::Destroy(IDevice* device)
{
    if (buffer == VK_NULL_HANDLE)
        return;                                   // idempotent : sûr si appelé deux fois

    VkDevice vk = device->API_Vulkan().GetHandle();

    if (mapped)
    {
        vkUnmapMemory(vk, memory);
        mapped = nullptr;
    }
    vkDestroyBuffer(vk, buffer, nullptr);
    vkFreeMemory(vk, memory, nullptr);
    buffer = VK_NULL_HANDLE;
    memory = VK_NULL_HANDLE;
}

void* EnigmaRHI::VulkanBuffer::Map(IDevice* device)
{
    if (desc.access == EMemoryAccess::GPU_ONLY)
        LOG_FATAL(ELogCategory::VULKAN, "Cannot map a GPU_ONLY buffer !");

    if (!mapped)
        vkMapMemory(device->API_Vulkan().GetHandle(), memory, 0, VK_WHOLE_SIZE, 0, &mapped);
    return mapped;
}

void EnigmaRHI::VulkanBuffer::Unmap(IDevice* device)
{
    if (!mapped || desc.persistentMap)            // un buffer persistent reste mappé
        return;

    vkUnmapMemory(device->API_Vulkan().GetHandle(), memory);
    mapped = nullptr;
}

void EnigmaRHI::VulkanBuffer::Write(IDevice* device, const void* data, size_t size, size_t offset)
{
    if (offset + size > desc.size)
        LOG_FATAL(ELogCategory::VULKAN, "Buffer write out of range !");

    std::memcpy(static_cast<char*>(Map(device)) + offset, data, size);
    Unmap(device);
}

void EnigmaRHI::VulkanBuffer::CopyTo(VulkanDevice* device, VulkanCommandPool* commandPool, VulkanBuffer& dst, size_t size, size_t srcOffset, size_t dstOffset)
{
    VkCommandBuffer cmd = commandPool->BeginSingleTimeCommands(device, commandPool->GetCommandBuffer(0)->API_Vulkan().GetHandle());

    VkBufferCopy region{ .srcOffset = srcOffset, .dstOffset = dstOffset, .size = size };
    vkCmdCopyBuffer(cmd, buffer, dst.buffer, 1, &region);

    commandPool->EndSingleTimeCommands(device, cmd);
}