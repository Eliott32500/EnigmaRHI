#include "VulkanSync.h"
#include "RHILog.h"

void EnigmaRHI::VulkanSync::Create(EnigmaRHI::IDevice* device)
{
	presentCompleteSemaphore.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);
	drawFences.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo
	{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};

	VkFenceCreateInfo fenceInfo
	{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};

	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device->API_Vulkan().GetHandle(), &semaphoreInfo, nullptr, &presentCompleteSemaphore[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device->API_Vulkan().GetHandle(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device->API_Vulkan().GetHandle(), &fenceInfo, nullptr, &drawFences[i]) != VK_SUCCESS) {

			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void EnigmaRHI::VulkanSync::Destroy(IDevice* device)
{
	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device->API_Vulkan().GetHandle(), renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device->API_Vulkan().GetHandle(), presentCompleteSemaphore[i], nullptr);
		vkDestroyFence(device->API_Vulkan().GetHandle(), drawFences[i], nullptr);
	}
}

bool EnigmaRHI::VulkanSync::AquireNextImage(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex)
{
	if (vkWaitForFences(device->API_Vulkan().GetHandle(), 1, &drawFences[currentFrame], VK_TRUE, UINT64_MAX) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to wait for fence!");

	nextImage = vkAcquireNextImageKHR(device->API_Vulkan().GetHandle(), swapChain->API_Vulkan().GetHandle(), UINT64_MAX, presentCompleteSemaphore[currentFrame], VK_NULL_HANDLE, imageIndex);

	// VK_ERROR_OUT_OF_DATE_KHR: The swap chain has become incompatible with the surface and can no longer be used for rendering.Usually happens after a window resize.
	// VK_SUBOPTIMAL_KHR : The swap chain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
	if (nextImage == VK_ERROR_OUT_OF_DATE_KHR)
	{
		swapChain->API_Vulkan().RecreateSwapChain(&device->API_Vulkan(), &commandPool->API_Vulkan(), &surface->API_Vulkan(), renderPass->API_Vulkan().GetRenderPass(), window);
		return false;
	}
	else if (nextImage != VK_SUCCESS && nextImage != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(device->API_Vulkan().GetHandle(), 1, &drawFences[currentFrame]);
	return true;
}

void EnigmaRHI::VulkanSync::PresentFrame(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex)
{
	VkSemaphore waitSemaphores[] = { presentCompleteSemaphore[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };

	VkCommandBuffer commandBuffer = commandPool->API_Vulkan().GetVulkanCommandBuffer(currentFrame).GetHandle();

	VkSubmitInfo submitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = waitSemaphores,
		.pWaitDstStageMask = waitStages,
		.commandBufferCount = 1,
		.pCommandBuffers = &commandBuffer,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = signalSemaphores,
	};

	if (vkQueueSubmit(device->API_Vulkan().GetGraphicsQueue().Handle(), 1, &submitInfo, drawFences[currentFrame]) != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer!");

	VkSwapchainKHR swapChains[] = { swapChain->API_Vulkan().GetHandle() };

	VkPresentInfoKHR presentInfo
	{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = signalSemaphores,
		.swapchainCount = 1,
		.pSwapchains = swapChains,
		.pImageIndices = imageIndex,
		.pResults = nullptr, // Optional
	};

	nextImage = vkQueuePresentKHR(device->API_Vulkan().GetPresentQueue().Handle(), &presentInfo);

	if (nextImage == VK_ERROR_OUT_OF_DATE_KHR || nextImage == VK_SUBOPTIMAL_KHR)
	{
		swapChain->API_Vulkan().RecreateSwapChain(&device->API_Vulkan(), &commandPool->API_Vulkan(), &surface->API_Vulkan(), renderPass->API_Vulkan().GetRenderPass(), window);
	}
	else if (nextImage != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}
}