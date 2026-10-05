#pragma once

#include "IDevice.h"
#include "IFormat.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanUtilities.h"

#include <vulkan/vulkan.h>
#include "volk.h"

#include <stdlib.h>
#include <iostream>
#include <optional>
#include <vector>
#include <map>
#include <set>

namespace EnigmaRHI
{

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
}

namespace EnigmaRHI
{
    class VulkanDevice : public IDevice
    {
    public:

        VulkanDevice() = default;

        void Create(IInstance* instance, ISurface* surface) override;
        void Destroy() override;

        IBuffer* CreateBuffer(size_t size, uint32_t usage, size_t properties) override;
        IImage* CreateImage(uint32_t width, uint32_t height, ImageFormat format, bool isTexture = false) override;
        void DeleteBuffer(IBuffer* buffer) override;
        void DeleteImage(IImage* image) override;

        void CreateLogicalDevice(VulkanSurface* surface);

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) const;

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        ImageFormat FindDepthFormat() override;
        ImageFormat FindSupportedFormat(const std::vector<ImageFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        bool HasStencilComponent(VkFormat format);

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const;

        VulkanDevice& API_Vulkan() override { return (*this); }

        void DeviceWaitIdle() override;

        VkDevice GetDevice() const { return logicalDevice; }
        VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
        VkQueue GetGraphicsQueue() const { return graphicsQueue; }
        VkQueue GetPresQueue() const { return presentationQueue; }


    private:

        int RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
        void PickPhysicalDevice(VulkanInstance* instance, VulkanSurface* surface);
        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice = VK_NULL_HANDLE;
        VkQueue graphicsQueue;
        VkQueue presentationQueue;
        std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    };
}