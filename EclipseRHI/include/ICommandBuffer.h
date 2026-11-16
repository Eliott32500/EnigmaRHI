#pragma once

#include <iostream>
#include "CommonTypes.h"
#include "DepthStencilTypes.h"
#include "RasterizationTypes.h"

namespace EnigmaRHI
{
    class VulkanCommandBuffer;
    class IDevice;
	class ICommandPool;
	class IPipeline;
    class ISync;
	class IDescriptor;
	class ISwapChain;
	class IBuffer;
    class IBindingSet;

    class ICommandBuffer
    {
    public:
        virtual ~ICommandBuffer() = default;

        virtual void Create(IDevice* device, ICommandPool* commandPool) = 0;

        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void Reset() = 0;

        virtual void BeginRendering(const RenderingInfo& renderingInfo) = 0;
        virtual void EndRendering() = 0;

        virtual void BindPipeline(IPipeline* pipeline) = 0;
        virtual void BindBindingSet(IPipeline* pipeline, IBindingSet* set, uint32_t setIndex = 0) = 0;
        virtual void BindVertexBuffer(IBuffer* buffer, size_t offset = 0, uint32_t binding = 0) = 0;
        virtual void BindIndexBuffer(IBuffer *buffer) = 0;

        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) = 0;
        virtual void Draw(uint32_t vertexCount, uint32_t instanceCount) = 0;

        virtual void TransitionImage(IImage* image, EImageLayout newLayout) = 0;

        virtual void SetViewport(const Viewport& viewport) = 0;
        virtual void SetScissor(const Rect2D& scissor) = 0;
        virtual void SetLineWidth(float width) = 0;
        virtual void SetDepthBias(float constantFactor, float clamp, float slopeFactor) = 0;
        virtual void SetBlendConstants(const float constants[4]) = 0;
        virtual void SetDepthBounds(float minDepth, float maxDepth) = 0;
        virtual void SetStencilCompareMask(EStencilFace face, uint32_t mask) = 0;
        virtual void SetStencilWriteMask(EStencilFace face, uint32_t mask) = 0;
        virtual void SetStencilReference(EStencilFace face, uint32_t reference) = 0;
        virtual void SetCullMode(ECullMode mode) = 0;
        virtual void SetFrontFace(EFrontFaceMode face) = 0;
        virtual void SetPrimitiveTopology(ETopologyMode topology) = 0;
        virtual void SetDepthTestEnable(bool enable) = 0;
        virtual void SetDepthWriteEnable(bool enable) = 0;
        virtual void SetDepthCompareOp(ECompareOp op) = 0;
        virtual void SetDepthBoundsTestEnable(bool enable) = 0;
        virtual void SetStencilTestEnable(bool enable) = 0;
        virtual void SetStencilOp(EStencilFace face, EStencilOp fail, EStencilOp pass, EStencilOp depthFail, ECompareOp compare) = 0;
        virtual void SetRasterizerDiscardEnable(bool enable) = 0;
        virtual void SetDepthBiasEnable(bool enable) = 0;


		virtual VulkanCommandBuffer& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanCommandBuffer"); }
    };
}