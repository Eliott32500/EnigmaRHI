#pragma once

#include "../../include/ICommandBuffer.h"
#include "VulkanDevice.h"

#include <array>

class VulkanCommandBuffer : public EnigmaRHI::ICommandBuffer
{
public:

    void Create(EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool) override;

    void BeginDraw(EnigmaRHI::IRenderPass* renderPass, EnigmaRHI::ISwapChain* swapChain, EnigmaRHI::IPipeline* pipeline, uint32_t imageIndex) override;
    void EndDraw() override;

    void BindDescriptorSet(EnigmaRHI::IPipeline* pipeline, EnigmaRHI::IDescriptor* descriptor, EnigmaRHI::ISync* sync) override;
    void BindVertexBuffer(EnigmaRHI::IBuffer* buffer) override;
    void BindIndexBuffer(EnigmaRHI::IBuffer* buffer) override;

    void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;

	VkCommandBuffer GetCommandBuffer() const { return commandBuffer; }

private:

	VkCommandBuffer commandBuffer;
};