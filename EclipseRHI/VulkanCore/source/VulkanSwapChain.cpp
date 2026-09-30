#define NOMINMAX

#include <cstdint>
#include <limits>
#include <iostream>
#include <algorithm>

#include "volk.h"
#include <vulkan/vulkan.h>

#include "../include/VulkanSwapChain.h"
#include "../include/VulkanUtilities.h"

VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		//VK_FORMAT_B8G8R8A8_SRGB
		if (availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentMode;
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void VulkanSwapChain::Create(EnigmaRHI::IDevice* device, EnigmaRHI::ISurface* surface, EnigmaRHI::ICommandPool* commandPool, GLFWwindow* window)
{
	SwapChainSupportDetails swapChainSupport = device->API_Vulkan().QuerySwapChainSupport(device->API_Vulkan().GetPhysicalDevice(), surface->API_Vulkan().GetSurface());

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, window);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		imageCount = swapChainSupport.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface->API_Vulkan().GetSurface(),

		.minImageCount = imageCount,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	};

	swapChainImageFormat = UtilitiesVulkan::FormatFromVulkan(surfaceFormat.format);
	swapChainExtent = extent;
	width = static_cast<float>(extent.width);
	height = static_cast<float>(extent.height);

	QueueFamilyIndices indices = device->API_Vulkan().FindQueueFamilies(device->API_Vulkan().GetPhysicalDevice(), surface->API_Vulkan().GetSurface());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device->API_Vulkan().GetDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		throw std::runtime_error("failed to create swap chain!");

	vkGetSwapchainImagesKHR(device->API_Vulkan().GetDevice(), swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device->API_Vulkan().GetDevice(), swapChain, &imageCount, swapChainImages.data());

	CreateImageViews(&device->API_Vulkan());
	CreateSwapChainDepthResources(&device->API_Vulkan(), &commandPool->API_Vulkan());
}

void VulkanSwapChain::Destroy(EnigmaRHI::IDevice* device)
{
	vkDestroyImageView(device->API_Vulkan().GetDevice(), depthImageView, nullptr);
	vkDestroyImage(device->API_Vulkan().GetDevice(), depthImage, nullptr);
	vkFreeMemory(device->API_Vulkan().GetDevice(), depthImageMemory, nullptr);

	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device->API_Vulkan().GetDevice(), framebuffer, nullptr);
	}

	for (auto images : swapChainImageViews)
	{
		vkDestroyImageView(device->API_Vulkan().GetDevice(), images, nullptr);
	}

	vkDestroySwapchainKHR(device->API_Vulkan().GetDevice(), swapChain, nullptr);
}

void VulkanSwapChain::RecreateSwapChain(VulkanDevice* device, VulkanCommandPool* commandPool, VulkanSurface* surface, VkRenderPass renderPass, GLFWwindow* window)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(device->GetDevice());

	Destroy(device);

	Create(device, surface, commandPool, window);
	CreateSwapChainFramebuffers(device, renderPass);
}

void VulkanSwapChain::CreateImageViews(VulkanDevice* device)
{
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++) 
	{
		VulkanImage temp;
		temp.CreateView(device, swapChainImages[i], UtilitiesVulkan::FormatToVulkan(swapChainImageFormat), VK_IMAGE_ASPECT_COLOR_BIT);
		swapChainImageViews[i] = temp.GetImageView();
	}
}

void VulkanSwapChain::CreateSwapChainFramebuffers(VulkanDevice* device, VkRenderPass renderPass)
{
	swapChainFramebuffers.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VkImageView attachments[] = {
			swapChainImageViews[i],
			depthImageView
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 2;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device->API_Vulkan().GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void VulkanSwapChain::CreateSwapChainDepthResources(VulkanDevice* device, VulkanCommandPool* commandPool)
{
	VulkanImage temp;

	temp.Create(device, swapChainExtent.width, swapChainExtent.height, device->FindDepthFormat());
	temp.CreateView(device, temp.GetImage(), UtilitiesVulkan::FormatToVulkan(device->FindDepthFormat()), VK_IMAGE_ASPECT_DEPTH_BIT);

	depthImage = temp.GetImage();
	depthImageView = temp.GetImageView();
	depthImageMemory = temp.GetMemory();
}
