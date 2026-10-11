#include "VulkanQueue.h"

VkResult EnigmaRHI::VulkanQueue::Submit(VkCommandBuffer cmd,
    VkSemaphore wait, VkPipelineStageFlags2 waitStage,
    VkSemaphore signal, VkPipelineStageFlags2 signalStage,
    VkFence fence) const
{
    VkCommandBufferSubmitInfo cmdInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = cmd,
    };
    VkSemaphoreSubmitInfo waitInfo {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = wait,
        .stageMask = waitStage,
    };
    VkSemaphoreSubmitInfo signalInfo {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = signal,
        .stageMask = signalStage,
    };
    VkSubmitInfo2 submit {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount = wait ? 1u : 0u,
        .pWaitSemaphoreInfos = &waitInfo,
        .commandBufferInfoCount = 1,
        .pCommandBufferInfos = &cmdInfo,
        .signalSemaphoreInfoCount = signal ? 1u : 0u,
        .pSignalSemaphoreInfos = &signalInfo,
    };
    
    return vkQueueSubmit2(queue, 1, &submit, fence);
}

VkResult EnigmaRHI::VulkanQueue::Present(VkSwapchainKHR swapchain, uint32_t imageIndex, VkSemaphore wait) const
{
    VkPresentInfoKHR info {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &wait,
        .swapchainCount = 1,
        .pSwapchains = &swapchain,
        .pImageIndices = &imageIndex,
    };

    return vkQueuePresentKHR(queue, &info);
}