#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"

void EnigmaRHI::VulkanDevice::Create(IInstance* instance, ISurface* surface)
{
	PickPhysicalDevice(&instance->API_Vulkan(), &surface->API_Vulkan());
	CreateLogicalDevice(&surface->API_Vulkan());
}

void EnigmaRHI::VulkanDevice::Destroy()
{
	vkDestroyDevice(logicalDevice, nullptr);
}

EnigmaRHI::IBuffer* EnigmaRHI::VulkanDevice::CreateBuffer(size_t size, uint32_t usage, size_t properties)
{
	IBuffer* buffer = new VulkanBuffer();
	buffer->Create(this, size, usage, properties);

	return buffer;
}

EnigmaRHI::IImage* EnigmaRHI::VulkanDevice::CreateImage(uint32_t width, uint32_t height, ImageFormat format, bool isTexture)
{
	IImage* image = new VulkanImage();
	image->Create(this, width, height, format, isTexture);

	return image;
}

void EnigmaRHI::VulkanDevice::DeleteBuffer(IBuffer* buffer)
{
	if (buffer)
		delete buffer;
}

void EnigmaRHI::VulkanDevice::DeleteImage(IImage* image)
{
	if (image)
		delete image;
}

void EnigmaRHI::VulkanDevice::PickPhysicalDevice(VulkanInstance* instance, VulkanSurface* surface)
{
	// compte le nombre de GPU
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, devices.data());

	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices)
	{
		int score = RateDeviceSuitability(device, surface->GetSurface());
		candidates.insert(std::make_pair(score, device));
	}

	// Check if the best candidate is suitable at all
	if (candidates.rbegin()->first > 0)
		physicalDevice = candidates.rbegin()->second;
	else
		throw std::runtime_error("failed to find a suitable GPU!");
}

int EnigmaRHI::VulkanDevice::RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	int score = 0;

	if (!IsDeviceSuitable(device, surface))
		return score;

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	return score;
}

bool EnigmaRHI::VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool EnigmaRHI::VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices = FindQueueFamilies(device, surface);

	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void EnigmaRHI::VulkanDevice::CreateLogicalDevice(VulkanSurface* surface)
{
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface->GetSurface());
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device!");

	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentationQueue);
}

EnigmaRHI::QueueFamilyIndices EnigmaRHI::VulkanDevice::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
			indices.presentFamily = i;
		if (indices.isComplete())
			break;
		i++;
	}
	return indices;
}

uint32_t EnigmaRHI::VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}
	throw std::runtime_error("failed to find suitable memory type!");
}

EnigmaRHI::ImageFormat EnigmaRHI::VulkanDevice::FindDepthFormat()
{
	return FindSupportedFormat(
		{ EnigmaRHI::ImageFormat::D32_SFLOAT, EnigmaRHI::ImageFormat::D32_SFLOAT_S8_UINT, EnigmaRHI::ImageFormat::D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

EnigmaRHI::ImageFormat EnigmaRHI::VulkanDevice::FindSupportedFormat(const std::vector<EnigmaRHI::ImageFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (EnigmaRHI::ImageFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, UtilitiesVulkan::FormatToVulkan(format), &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

bool EnigmaRHI::VulkanDevice::HasStencilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

EnigmaRHI::SwapChainSupportDetails EnigmaRHI::VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

void EnigmaRHI::VulkanDevice::DeviceWaitIdle()
{
	vkDeviceWaitIdle(logicalDevice);
}
