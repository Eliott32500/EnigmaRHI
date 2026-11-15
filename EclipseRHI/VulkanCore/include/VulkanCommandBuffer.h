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

        void BindBindingSet(IPipeline* pipeline, IBindingSet* set, uint32_t setIndex = 0) override;

        void BindVertexBuffer(IBuffer* buffer, size_t offset = 0, uint32_t binding = 0) override;
        void BindIndexBuffer(IBuffer* buffer) override;

        void SetViewport(const Viewport& viewport) override;
        void SetScissor(const Rect2D& scissor) override;

        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;
        void Draw(uint32_t vertexCount, uint32_t instanceCount) override;

        void TransitionImageLayout(IImage* image, EImageLayout oldLayout, EImageLayout newLayout,
            EAccessFlag srcAccessMask, EAccessFlag destAccessMask, EPipelineStageFlag srcStageMask,
            EPipelineStageFlag dstStageMask) override;

        VkCommandBuffer GetHandle() const { return commandBuffer; }

        VulkanCommandBuffer& API_Vulkan() override { return *this; }

    private:

        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    };
}