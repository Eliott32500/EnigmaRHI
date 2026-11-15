#pragma once
#include "volk.h"
#include "IQueue.h"

namespace EnigmaRHI
{
    class IDevice;

    class VulkanQueue : public IQueue
    {
    public:
        void Init(VkDevice device, uint32_t familyIndex, ECommandQueueType type, uint32_t queueIndex = 0);

        VkQueue  GetHandle() const { return queue; }
        uint32_t FamilyIndex() const override { return familyIndex; }
        ECommandQueueType GetType() const override { return type; }
        void WaitIdle() const override { vkQueueWaitIdle(queue); }

        void Submit(ICommandBuffer* cmd, ISwapChain* swapChain, IFence* fence) override;

        VulkanQueue& API_Vulkan() override { return *this; }

        VkResult Present(VkSwapchainKHR swapchain, uint32_t imageIndex, VkSemaphore wait) const;

    private:
        VkResult SubmitInternal(VkCommandBuffer cmd,
            VkSemaphore wait, VkPipelineStageFlags2 waitStage,
            VkSemaphore signal, VkPipelineStageFlags2 signalStage,
            VkFence fence) const;

        VkQueue  queue = VK_NULL_HANDLE;
        uint32_t familyIndex = 0;
        ECommandQueueType type = ECommandQueueType::GRAPHICS;
    };
}