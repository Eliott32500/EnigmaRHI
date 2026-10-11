#pragma once
#include "VulkanPhysicalDevice.h"
#include "IDevice.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanImageTypes.h"
#include "VulkanQueue.h"
#include "volk.h"

namespace EnigmaRHI
{
    class VulkanDevice : public IDevice
    {
    public:
        VulkanDevice() = default;

        void Create(IInstance* instance, ISurface* surface) override;
        void Destroy() override;
        void DeviceWaitIdle() override;

        IBuffer* CreateBuffer(size_t size, uint32_t usage, size_t properties) override;
        IImage* CreateImage(uint32_t width, uint32_t height, EImageFormat format, bool isTexture = false) override;
        void DeleteBuffer(IBuffer* buffer) override;
        void DeleteImage(IImage* image) override;

        EImageFormat FindDepthFormat() override { return physicalVulkanDevice.FindDepthFormat(); }

        VulkanDevice& API_Vulkan() override { return *this; }

        // Vulkan access
        VkDevice GetDevice() const { return logicalDevice; }
        VulkanPhysicalDevice GetPhysicalDevice() const { return physicalVulkanDevice; }
        VulkanQueue& GetGraphicsQueue() { return graphicsQueue; }
        VulkanQueue& GetPresentQueue() { return presentQueue; }

    private:
        void CreateLogicalDevice();

        VkDevice             logicalDevice = VK_NULL_HANDLE;
        VulkanPhysicalDevice physicalVulkanDevice;
        VulkanQueue          graphicsQueue;
        VulkanQueue          presentQueue;
    };
}