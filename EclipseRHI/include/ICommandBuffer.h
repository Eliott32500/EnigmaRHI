#pragma once

#include <iostream>
#include "CommonTypes.h"


namespace EnigmaRHI
{
    class VulkanCommandBuffer;
    class IDevice;
	class ICommandPool;
	class IPipeline;
    class ISync;
	class IDescriptor;
	class IRenderPass;
	class ISwapChain;
	class IBuffer;

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

        virtual void BindDescriptorSet(IPipeline* pipeline, IDescriptor* descriptor, ISync* sync) = 0;
        virtual void BindVertexBuffer(IBuffer *buffer) = 0;
        virtual void BindIndexBuffer(IBuffer *buffer) = 0;

        virtual void SetViewport(const Viewport& viewport) = 0;
        virtual void SetScissor(const Rect2D& scissor) = 0;

        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) = 0;
        virtual void Draw(uint32_t vertexCount, uint32_t instanceCount) = 0;

        virtual void TransitionImageLayout(IImage* image, EImageLayout oldLayout, EImageLayout newLayout,
            EAccessFlag srcMask, EAccessFlag destMask, EPipelineStageFlag srcStageFlag,
            EPipelineStageFlag dstStageMask) = 0;

		virtual VulkanCommandBuffer& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanCommandBuffer"); }
    };
}