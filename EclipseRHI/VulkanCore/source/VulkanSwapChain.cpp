#define NOMINMAX

#include <cstdint>
#include <limits>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "volk.h"
#include <vulkan/vulkan.h>

#include "VulkanSwapChain.h"
#include "VulkanImageTypes.h"

VkSurfaceFormatKHR EnigmaRHI::VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	const auto formatIt = std::ranges::find_if(
		availableFormats,
		[](const auto& format) { return format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; });
	return formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
}

VkPresentModeKHR EnigmaRHI::VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	assert(std::ranges::any_of(availablePresentModes, [](auto presentMode) { return presentMode == VK_PRESENT_MODE_FIFO_KHR; }));
	return std::ranges::any_of(availablePresentModes,
		[](const VkPresentModeKHR value) { return VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR == value; }) ?
		 VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR :
		 VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
}

EnigmaRHI::Extent2D EnigmaRHI::VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Extent2D windowExtent)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return { capabilities.currentExtent.width, capabilities.currentExtent.height };
	}

	windowExtent.width = std::clamp(windowExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	windowExtent.height = std::clamp(windowExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	
	return windowExtent;
}

uint32_t EnigmaRHI::VulkanSwapChain::ChooseSwapMinImageCount(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
	if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount))
	{
		minImageCount = surfaceCapabilities.maxImageCount;
	}
	return minImageCount;
}

void EnigmaRHI::VulkanSwapChain::Create(IDevice* device, ISurface* surface, ICommandPool* commandPool, Extent2D windowExtent)
{
	SwapChainSupportDetails swapChainSupport = VulkanPhysicalDevice::QuerySwapChainSupport(device->API_Vulkan().GetPhysicalDevice().Handle(), surface->API_Vulkan().GetSurface());

	swapChainSurfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	swapChainExtent = ChooseSwapExtent(swapChainSupport.capabilities, windowExtent);
	uint32_t imageCount = ChooseSwapMinImageCount(swapChainSupport.capabilities);

	VkSwapchainCreateInfoKHR createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface->API_Vulkan().GetSurface(),
		.minImageCount = imageCount,
		.imageFormat = swapChainSurfaceFormat.format,
		.imageColorSpace = swapChainSurfaceFormat.colorSpace,
		.imageExtent = {swapChainExtent.width, swapChainExtent.height},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	};

	swapChainImageFormat = Utilities::FormatFromVulkan(swapChainSurfaceFormat.format);

	uint32_t queueFamilyIndices[] = { device->API_Vulkan().GetGraphicsQueue().FamilyIndex(), device->API_Vulkan().GetPresentQueue().FamilyIndex() };

	if (queueFamilyIndices[0] != queueFamilyIndices[1])
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
	createInfo.oldSwapchain = nullptr;

	if (vkCreateSwapchainKHR(device->API_Vulkan().GetDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		throw std::runtime_error("failed to create swap chain!");

	/*swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device->API_Vulkan().GetDevice(), swapChain, &imageCount, swapChainImages.data());*/

	CreateVulkanImages(&device->API_Vulkan(), imageCount);
	CreateSwapChainDepthResources(&device->API_Vulkan(), &commandPool->API_Vulkan());
}

void EnigmaRHI::VulkanSwapChain::Destroy(IDevice* device)
{
	vkDestroyImageView(device->API_Vulkan().GetDevice(), depthImageView, nullptr);
	vkDestroyImage(device->API_Vulkan().GetDevice(), depthImage, nullptr);
	vkFreeMemory(device->API_Vulkan().GetDevice(), depthImageMemory, nullptr);

	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device->API_Vulkan().GetDevice(), framebuffer, nullptr);
	}

	for (auto& image : swapChainVulkanImages)
	{
		image.Destroy(&device->API_Vulkan());
	}

	swapChainVulkanImages.clear();

	vkDestroySwapchainKHR(device->API_Vulkan().GetDevice(), swapChain, nullptr);
}

void EnigmaRHI::VulkanSwapChain::RecreateSwapChain(VulkanDevice* device, VulkanCommandPool* commandPool, VulkanSurface* surface, VkRenderPass renderPass, GLFWwindow* window)
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

	Create(device, surface, commandPool, Extent2D{ (unsigned int)width, (unsigned int)height });
	CreateSwapChainFramebuffers(device, renderPass);
}

void EnigmaRHI::VulkanSwapChain::CreateVulkanImages(VulkanDevice* device, uint32_t imageCount)
{
	assert(swapChainVulkanImages.empty());

	VkDevice vkDevice = device->API_Vulkan().GetDevice();

	vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, nullptr);
	std::vector<VkImage> images(imageCount);
	vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, images.data());

	swapChainVulkanImages.resize(imageCount);

	for (uint32_t i = 0; i < imageCount; i++)
	{
		swapChainVulkanImages[i].CreateFromExisting(
			device,
			images[i],
			swapChainSurfaceFormat.format,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
	}
}

void EnigmaRHI::VulkanSwapChain::CreateSwapChainFramebuffers(VulkanDevice* device, VkRenderPass renderPass)
{
	swapChainFramebuffers.resize(swapChainVulkanImages.size());

	for (size_t i = 0; i < swapChainVulkanImages.size(); i++)
	{
		VkImageView attachments[] = {
			swapChainVulkanImages[i].GetImageView(),
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

void EnigmaRHI::VulkanSwapChain::CreateSwapChainDepthResources(VulkanDevice* device, VulkanCommandPool* commandPool)
{
	VulkanImage temp;

	temp.Create(device, swapChainExtent.width, swapChainExtent.height, device->FindDepthFormat());
	temp.CreateView(device, Utilities::ToVulkanFormat(device->FindDepthFormat()), VK_IMAGE_ASPECT_DEPTH_BIT);

	depthImage = temp.GetImage();
	depthImageView = temp.GetImageView();
	depthImageMemory = temp.GetMemory();
}
