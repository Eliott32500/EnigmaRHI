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
        void WaitIdle() override;

        IBuffer* CreateBuffer(const BufferDesc& desc) override;
        IBuffer* CreateBufferWithData(ICommandPool* commandPool, const BufferDesc& desc, const void* data);
        void DeleteBuffer(IBuffer* buffer) override;
        IImage* CreateImage(ImageDesc& desc) override;
        IImage* CreateImageWithData(ICommandPool* commandPool, const ImageDesc& desc, const void* data) override;
        void DeleteImage(IImage* image) override;
        IFence* CreateFence(bool signaled = false) override;
        void DeleteFence(IFence* fence) override;
        IBindingLayout* CreateBindingLayout(const BindingLayoutDesc& desc) override;
        void DeleteBindingLayout(IBindingLayout* layout) override;
        IBindingSet* CreateBindingSet(IBindingLayout* layout, const BindingSetDesc& desc) override;
        void DeleteBindingSet(IBindingSet* set) override;
        VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }

        EImageFormat FindDepthFormat() override { return physicalVulkanDevice.FindDepthFormat(); }

        IQueue& GetQueue(ECommandQueueType type) override;

        VulkanDevice& API_Vulkan() override { return *this; }

        // Vulkan access
        VkDevice GetHandle() const { return logicalDevice; }
        const VulkanPhysicalDevice& GetPhysicalDevice() const { return physicalVulkanDevice; }
        VulkanQueue& GetGraphicsQueue() { return graphicsQueue; }
        VulkanQueue& GetPresentQueue() { return presentQueue; }

    private:

        void CreateDescriptorPool();
        void CreateLogicalDevice();

        VkDevice logicalDevice = VK_NULL_HANDLE;
        VulkanPhysicalDevice physicalVulkanDevice;
        VulkanQueue graphicsQueue;
        VulkanQueue presentQueue;
        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    };
}