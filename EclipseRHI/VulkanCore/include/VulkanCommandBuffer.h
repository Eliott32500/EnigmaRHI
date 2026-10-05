#pragma once

#include "ICommandBuffer.h"
#include "VulkanDevice.h"

#include <array>

namespace EnigmaRHI
{
    class VulkanCommandBuffer : public ICommandBuffer
    {
    public:

        void Create(IDevice* device, ICommandPool* commandPool) override;

        void BeginDraw(IRenderPass* renderPass, ISwapChain* swapChain, IPipeline* pipeline, uint32_t imageIndex) override;
        void EndDraw() override;

        void BindDescriptorSet(IPipeline* pipeline, IDescriptor* descriptor, ISync* sync) override;
        void BindVertexBuffer(IBuffer* buffer) override;
        void BindIndexBuffer(IBuffer* buffer) override;

        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;

        VkCommandBuffer GetCommandBuffer() const { return commandBuffer; }

    private:

        VkCommandBuffer commandBuffer;
    };
}