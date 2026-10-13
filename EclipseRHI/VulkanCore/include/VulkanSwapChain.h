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

		void Create(IDevice* device, ISurface* surface, ICommandPool* commandPool, Extent2D windowExtent) override;
		void Destroy(IDevice* device) override;
		void CreateVulkanImages(VulkanDevice* device, uint32_t imageCount);

		// A modifier impérativement
		void RecreateSwapChain(VulkanDevice* device, VulkanCommandPool* commandPool, VulkanSurface* surface, VkRenderPass renderPass, GLFWwindow* window);
		//

		//A supprimer plus tard
		void CreateSwapChainFramebuffers(VulkanDevice* device, VkRenderPass renderPass);
		void CreateSwapChainDepthResources(VulkanDevice* device, VulkanCommandPool* commandPool);
		EImageFormat GetSwapChainImageFormat() override { return swapChainImageFormat; };
		std::vector<VkFramebuffer> GetSwapChainFramebuffers() const { return swapChainFramebuffers; };
		//

		VkSwapchainKHR GetSwapChain() const { return swapChain; };

		VulkanSwapChain& API_Vulkan() override { return (*this); }

	private:

		VkSwapchainKHR swapChain;

		std::vector<VulkanImage> swapChainVulkanImages;
		VkSurfaceFormatKHR swapChainSurfaceFormat;

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		Extent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Extent2D windowExtent);
		uint32_t ChooseSwapMinImageCount(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

		//Old
		VkImage depthImage;
		VkImageView depthImageView;
		VkDeviceMemory depthImageMemory;

		//A supprimer plus tard
		EImageFormat swapChainImageFormat;
		//

		std::vector<VkFramebuffer> swapChainFramebuffers;
	};
}