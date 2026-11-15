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

        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;
        void Draw(uint32_t vertexCount, uint32_t instanceCount) override;

        void TransitionImageLayout(IImage* image, EImageLayout oldLayout, EImageLayout newLayout,
            EAccessFlag srcAccessMask, EAccessFlag destAccessMask, EPipelineStageFlag srcStageMask,
            EPipelineStageFlag dstStageMask) override;

        void SetViewport(const Viewport& viewport) override;
        void SetScissor(const Rect2D& scissor) override;
        void SetLineWidth(float width) override;
        void SetDepthBias(float constantFactor, float clamp, float slopeFactor) override;
        void SetBlendConstants(const float constants[4]) override;
        void SetDepthBounds(float minDepth, float maxDepth) override;
        void SetStencilCompareMask(EStencilFace face, uint32_t mask) override;
        void SetStencilWriteMask(EStencilFace face, uint32_t mask) override;
        void SetStencilReference(EStencilFace face, uint32_t reference) override;
        void SetCullMode(ECullMode mode) override;
        void SetFrontFace(EFrontFaceMode face) override;
        void SetPrimitiveTopology(ETopologyMode topology) override;
        void SetDepthTestEnable(bool enable) override;
        void SetDepthWriteEnable(bool enable) override;
        void SetDepthCompareOp(ECompareOp op) override;
        void SetDepthBoundsTestEnable(bool enable) override;
        void SetStencilTestEnable(bool enable) override;
        void SetStencilOp(EStencilFace face, EStencilOp fail, EStencilOp pass, EStencilOp depthFail, ECompareOp compare) override;
        void SetRasterizerDiscardEnable(bool enable) override;
        void SetDepthBiasEnable(bool enable) override;

        VkCommandBuffer GetHandle() const { return commandBuffer; }

        VulkanCommandBuffer& API_Vulkan() override { return *this; }

    private:

        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    };
}