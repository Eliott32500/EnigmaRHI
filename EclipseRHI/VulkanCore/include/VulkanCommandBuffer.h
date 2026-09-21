#pragma once

#include "../../include/ICommandBuffer.h"
#include "VulkanDevice.h"

#include <array>

class VulkanCommandBuffer : public EclipseRHI::ICommandBuffer
{
public:

    void Create(EclipseRHI::IDevice* device, EclipseRHI::ICommandPool* commandPool) override;

    void BeginDraw(EclipseRHI::IRenderPass* renderPass, EclipseRHI::ISwapChain* swapChain, EclipseRHI::IPipeline* pipeline, uint32_t imageIndex) override;
    void EndDraw() override;

    void BindDescriptorSet(EclipseRHI::IPipeline* pipeline, EclipseRHI::IDescriptor* descriptor, EclipseRHI::ISync* sync) override;
    void BindVertexBuffer(EclipseRHI::IBuffer* buffer) override;
    void BindIndexBuffer(EclipseRHI::IBuffer* buffer) override;

    void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;

	VkCommandBuffer GetCommandBuffer() const { return commandBuffer; }

private:

	VkCommandBuffer commandBuffer;
};