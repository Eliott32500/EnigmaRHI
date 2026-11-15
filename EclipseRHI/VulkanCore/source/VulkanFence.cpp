#include "VulkanFence.h"
#include "VulkanDevice.h"
#include "RHILog.h"

void EnigmaRHI::VulkanFence::Create(IDevice* device, bool signaled)
{
    VkFenceCreateInfo info{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                            .flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0u };
    vkCreateFence(device->API_Vulkan().GetHandle(), &info, nullptr, &fence);
}

void EnigmaRHI::VulkanFence::Destroy(IDevice* device)
{
    vkDestroyFence(device->API_Vulkan().GetHandle(), fence, nullptr);
    fence = VK_NULL_HANDLE;
}

bool EnigmaRHI::VulkanFence::Wait(IDevice* device, uint64_t timeout)
{
    return vkWaitForFences(device->API_Vulkan().GetHandle(), 1, &fence, VK_TRUE, timeout) == VK_SUCCESS;
}

void EnigmaRHI::VulkanFence::Reset(IDevice* device)
{
    vkResetFences(device->API_Vulkan().GetHandle(), 1, &fence);
}

bool EnigmaRHI::VulkanFence::IsSignaled(IDevice* device) const
{
    return vkGetFenceStatus(device->API_Vulkan().GetHandle(), fence) == VK_SUCCESS;
}