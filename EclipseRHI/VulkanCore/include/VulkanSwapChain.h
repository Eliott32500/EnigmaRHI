#pragma once

#include "../../include/ISwapChain.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include <cstdint>

class VulkanSwapChain : public EclipseRHI::ISwapChain
{
public:

	void Create(EclipseRHI::IDevice* device, EclipseRHI::ISurface* surface, EclipseRHI::ICommandPool* commandPool, GLFWwindow* window) override;
	void Destroy(EclipseRHI::IDevice* device) override;

	void RecreateSwapChain(VulkanDevice* device, VulkanCommandPool* commandPool, VulkanSurface* surface, VkRenderPass renderPass, GLFWwindow* window);
	void CreateImageViews(VulkanDevice* device);
	void CreateSwapChainFramebuffers(VulkanDevice* device, VkRenderPass renderPass);
	void CreateSwapChainDepthResources(VulkanDevice* device, VulkanCommandPool* commandPool);

	VkExtent2D GetSwapChainExtent() const { return swapChainExtent; };

	EclipseRHI::ImageFormat GetSwapChainImageFormat() override { return swapChainImageFormat; };

	VkSwapchainKHR GetSwapChain() const { return swapChain; };
	std::vector<VkFramebuffer> GetSwapChainFramebuffers() const { return swapChainFramebuffers; };

	VulkanSwapChain& API_Vulkan() override { return (*this); }

private:

	VkImage depthImage;
	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;

	VkSwapchainKHR swapChain;

	EclipseRHI::ImageFormat swapChainImageFormat;

	VkExtent2D swapChainExtent;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};