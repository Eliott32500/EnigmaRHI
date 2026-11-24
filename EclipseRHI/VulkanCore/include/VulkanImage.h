#pragma once

#include "VulkanBuffer.h"
#include "IImage.h"

namespace EnigmaRHI
{
    class VulkanImage : public IImage
    {
    public:

        void Create(IDevice* device, ImageDesc& imageDesc) override;
        void UploadData(IDevice* device, VulkanCommandPool* commandPool, const ImageDesc& desc, const void* data) override;
        void Destroy(IDevice* device) override;

        void CreateFromExisting(VulkanDevice* device, VkImage existingImage, VkFormat format, VkImageAspectFlags aspectFlags);
        void CreateView(VulkanDevice* device, VkFormat format, VkImageAspectFlags aspect);
        void RecordTransition(VkCommandBuffer cmd, VkImageLayout newLayout);
        VkImageLayout ResolveDepthAttachmentLayout(EnigmaRHI::EImageLayout requested) const;

        VkImage GetImage() const { return image; }
        VkImageView GetImageView() const { return view; }
        VkSampler GetSampler() const { return sampler; }
        VkImageLayout GetImageLayout() const { return imageLayout; };
        VkImageAspectFlags GetAspectMask() const { return aspectMask; };

        VulkanImage& API_Vulkan() override { return (*this); }

    private:
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bool ownsImage = false;

        void CreateView(VulkanDevice* device, VkImageAspectFlags aspect);
        void CreateSampler(VulkanDevice* device, ImageSampler imageSampler);
        void AllocateImageMemory(const VulkanDevice& device, const VkImage& image);
        void TransitionLayout(VulkanDevice* device, VulkanCommandPool* commandPool, VkImageLayout newLayout);
        void CopyFromBuffer(VulkanDevice* device, VulkanCommandPool* cmdPool, VkBuffer buffer, Extent3D extent);
        void GenerateMipmaps(VulkanDevice* device, VulkanCommandPool* commandPool, const ImageDesc& desc);
    };
}