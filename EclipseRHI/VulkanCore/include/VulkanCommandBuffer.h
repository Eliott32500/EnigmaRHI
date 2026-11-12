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

        void Begin() override;
        void End() override;
        void Reset() override;

        void BeginRendering(const RenderingInfo& renderingInfo) override;

        void EndRendering() override;

        void BindPipeline(IPipeline* pipeline) override;

        void BindDescriptorSet(IPipeline* pipeline, IDescriptor* descriptor, ISync* sync) override;

        void BindVertexBuffer(IBuffer* buffer) override;
        void BindIndexBuffer(IBuffer* buffer) override;

        void SetViewport(const Viewport& viewport) override;
        void SetScissor(const Rect2D& scissor) override;

        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;
        void Draw(uint32_t vertexCount, uint32_t instanceCount) override;

        void TransitionImageLayout(IImage* image, EImageLayout oldLayout, EImageLayout newLayout,
            EAccessFlag srcAccessMask, EAccessFlag destAccessMask, EPipelineStageFlag srcStageMask,
            EPipelineStageFlag dstStageMask) override;

        VkCommandBuffer GetCommandBuffer() const { return commandBuffer; }

        VulkanCommandBuffer& API_Vulkan() override { return *this; }

    private:

        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    };
}