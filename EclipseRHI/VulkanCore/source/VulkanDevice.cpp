#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanDeviceCapabilities.h"
#include "RHILog.h"
#include <set>

namespace EnigmaRHI
{
    void VulkanDevice::Create(IInstance* instance, ISurface* surface)
    {
        physicalVulkanDevice.Pick(&instance->API_Vulkan(), &surface->API_Vulkan());
        CreateLogicalDevice();

        capabilities.limits = Utilities::ToDeviceLimits(physicalVulkanDevice.GetLimits());
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

        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT DynamicStateFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
            .pNext = nullptr,
            .extendedDynamicState = VK_TRUE,
        };

        const DeviceFeatures required = GetRequiredFeatures();

        VkPhysicalDeviceVulkan13Features f13
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .synchronization2 = required.synchronization2 ? VK_TRUE : VK_FALSE,
            .dynamicRendering = required.dynamicRendering ? VK_TRUE : VK_FALSE,
        };

        VkPhysicalDeviceVulkan12Features f12
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
            .pNext = &f13,
            .timelineSemaphore = required.timelineSemaphore ? VK_TRUE : VK_FALSE,
        };

        VkPhysicalDeviceFeatures2 features2
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &f12,
            .features = {.samplerAnisotropy = required.samplerAnisotropy ? VK_TRUE : VK_FALSE },
        };

        VkDeviceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		    .pNext = &features2,
            .queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size()),
            .pQueueCreateInfos = queueInfos.data(),
            .enabledExtensionCount = static_cast<uint32_t>(physicalVulkanDevice.requiredDeviceExtensions.size()),
            .ppEnabledExtensionNames = physicalVulkanDevice.requiredDeviceExtensions.data(),
            .pEnabledFeatures = nullptr,
        };

        if (vkCreateDevice(physicalVulkanDevice.Handle(), &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
            LOG_FATAL(LogCategory::Vulkan, "Failed to create logical device !");

        capabilities.features = required;

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

    IImage* VulkanDevice::CreateImage(ImageDesc imageDesc)
    {
        IImage* image = new VulkanImage();
        image->Create(this, imageDesc);
        return image;
    }

    IImage* VulkanDevice::CreateImageTexture(ICommandPool* commandPool, ImageDesc imageDesc)
    {
        IImage* image = new VulkanImage();
        image->CreateTextureImage(this, commandPool, imageDesc);
        return image;
    }

    void VulkanDevice::DeleteBuffer(IBuffer* buffer) { delete buffer; }
    void VulkanDevice::DeleteImage(IImage* image) { delete image; }

    void VulkanDevice::WaitIdle() { vkDeviceWaitIdle(logicalDevice); }
}