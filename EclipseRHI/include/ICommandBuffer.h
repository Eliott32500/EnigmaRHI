#pragma once

#include <iostream>


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

        virtual void BeginDraw(IRenderPass* renderPass, ISwapChain* swapChain, IPipeline* pipeline, uint32_t imageIndex) = 0;
        virtual void EndDraw() = 0;

        virtual void BindDescriptorSet(IPipeline* pipeline, IDescriptor* descriptor, EnigmaRHI::ISync* sync) = 0;
        virtual void BindVertexBuffer(IBuffer *buffer) = 0;
        virtual void BindIndexBuffer(IBuffer *buffer) = 0;

        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) = 0;

		virtual VulkanCommandBuffer& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanCommandBuffer"); }
    };
}