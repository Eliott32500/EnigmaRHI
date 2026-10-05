#pragma once

#include <iostream>
#include <vector>
#include "ISync.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"

namespace EnigmaRHI
{
	class VulkanSync : public ISync
	{
	public:

		void Create(IDevice* device) override;
		void Destroy(IDevice* device) override;

		void AquireNextImage(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) override;
		void PresentFrame(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) override;

		VulkanSync& API_Vulkan() override { return (*this); }

	private:
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		VkResult nextImage;
	};
}