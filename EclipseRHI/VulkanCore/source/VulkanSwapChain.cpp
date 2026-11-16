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
#include "RHIConfig.h"
#include "RHILog.h"

namespace
{
	VkSemaphore CreateBinarySemaphore(VkDevice device)
	{
		VkSemaphoreCreateInfo info{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		VkSemaphore semaphore = VK_NULL_HANDLE;
		if (vkCreateSemaphore(device, &info, nullptr, &semaphore) != VK_SUCCESS)
			LOG_ERROR(ELogCategory::VULKAN, "Failed to create semaphore !");

		return semaphore;
	}
}

void EnigmaRHI::VulkanSwapChain::DestroyResources(IDevice* device)
{
	VkDevice vkDevice = device->API_Vulkan().GetHandle();

	for (VkSemaphore s : renderFinishedSemaphores)
		vkDestroySemaphore(vkDevice, s, nullptr);
	renderFinishedSemaphores.clear();

	for (auto* image : swapChainImages)
	{
		image->Destroy(&device->API_Vulkan());
		delete image;
	}
	swapChainImages.clear();
}

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

void EnigmaRHI::VulkanSwapChain::Create(IDevice* device, ISurface* surface, Extent2D windowExtent)
{
	SwapChainSupportDetails swapChainSupport = VulkanPhysicalDevice::QuerySwapChainSupport(device->API_Vulkan().GetPhysicalDevice().Handle(), surface->API_Vulkan().GetHandle());

	swapChainSurfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	swapChainExtent = ChooseSwapExtent(swapChainSupport.capabilities, windowExtent);
	uint32_t imageCount = ChooseSwapMinImageCount(swapChainSupport.capabilities);

	VkSwapchainCreateInfoKHR createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface->API_Vulkan().GetHandle(),
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

	if (vkCreateSwapchainKHR(device->API_Vulkan().GetHandle(), &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create swap chain !");

	CreateVulkanImages(&device->API_Vulkan(), imageCount);
	CreateSyncObjects(device, imageCount);
}

void EnigmaRHI::VulkanSwapChain::Destroy(IDevice* device)
{
	VkDevice vkDevice = device->API_Vulkan().GetHandle();

	DestroyResources(device);

	for (VkSemaphore s : imageAvailableSemaphores)
	{
		vkDestroySemaphore(vkDevice, s, nullptr);
	}

	imageAvailableSemaphores.clear();

	vkDestroySwapchainKHR(vkDevice, swapChain, nullptr);
}

void EnigmaRHI::VulkanSwapChain::Resize(IDevice* device, ISurface* surface, Extent2D newExtent)
{
	device->WaitIdle();

	DestroyResources(device);
	vkDestroySwapchainKHR(device->API_Vulkan().GetHandle(), swapChain, nullptr);

	Create(device, surface, newExtent);
}

void EnigmaRHI::VulkanSwapChain::CreateVulkanImages(VulkanDevice* device, uint32_t imageCount)
{
	assert(swapChainImages.empty());

	VkDevice vkDevice = device->API_Vulkan().GetHandle();

	vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, nullptr);
	std::vector<VkImage> images(imageCount);
	vkGetSwapchainImagesKHR(vkDevice, swapChain, &imageCount, images.data());

	swapChainImages.resize(imageCount);

	for (uint32_t i = 0; i < imageCount; i++)
	{
		swapChainImages[i] = new VulkanImage();
		swapChainImages[i]->API_Vulkan().CreateFromExisting(device, images[i], swapChainSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

EnigmaRHI::EAcquireResult EnigmaRHI::VulkanSwapChain::Acquire(IDevice* device, uint32_t frame)
{
	currentFrame = frame;

	VkResult result = vkAcquireNextImageKHR(device->API_Vulkan().GetHandle(), swapChain, UINT64_MAX, imageAvailableSemaphores[frame], VK_NULL_HANDLE, &currentImageIndex);

	switch (result)
	{
	case VK_SUCCESS:
		return EAcquireResult::SUCCESS;
	case VK_SUBOPTIMAL_KHR:
		return EAcquireResult::SUBOPTIMAL;
	case VK_ERROR_OUT_OF_DATE_KHR:
		return EAcquireResult::OUT_OF_DATE;
	default:
		LOG_FATAL(ELogCategory::VULKAN, std::format("Failed to acquire swap chain image ! (VkResult {})", static_cast<int>(result)));
	}
}

EnigmaRHI::EPresentResult EnigmaRHI::VulkanSwapChain::Present(IDevice* device, IQueue* queue)
{
	VkResult result = queue->API_Vulkan().Present(swapChain, currentImageIndex, renderFinishedSemaphores[currentImageIndex]);

	switch (result)
	{
	case VK_SUCCESS:
		return EPresentResult::SUCCESS;
	case VK_SUBOPTIMAL_KHR:
		return EPresentResult::SUBOPTIMAL;
	case VK_ERROR_OUT_OF_DATE_KHR:
		return EPresentResult::OUT_OF_DATE;
	default:
		LOG_FATAL(ELogCategory::VULKAN, std::format("Failed to present swap chain image ! (VkResult {})", static_cast<int>(result)));
	}
}

void EnigmaRHI::VulkanSwapChain::CreateSyncObjects(IDevice* device, uint32_t imageCount)
{
	VkDevice vkDevice = device->API_Vulkan().GetHandle();

	if (imageAvailableSemaphores.empty())
	{
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		for (auto& s : imageAvailableSemaphores)
		{
			s = CreateBinarySemaphore(vkDevice);
		}
	}

	renderFinishedSemaphores.resize(imageCount);
	for (auto& s : renderFinishedSemaphores)
	{
		s = CreateBinarySemaphore(vkDevice);
	}
}
