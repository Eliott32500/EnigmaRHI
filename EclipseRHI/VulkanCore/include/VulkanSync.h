#pragma once

#include <iostream>
#include <vector>
#include "../../include/ISync.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"

class VulkanSync : public EnigmaRHI::ISync
{
public:

	void Create(EnigmaRHI::IDevice* device) override;
	void Destroy(EnigmaRHI::IDevice* device) override;

	void AquireNextImage(EnigmaRHI::IDevice* device, EnigmaRHI::ISwapChain* swapChain, EnigmaRHI::ICommandPool* commandPool, EnigmaRHI::ISurface* surface, EnigmaRHI::IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) override;
	void PresentFrame(EnigmaRHI::IDevice* device, EnigmaRHI::ISwapChain* swapChain, EnigmaRHI::ICommandPool* commandPool, EnigmaRHI::ISurface* surface, EnigmaRHI::IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) override;

	VulkanSync& API_Vulkan() override { return (*this); }

private:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	VkResult nextImage;
};