#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanImage.h"
#include "VulkanDeviceCapabilities.h"
#include "VulkanFence.h"
#include "VulkanBindingLayout.h"
#include "VulkanBindingSet.h"
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
        vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);

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
            LOG_FATAL(ELogCategory::VULKAN, "Failed to create logical device !");

        capabilities.features = required;

        volkLoadDevice(logicalDevice);

        CreateDescriptorPool();

        graphicsQueue.Init(GetHandle(), indices.graphicsFamily.value(), ECommandQueueType::GRAPHICS);
        presentQueue.Init(GetHandle(), indices.presentFamily.value(), ECommandQueueType::GRAPHICS);
    }

    IBuffer* VulkanDevice::CreateBuffer(const BufferDesc& desc)
    {
        IBuffer* buffer = new VulkanBuffer();
        buffer->Create(this, desc);
        return buffer;
    }

    IBuffer* VulkanDevice::CreateBufferWithData(ICommandPool* commandPool, const BufferDesc& desc, const void* data)
    {
        BufferDesc stagingDesc{ .size = desc.size, .usage = EBufferUsage::TRANSFER_SRC, .access = EMemoryAccess::CPU_TO_GPU };
        IBuffer* staging = CreateBuffer(stagingDesc);
        staging->Write(this, data, desc.size);

        BufferDesc dstDesc = desc;
        dstDesc.usage = desc.usage | EBufferUsage::TRANSFER_DST;
        dstDesc.access = EMemoryAccess::GPU_ONLY;
        IBuffer* dst = CreateBuffer(dstDesc);

        staging->API_Vulkan().CopyTo(this, &commandPool->API_Vulkan(), dst->API_Vulkan(), desc.size);

        DeleteBuffer(staging);
        return dst;
    }

    void VulkanDevice::DeleteBuffer(IBuffer* buffer)
    {
        buffer->Destroy(this);
        delete buffer;
    }

    IImage* VulkanDevice::CreateImage(ImageDesc& imageDesc)
    {
        IImage* image = new VulkanImage();
        image->Create(this, imageDesc);
        return image;
    }

    IImage* VulkanDevice::CreateImageWithData(ICommandPool* commandPool, const ImageDesc& desc, const void* data)
    {
        ImageDesc d = desc;
        d.usage |= EImageUsage::SAMPLED | EImageUsage::TRANSFER_DST;

        VulkanImage* image = new VulkanImage();
        image->Create(this, d);
        image->UploadData(this, &commandPool->API_Vulkan(), d, data);
        return image;
    }

    void VulkanDevice::DeleteImage(IImage* image) 
    {
        image->Destroy(this);
        delete image; 
    }

    IFence* VulkanDevice::CreateFence(bool signaled)
    {
        IFence* fence = new VulkanFence();
        fence->Create(this, signaled);
        return fence;
    }

    void VulkanDevice::DeleteFence(IFence* fence) 
    {
        fence->Destroy(this);
        delete fence; 
    }

    void VulkanDevice::CreateDescriptorPool()
    {
        VkDescriptorPoolSize sizes[] = {
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1024 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,          256 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024 },
        };

        VkDescriptorPoolCreateInfo info
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets = 1024,
            .poolSizeCount = static_cast<uint32_t>(std::size(sizes)),
            .pPoolSizes = sizes,
        };

        if (vkCreateDescriptorPool(logicalDevice, &info, nullptr, &descriptorPool) != VK_SUCCESS)
            LOG_FATAL(ELogCategory::VULKAN, "Failed to create descriptor pool !");
    }

    IBindingLayout* VulkanDevice::CreateBindingLayout(const BindingLayoutDesc& desc)
    {
        IBindingLayout* l = new VulkanBindingLayout();
        l->Create(this, desc);
        return l;
    }
    void VulkanDevice::DeleteBindingLayout(IBindingLayout* l) { l->Destroy(this); delete l; }

    IBindingSet* VulkanDevice::CreateBindingSet(IBindingLayout* layout, const BindingSetDesc& desc)
    {
        IBindingSet* s = new VulkanBindingSet();
        s->Create(this, layout, desc);
        return s;
    }

    void VulkanDevice::DeleteBindingSet(IBindingSet* s) 
    { 
        s->Destroy(this);
        delete s; 
    }

    IQueue& VulkanDevice::GetQueue(ECommandQueueType type)
    {
        switch (type)
        {
        case ECommandQueueType::GRAPHICS:
        default:
            return graphicsQueue;
        }
    }

    void VulkanDevice::WaitIdle() 
    { 
        vkDeviceWaitIdle(logicalDevice); 
    }
}