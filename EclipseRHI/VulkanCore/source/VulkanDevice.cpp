#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "RHILog.h"
#include <set>

namespace EnigmaRHI
{
    void VulkanDevice::Create(IInstance* instance, ISurface* surface)
    {
        physicalVulkanDevice.Pick(&instance->API_Vulkan(), &surface->API_Vulkan());
        CreateLogicalDevice();
    }

    void VulkanDevice::Destroy()
    {
        if (logicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(logicalDevice, nullptr);
            logicalDevice = VK_NULL_HANDLE;
        }
    }

    void VulkanDevice::CreateLogicalDevice()
    {
        const QueueFamilyIndices& indices = physicalVulkanDevice.Queues();

        std::set<uint32_t> uniqueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
        std::vector<VkDeviceQueueCreateInfo> queueInfos;
        float priority = 1.0f;
        for (uint32_t family : uniqueFamilies)
        {
            VkDeviceQueueCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            info.queueFamilyIndex = family;
            info.queueCount = 1;
            info.pQueuePriorities = &priority;
            queueInfos.push_back(info);
        }

        VkPhysicalDeviceVulkan13Features f13
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .synchronization2 = VK_TRUE,
            .dynamicRendering = VK_TRUE,
        };
        VkPhysicalDeviceVulkan12Features f12
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
            .pNext = &f13,
            .timelineSemaphore = VK_TRUE,
        };
        VkPhysicalDeviceFeatures2 features2
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &f12,
            .features = {.samplerAnisotropy = VK_TRUE },
        };

        VkDeviceCreateInfo createInfo{};
		createInfo.pNext = &features2;
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
        createInfo.pQueueCreateInfos = queueInfos.data();
        createInfo.pEnabledFeatures = nullptr;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(kRequiredDeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = kRequiredDeviceExtensions.data();

        if (vkCreateDevice(physicalVulkanDevice.Handle(), &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
            LOG_FATAL(LogCategory::Vulkan, "Failed to create logical device !");

        volkLoadDevice(logicalDevice);

        graphicsQueue.Init(logicalDevice, indices.graphicsFamily.value());
        presentQueue.Init(logicalDevice, indices.presentFamily.value());
    }

    IBuffer* VulkanDevice::CreateBuffer(size_t size, uint32_t usage, size_t properties)
    {
        IBuffer* buffer = new VulkanBuffer();
        buffer->Create(this, size, usage, properties);
        return buffer;
    }

    IImage* VulkanDevice::CreateImage(uint32_t w, uint32_t h, EImageFormat format, bool isTexture)
    {
        IImage* image = new VulkanImage();
        image->Create(this, w, h, format, isTexture);
        return image;
    }

    void VulkanDevice::DeleteBuffer(IBuffer* buffer) { delete buffer; }
    void VulkanDevice::DeleteImage(IImage* image) { delete image; }

    void VulkanDevice::DeviceWaitIdle() { vkDeviceWaitIdle(logicalDevice); }
}