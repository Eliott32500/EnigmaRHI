#pragma once

#include <iostream>
#include <vector>
#include "../../include/ISync.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"

class VulkanSync : public EclipseRHI::ISync
{
public:

	void Create(EclipseRHI::IDevice* device) override;
	void Destroy(EclipseRHI::IDevice* device) override;

	void AquireNextImage(EclipseRHI::IDevice* device, EclipseRHI::ISwapChain* swapChain, EclipseRHI::ICommandPool* commandPool, EclipseRHI::ISurface* surface, EclipseRHI::IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) override;
	void PresentFrame(EclipseRHI::IDevice* device, EclipseRHI::ISwapChain* swapChain, EclipseRHI::ICommandPool* commandPool, EclipseRHI::ISurface* surface, EclipseRHI::IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) override;

	VulkanSync& API_Vulkan() override { return (*this); }

private:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	VkResult nextImage;
};