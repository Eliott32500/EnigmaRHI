#pragma once

#include "VulkanBuffer.h"
#include "IImage.h"

namespace EnigmaRHI
{
    class VulkanImage : public IImage
    {
    public:

        void Create(IDevice* device, uint32_t width, uint32_t height, EImageFormat format, bool isTexture = false) override;

        void CreateTextureImage(const void* data, IDevice* device, ICommandPool* commandPool,
            uint32_t width, uint32_t height,
            EImageFormat format
        ) override;

        void CreateView(VulkanDevice* device, VkImage image, VkFormat format, VkImageAspectFlags aspect);
        void CreateSampler(VulkanDevice* device, VkFilter filter = VK_FILTER_LINEAR);

        void TransitionLayout(VulkanDevice* device, VulkanCommandPool* cmdPool, VkImageLayout oldLayout, VkImageLayout newLayout);

        void CopyFromBuffer(VulkanDevice* device, VulkanCommandPool* cmdPool, VkBuffer buffer, uint32_t width, uint32_t height);

        VkDescriptorImageInfo GetDescriptorInfo() const { return descriptorInfo; }
        VkImage GetImage() const { return image; }
        VkFormat GetFormat() const { return format; }
        VkImageView GetImageView() const { return view; }
        VkDeviceMemory GetMemory() const { return memory; }
        VkSampler GetSampler() const { return sampler; }
        VkImageLayout GetImageLayout() const { return imageLayout; }

        void SetImage(VkImage img) { image = img; }

        void Destroy(IDevice* device) override;

        VulkanImage& API_Vulkan() override { return (*this); }

    private:
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        VkFormat format;
        VkDescriptorImageInfo descriptorInfo{};
    };
}