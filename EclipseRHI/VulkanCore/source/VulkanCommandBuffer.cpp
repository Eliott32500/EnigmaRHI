#include "../include/VulkanCommandBuffer.h"
#include "../include/VulkanCommandPool.h"
#include "../include/VulkanPipeline.h"

#include "../../include/ISync.h"

void VulkanCommandBuffer::Create(EclipseRHI::IDevice* device, EclipseRHI::ICommandPool* commandPool)
{
	VkCommandBufferAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool->API_Vulkan().GetCommandPool(),
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};

	if (vkAllocateCommandBuffers(device->API_Vulkan().GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");
}

void VulkanCommandBuffer::BeginDraw(EclipseRHI::IRenderPass* renderPass, EclipseRHI::ISwapChain* swapChain, EclipseRHI::IPipeline* pipeline, uint32_t imageIndex)
{
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0, // Optional
		.pInheritanceInfo = nullptr, // Optional
	};

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = renderPass->API_Vulkan().GetRenderPass(),
		.framebuffer = swapChain->API_Vulkan().GetSwapChainFramebuffers()[imageIndex],
		.clearValueCount = static_cast<uint32_t>(clearValues.size()),
		.pClearValues = clearValues.data(),
	};

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->API_Vulkan().GetSwapChainExtent();

	////// BEGIN ///////
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->API_Vulkan().GetGraphicsPipeline());

	VkViewport viewport
	{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<float>(swapChain->API_Vulkan().GetSwapChainExtent().width),
		.height = static_cast<float>(swapChain->API_Vulkan().GetSwapChainExtent().height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor
	{
		.offset = { 0, 0 },
		.extent = swapChain->API_Vulkan().GetSwapChainExtent(),
	};
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void VulkanCommandBuffer::EndDraw()
{
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to record command buffer!");
}


void VulkanCommandBuffer::BindDescriptorSet(EclipseRHI::IPipeline* pipeline, EclipseRHI::IDescriptor* descriptor, EclipseRHI::ISync* sync)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->API_Vulkan().GetPipelineLayout(), 0, 1, &descriptor->API_Vulkan().GetDescriptorSets()[sync->GetCurrentFrame()], 0, nullptr);
}

void VulkanCommandBuffer::BindVertexBuffer(EclipseRHI::IBuffer* buffer)
{
	VkBuffer vertexBuffers[] = { buffer->API_Vulkan().GetBuffer()};
	VkDeviceSize offsets[] = { buffer->API_Vulkan().bufferInfo.offset};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

void VulkanCommandBuffer::BindIndexBuffer(EclipseRHI::IBuffer* buffer)
{
	vkCmdBindIndexBuffer(commandBuffer, buffer->API_Vulkan().GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void VulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount)
{
	vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, 0, 0, 0);
}
