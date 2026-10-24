#pragma once

#include "VulkanBuffer.h"
#include "IImage.h"

namespace EnigmaRHI
{
    class VulkanImage : public IImage
    {
    public:

        void Create(IDevice* device, ImageDesc imageDesc) override;
        void CreateTextureImage(IDevice* device, ICommandPool* commandPool, ImageDesc imageDesc) override;
        void CreateFromExisting(VulkanDevice* device, VkImage existingImage, VkFormat format, VkImageAspectFlags aspectFlags);
        void CreateView(VulkanDevice* device, VkFormat format, VkImageAspectFlags aspect);

        VkImage GetImage() const { return image; }
        VkImageView GetImageView() const { return view; }
        VkDeviceMemory GetMemory() const { return memory; }
        VkSampler GetSampler() const { return sampler; }
        VkImageLayout GetImageLayout() const { return imageLayout; }

        void Destroy(IDevice* device) override;

        VulkanImage& API_Vulkan() override { return (*this); }

    private:
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        bool ownsImage = false;

        void CreateView(VulkanDevice* device, ImageDesc imageDesc, VkImageAspectFlags aspect);
        void CreateSampler(VulkanDevice* device, ImageSampler imageSampler);
        void AllocateImageMemory(const VulkanDevice& device, const VkImage& image);
        void TransitionLayout(VulkanDevice* device, VulkanCommandPool* cmdPool, EImageFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyFromBuffer(VulkanDevice* device, VulkanCommandPool* cmdPool, VkBuffer buffer, Extent3D extent);
    };
}