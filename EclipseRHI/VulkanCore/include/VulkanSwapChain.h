#pragma once

#include "ISwapChain.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include <cstdint>

namespace EnigmaRHI
{
	class VulkanSwapChain : public ISwapChain
	{
	public:

		void Create(IDevice* device, ISurface* surface, Extent2D windowExtent) override;
		void Destroy(IDevice* device) override;
		void Resize(IDevice* device, ISurface* surface, Extent2D newExtent) override;
		EAcquireResult Acquire(IDevice* device, uint32_t frame) override;
		EPresentResult Present(IDevice* device, IQueue* queue) override;

		VkSemaphore GetImageAvailable() const { return imageAvailableSemaphores[currentFrame]; }
		VkSemaphore GetRenderFinished() const { return renderFinishedSemaphores[currentImageIndex]; }
		IImage* GetCurrentImage() override { return swapChainImages[currentImageIndex]; }
		EImageFormat GetSwapChainImageFormat() override { return swapChainImageFormat; };
		VkSwapchainKHR GetHandle() const { return swapChain; };

		VulkanSwapChain& API_Vulkan() override { return (*this); }

	private:

		void CreateSyncObjects(IDevice* device, uint32_t imageCount);
		void CreateVulkanImages(VulkanDevice* device, uint32_t imageCount);
		void DestroyResources(IDevice* device);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		Extent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Extent2D windowExtent);
		uint32_t ChooseSwapMinImageCount(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

		VkSwapchainKHR swapChain;

		VkSurfaceFormatKHR swapChainSurfaceFormat;

		EImageFormat swapChainImageFormat;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		uint32_t currentFrame = 0;
		uint32_t currentImageIndex = 0;
	};
}