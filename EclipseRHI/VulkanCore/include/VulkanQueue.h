#pragma once
#include "volk.h"
#include "IQueue.h"
#include <cstdint>

namespace EnigmaRHI
{
    class VulkanQueue : IQueue
    {
    public:

        void Init(VkDevice device, uint32_t familyIndex, uint32_t queueIndex = 0)
        {
            this->familyIndex = familyIndex;
            vkGetDeviceQueue(device, familyIndex, queueIndex, &queue);
        }

        VkQueue  Handle() const { return queue; }
        uint32_t FamilyIndex() const override { return familyIndex; }
        void WaitIdle() const override { vkQueueWaitIdle(queue); };
        ECommandQueueType GetType() const override {return type;};
        VulkanQueue& API_Vulkan() override { return *this; }

        VkResult Submit(VkCommandBuffer cmd,
            VkSemaphore wait, VkPipelineStageFlags2 waitStage,
            VkSemaphore signal, VkPipelineStageFlags2 signalStage,
            VkFence fence) const;

        VkResult Present(VkSwapchainKHR swapchain, uint32_t imageIndex, VkSemaphore wait) const;

    private:
        VkQueue  queue = VK_NULL_HANDLE;
        uint32_t familyIndex = 0;
        ECommandQueueType type = ECommandQueueType::GRAPHICS;
    };
}