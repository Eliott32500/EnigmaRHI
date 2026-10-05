#include "VulkanSync.h"

void VulkanSync::Create(EnigmaRHI::IDevice* device)
{
	imageAvailableSemaphores.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);

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
		if (vkCreateSemaphore(device->API_Vulkan().GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device->API_Vulkan().GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device->API_Vulkan().GetDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void VulkanSync::Destroy(EnigmaRHI::IDevice* device)
{
	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device->API_Vulkan().GetDevice(), renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device->API_Vulkan().GetDevice(), imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device->API_Vulkan().GetDevice(), inFlightFences[i], nullptr);
	}
}

void VulkanSync::AquireNextImage(EnigmaRHI::IDevice* device, EnigmaRHI::ISwapChain* swapChain, EnigmaRHI::ICommandPool* commandPool, EnigmaRHI::ISurface* surface, EnigmaRHI::IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex)
{
	vkWaitForFences(device->API_Vulkan().GetDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	nextImage = vkAcquireNextImageKHR(device->API_Vulkan().GetDevice(), swapChain->API_Vulkan().GetSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, imageIndex);

	// VK_ERROR_OUT_OF_DATE_KHR: The swap chain has become incompatible with the surface and can no longer be used for rendering.Usually happens after a window resize.
	// VK_SUBOPTIMAL_KHR : The swap chain can still be used to successfully present to the surface, but the surface properties are no longer matched exactly.
	if (nextImage == VK_ERROR_OUT_OF_DATE_KHR)
	{
		swapChain->API_Vulkan().RecreateSwapChain(&device->API_Vulkan(), &commandPool->API_Vulkan(), &surface->API_Vulkan(), renderPass->API_Vulkan().GetRenderPass(), window);
		return;
	}
	else if (nextImage != VK_SUCCESS && nextImage != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(device->API_Vulkan().GetDevice(), 1, &inFlightFences[currentFrame]);
}

void VulkanSync::PresentFrame(EnigmaRHI::IDevice* device, EnigmaRHI::ISwapChain* swapChain, EnigmaRHI::ICommandPool* commandPool, EnigmaRHI::ISurface* surface, EnigmaRHI::IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex)
{
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };

	VkCommandBuffer commandBuffer = commandPool->API_Vulkan().GetVulkanCommandBuffer(currentFrame).GetCommandBuffer();

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

	if (vkQueueSubmit(device->API_Vulkan().GetGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer!");

	VkSwapchainKHR swapChains[] = { swapChain->API_Vulkan().GetSwapChain() };

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

	nextImage = vkQueuePresentKHR(device->API_Vulkan().GetPresQueue(), &presentInfo);

	if (nextImage == VK_ERROR_OUT_OF_DATE_KHR || nextImage == VK_SUBOPTIMAL_KHR)
	{
		swapChain->API_Vulkan().RecreateSwapChain(&device->API_Vulkan(), &commandPool->API_Vulkan(), &surface->API_Vulkan(), renderPass->API_Vulkan().GetRenderPass(), window);
	}

	else if (nextImage != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	vkQueueWaitIdle(device->API_Vulkan().GetPresQueue());
}