#pragma once
#include "volk.h"
#include "VulkanImageTypes.h"
#include "DeviceCapabilities.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace EnigmaRHI
{
    class VulkanInstance;
    class VulkanSurface;

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanPhysicalDevice
    {
    public:
        void Pick(VulkanInstance* instance, VulkanSurface* surface);

        VkPhysicalDevice Handle() const { return handle; }
        const QueueFamilyIndices& Queues() const { return queueFamilies; }

        VkPhysicalDeviceLimits GetLimits() const { return properties.limits; }

        uint32_t     FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        EImageFormat FindDepthFormat() const;
        EImageFormat FindSupportedFormat(const std::vector<EImageFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
        static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

        static const std::vector<const char*> requiredDeviceExtensions;

    private:
        int  RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        bool SupportsRequiredFeatures(VkPhysicalDevice device, const DeviceFeatures& required);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);


        VkPhysicalDevice                 handle = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties       properties{};
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        QueueFamilyIndices               queueFamilies;
    };
}