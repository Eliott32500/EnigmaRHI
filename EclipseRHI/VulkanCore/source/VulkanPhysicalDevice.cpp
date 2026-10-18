#include "VulkanPhysicalDevice.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "RHILog.h"
#include <map>
#include <set>
#include <string>

namespace EnigmaRHI
{
    const std::vector<const char*> VulkanPhysicalDevice::requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void VulkanPhysicalDevice::Pick(VulkanInstance* instance, VulkanSurface* surface)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, nullptr);
        if (deviceCount == 0)
            LOG_FATAL(LogCategory::Vulkan, "Failed to find GPUs with Vulkan support !");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance->GetInstance(), &deviceCount, devices.data());

        std::multimap<int, VkPhysicalDevice> candidates;
        for (VkPhysicalDevice device : devices)
            candidates.emplace(RateDeviceSuitability(device, surface->GetSurface()), device);

        if (candidates.rbegin()->first <= 0)
            LOG_FATAL(LogCategory::Vulkan, "Failed to find a suitable GPU !");

        handle = candidates.rbegin()->second;

        vkGetPhysicalDeviceProperties(handle, &properties);
        LOG_INFO(LogCategory::RHI, "Selected GPU : " + std::string(properties.deviceName));

        vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
        queueFamilies = FindQueueFamilies(handle, surface->GetSurface());
    }

    int VulkanPhysicalDevice::RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        if (!IsDeviceSuitable(device, surface))
            return 0;

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);

        int score = 0;
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            score += 1000;

        score += static_cast<int>(props.limits.maxImageDimension2D);

        return score;
    }

    bool VulkanPhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device, surface);

        bool extensionsSupported = CheckDeviceExtensionSupport(device);
        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails support = QuerySwapChainSupport(device, surface);
            swapChainAdequate = !support.formats.empty() && !support.presentModes.empty();
        }

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);

        if (props.apiVersion < VK_API_VERSION_1_3) 
            return false;

        DeviceFeatures requiredFeatures{};

        return indices.isComplete() && extensionsSupported && swapChainAdequate
            && SupportsRequiredFeatures(device, requiredFeatures);
    }

    bool VulkanPhysicalDevice::SupportsRequiredFeatures(
        VkPhysicalDevice device,
        const DeviceFeatures& required)
    {
        VkPhysicalDeviceVulkan11Features f11
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        };

        VkPhysicalDeviceVulkan12Features f12
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
            .pNext = &f11,
        };

        VkPhysicalDeviceVulkan13Features f13
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext = &f12
        };

        VkPhysicalDeviceFeatures2 f2{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &f13
        };

        vkGetPhysicalDeviceFeatures2(device, &f2);

        return (!required.samplerAnisotropy || f2.features.samplerAnisotropy)
            && (!required.dynamicRendering || f13.dynamicRendering)
            && (!required.synchronization2 || f13.synchronization2);
    }

    bool VulkanPhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t count = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
        std::vector<VkExtensionProperties> available(count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &count, available.data());

        std::set<std::string> required(requiredDeviceExtensions.begin(), requiredDeviceExtensions.end());
        for (const auto& ext : available)
            required.erase(ext.extensionName);
        return required.empty();
    }

    QueueFamilyIndices VulkanPhysicalDevice::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        uint32_t count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
        std::vector<VkQueueFamilyProperties> families(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.data());

        for (uint32_t i = 0; i < count; i++)
        {
            const bool graphics = families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
            VkBool32 present = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present);

            if (graphics && present)
            {
                indices.graphicsFamily = i;
                indices.presentFamily = i;
                break;
            }

            if (graphics && !indices.graphicsFamily) 
                indices.graphicsFamily = i;
            if (present && !indices.presentFamily)  
                indices.presentFamily = i;
        }
        return indices;
    }

    uint32_t VulkanPhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props) const
    {
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1u << i)) &&
                (memoryProperties.memoryTypes[i].propertyFlags & props) == props)
                return i;
        }
        LOG_FATAL(LogCategory::Vulkan, "Failed to find suitable memory type!");
        return 0;
    }

    EImageFormat VulkanPhysicalDevice::FindDepthFormat() const
    {
        return FindSupportedFormat(
            { EImageFormat::D32_SFLOAT, EImageFormat::D32_SFLOAT_S8_UINT, EImageFormat::D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    EImageFormat VulkanPhysicalDevice::FindSupportedFormat(const std::vector<EImageFormat>& candidates,
        VkImageTiling tiling, VkFormatFeatureFlags features) const
    {
        for (EImageFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(handle, Utilities::ToVulkanFormat(format), &props);

            const VkFormatFeatureFlags supported = (tiling == VK_IMAGE_TILING_LINEAR)
                ? props.linearTilingFeatures : props.optimalTilingFeatures;
            if ((supported & features) == features)
                return format;
        }
        LOG_FATAL(LogCategory::Vulkan, "Failed to find supported format!");
        return candidates.front();
    }

    bool VulkanPhysicalDevice::HasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    EnigmaRHI::SwapChainSupportDetails VulkanPhysicalDevice::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
        if (count != 0)
        {
            details.formats.resize(count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, details.formats.data());
        }

        count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
        if (count != 0)
        {
            details.presentModes.resize(count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, details.presentModes.data());
        }
        return details;
    }

}