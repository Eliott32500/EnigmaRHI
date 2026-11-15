#include "VulkanBufferTypes.h"

VkBufferUsageFlags EnigmaRHI::Utilities::ToVkBufferUsage(EBufferUsage usage)
{
    using namespace EnigmaRHI;
    VkBufferUsageFlags f = 0;
    if (HasFlag(usage, EBufferUsage::VERTEX))       f |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (HasFlag(usage, EBufferUsage::INDEX))        f |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if (HasFlag(usage, EBufferUsage::UNIFORM))      f |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if (HasFlag(usage, EBufferUsage::STORAGE))      f |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    if (HasFlag(usage, EBufferUsage::TRANSFER_SRC)) f |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (HasFlag(usage, EBufferUsage::TRANSFER_DST)) f |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    if (HasFlag(usage, EBufferUsage::INDIRECT))     f |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    return f;
}

VkMemoryPropertyFlags EnigmaRHI::Utilities::ToVkMemory(EMemoryAccess access)
{
    switch (access)
    {
    case EnigmaRHI::EMemoryAccess::CPU_TO_GPU:
    case EnigmaRHI::EMemoryAccess::GPU_TO_CPU:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    default:
        return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
}
