#pragma once
#include "volk.h"
#include "VulkanImageTypes.h"

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

    inline const std::vector<const char*> kRequiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    class VulkanPhysicalDevice
    {
    public:
        void Pick(VulkanInstance* instance, VulkanSurface* surface);

        VkPhysicalDevice Handle() const { return handle; }
        const QueueFamilyIndices& Queues() const { return queueFamilies; }

        uint32_t     FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        EImageFormat FindDepthFormat() const;
        EImageFormat FindSupportedFormat(const std::vector<EImageFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

        static bool HasStencilComponent(VkFormat format);
        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    private:
        static int  RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        VkPhysicalDevice                 handle = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties       properties{};
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        QueueFamilyIndices               queueFamilies;
    };
}