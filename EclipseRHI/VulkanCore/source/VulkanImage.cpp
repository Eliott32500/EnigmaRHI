#include "VulkanImage.h"
#include "RHILog.h"
#include "VulkanCommonTypes.h"
#include "VulkanDepthStencilTypes.h"
#include <assert.h>
#include <algorithm>
#include <array>

void EnigmaRHI::VulkanImage::Create(IDevice* device, ImageDesc& imageDesc)
{
	const DeviceLimits& limits = device->GetCapabilities().limits;

	uint32_t maxDim = 0;
	switch (imageDesc.type)
	{
	case EImageType::TYPE_1D: maxDim = limits.maxImageDimension1D; break;
	case EImageType::TYPE_2D: maxDim = limits.maxImageDimension2D; break;
	case EImageType::TYPE_3D: maxDim = limits.maxImageDimension3D; break;
	}

	if (imageDesc.extent.width > maxDim || imageDesc.extent.height > maxDim)
	{
		LOG_WARNING(ELogCategory::VULKAN, "Image extent exceeds device limit (" + std::to_string(maxDim) + ")");
		imageDesc.extent.width > imageDesc.extent.height ? imageDesc.extent.width = maxDim : imageDesc.extent.height = maxDim;
	}

	if (imageDesc.arrayLayers > limits.maxImageArrayLayers)
	{
		LOG_WARNING(ELogCategory::VULKAN, "arrayLayers exceeds device limit");
		imageDesc.arrayLayers = limits.maxImageArrayLayers;
	}

	const bool isDepth = Utilities::HasDepthComponent(imageDesc.format);
	const uint32_t maxSamples = isDepth ? limits.maxDepthSampleCount : limits.maxColorSampleCount;

	if (imageDesc.sampleCount > maxSamples)
	{
		LOG_WARNING(ELogCategory::VULKAN, "sampleCount clamped to " + std::to_string(maxSamples));
		imageDesc.sampleCount = maxSamples;
	}

	desc = imageDesc;

	VkImageCreateFlags flags = 0;

	if (imageDesc.viewType == EImageViewType::VIEW_CUBE || imageDesc.viewType == EImageViewType::VIEW_CUBE_ARRAY)
		flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

	VkImageCreateInfo imageInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.flags = flags,
		.imageType = Utilities::ToVulkanImageType(imageDesc.type),
		.format = Utilities::ToVulkanFormat(imageDesc.format),
		.extent = Utilities::ToVulkanExtent(imageDesc.extent),
		.mipLevels = imageDesc.mipLevels,
		.arrayLayers = imageDesc.arrayLayers,
		.samples = static_cast<VkSampleCountFlagBits>(imageDesc.sampleCount),
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = Utilities::ToVulkanImageUsage(imageDesc.usage),
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};

	imageLayout = imageInfo.initialLayout;

	VulkanDevice& myDevice = device->API_Vulkan();
	VkDevice vulkanDevice = myDevice.GetHandle();

	if (vkCreateImage(vulkanDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create image !");

	AllocateImageMemory(myDevice, image);

	ownsImage = true;

	if (isDepth)
	{
		aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (Utilities::HasStencilComponent(imageDesc.format))
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	CreateView(&myDevice, aspectMask);

	if (imageInfo.usage & VK_IMAGE_USAGE_SAMPLED_BIT)
		CreateSampler(&myDevice, imageDesc.sampler);
}


void EnigmaRHI::VulkanImage::AllocateImageMemory(const VulkanDevice& device, const VkImage& image)
{
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device.GetHandle(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = device.GetPhysicalDevice().FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
	};

	if (vkAllocateMemory(device.GetHandle(), &allocInfo, nullptr, &memory) != VK_SUCCESS)
		LOG_ERROR(ELogCategory::VULKAN, "Failed to allocate image memory!");

	vkBindImageMemory(device.GetHandle(), image, memory, 0);
}

void EnigmaRHI::VulkanImage::UploadData(IDevice* device, VulkanCommandPool* commandPool, const ImageDesc& desc, const void* data)
{
	VulkanDevice& dev = device->API_Vulkan();

	const VkDeviceSize imageSize = static_cast<VkDeviceSize>(desc.extent.width) * desc.extent.height
		* (std::max)(1u, desc.extent.depth) * (std::max)(1u, desc.arrayLayers) * Utilities::BytesPerPixel(desc.format);

	BufferDesc stagingDesc
	{
		.size = imageSize,
		.usage = EBufferUsage::TRANSFER_SRC,
		.access = EMemoryAccess::CPU_TO_GPU,
	};

	VulkanBuffer stagingBuffer;
	stagingBuffer.Create(device, stagingDesc);
	stagingBuffer.Write(device, data, imageSize);

	TransitionLayout(&dev, commandPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyFromBuffer(&dev, commandPool, stagingBuffer.GetHandle(), desc.extent);

	if (desc.mipLevels > 1)
	{
		try
		{
			GenerateMipmaps(&dev, commandPool, desc);
		}
		catch (...)
		{
			stagingBuffer.Destroy(&dev);
			throw;
		}
	}
	else
		TransitionLayout(&dev, commandPool, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stagingBuffer.Destroy(&dev);
}

void EnigmaRHI::VulkanImage::CreateFromExisting(VulkanDevice* device, VkImage existingImage, VkFormat format, VkImageAspectFlags aspectFlags)
{
	assert(image == VK_NULL_HANDLE);
	assert(existingImage != VK_NULL_HANDLE);

	image = existingImage;

	CreateView(device, format, aspectFlags);

	ownsImage = false;
}

void EnigmaRHI::VulkanImage::CreateView(VulkanDevice* device, VkFormat format, VkImageAspectFlags aspect)
{
	if (view != VK_NULL_HANDLE)
	{
		vkDestroyImageView(device->GetHandle(), view, nullptr);
		view = VK_NULL_HANDLE;
	}

	VkImageViewCreateInfo viewInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
	};

	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = desc.mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(device->GetHandle(), &viewInfo, nullptr, &view) != VK_SUCCESS)
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create image view!");
}

void EnigmaRHI::VulkanImage::CreateView(VulkanDevice* device, VkImageAspectFlags aspect)
{
	if (view != VK_NULL_HANDLE)
	{
		vkDestroyImageView(device->GetHandle(), view, nullptr);
		view = VK_NULL_HANDLE;
	}

	VkImageViewCreateInfo viewInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image,
		.viewType = Utilities::ToVulkanImageViewType(desc.viewType),
		.format = Utilities::ToVulkanFormat(desc.format),
	};

	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = desc.mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = desc.arrayLayers;

	if (vkCreateImageView(device->GetHandle(), &viewInfo, nullptr, &view) != VK_SUCCESS)
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create image view!");
}

void EnigmaRHI::VulkanImage::CreateSampler(VulkanDevice* device, ImageSampler imageSampler)
{
	if (sampler != VK_NULL_HANDLE)
	{
		vkDestroySampler(device->GetHandle(), sampler, nullptr);
		sampler = VK_NULL_HANDLE;
	}

	const float deviceMax = device->GetCapabilities().limits.maxSamplerAnisotropy;
	const float requested = imageSampler.maxAnisotropy;

	const bool  useAniso = requested > 0.f;
	const float aniso = (requested == 1.f) ? deviceMax : (std::min)(requested, deviceMax);


	VkSamplerCreateInfo samplerInfo
	{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = Utilities::ToVulkanFilter(imageSampler.magFilter),
		.minFilter = Utilities::ToVulkanFilter(imageSampler.minFilter),
		.mipmapMode = Utilities::ToVulkanMipmapMode(imageSampler.mipMapMode),

		// X
		.addressModeU = Utilities::ToVulkanWrapMode(imageSampler.wrapU),
		// Y
		.addressModeV = Utilities::ToVulkanWrapMode(imageSampler.wrapV),
		// Z
		.addressModeW = Utilities::ToVulkanWrapMode(imageSampler.wrapW),

		.mipLodBias = imageSampler.mipLodBias,

		// Anisotropic filter
		.anisotropyEnable = useAniso ? VK_TRUE : VK_FALSE,
		.maxAnisotropy = useAniso ? aniso : 1.0f,

		.compareEnable = imageSampler.compareEnable,
		.compareOp = Utilities::ToVulkanCompareOp(imageSampler.compareOp),

		.minLod = imageSampler.minLod,
		.maxLod = imageSampler.maxLod,

		// Field specifies which color is returned when sampling beyond the image with clamp to border addressing mode
		.borderColor = Utilities::ToVulkanBorderColor(imageSampler.borderColor),
		.unnormalizedCoordinates = VK_FALSE,
	};

	if (vkCreateSampler(device->GetHandle(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create texture sampler!");
}


void EnigmaRHI::VulkanImage::RecordTransition(VkCommandBuffer cmd, VkImageLayout newLayout)
{
	const Utilities::LayoutSync src = Utilities::GetLayoutSync(imageLayout, true);
	const Utilities::LayoutSync dst = Utilities::GetLayoutSync(newLayout, false);

	VkImageMemoryBarrier2 barrier
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = src.stage,
		.srcAccessMask = src.access,
		.dstStageMask = dst.stage,
		.dstAccessMask = dst.access,
		.oldLayout = imageLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange = { aspectMask, 0, desc.mipLevels, 0, desc.arrayLayers },
	};

	VkDependencyInfo dependency
	{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &barrier,
	};

	vkCmdPipelineBarrier2(cmd, &dependency);

	imageLayout = newLayout;
}

VkImageLayout EnigmaRHI::VulkanImage::ResolveDepthAttachmentLayout(EnigmaRHI::EImageLayout requested) const
{
	const VkImageLayout wanted = EnigmaRHI::Utilities::ToVulkanImageLayout(requested);
	const VkImageLayout tracked = imageLayout;

	auto isDepthAttachmentLayout = [](VkImageLayout l)
		{
			return l == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL || l == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		};

	return (isDepthAttachmentLayout(wanted) && isDepthAttachmentLayout(tracked)) ? tracked : wanted;
}

void EnigmaRHI::VulkanImage::TransitionLayout(VulkanDevice* device, VulkanCommandPool* commandPool, VkImageLayout newLayout)
{
	VkCommandBuffer cmd = commandPool->BeginSingleTimeCommands(device, commandPool->GetCommandBuffer(0)->API_Vulkan().GetHandle());
	RecordTransition(cmd, newLayout);
	commandPool->EndSingleTimeCommands(device, cmd);
}

void EnigmaRHI::VulkanImage::CopyFromBuffer(VulkanDevice* device, VulkanCommandPool* commandPool, VkBuffer buffer, Extent3D extent)
{
	VkCommandBuffer commandBuffer = commandPool->BeginSingleTimeCommands(device, commandPool->GetCommandBuffer(0)->API_Vulkan().GetHandle());

	// specify which part of the buffer is going to be copied to which part of the image.
	VkBufferImageCopy region
	{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageOffset = { 0, 0, 0 },
		.imageExtent = {extent.width, extent.height, extent.depth} ,
	};

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = desc.arrayLayers;

	vkCmdCopyBufferToImage(commandBuffer, buffer, image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	commandPool->EndSingleTimeCommands(device, commandBuffer);
}

void EnigmaRHI::VulkanImage::GenerateMipmaps(VulkanDevice* device, VulkanCommandPool* commandPool, const ImageDesc& desc)
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(device->GetPhysicalDevice().Handle(), Utilities::ToVulkanFormat(desc.format), &formatProperties);
	constexpr VkFormatFeatureFlags requiredFeatures = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT
		| VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT;

	if ((formatProperties.optimalTilingFeatures & requiredFeatures) != requiredFeatures)
		throw std::runtime_error("texture image format does not support linear blitting!");

	VkCommandBuffer cmd = commandPool->BeginSingleTimeCommands(
		device,
		commandPool->GetCommandBuffer(0)->API_Vulkan().GetHandle()
	);

	int32_t mipWidth = desc.extent.width;
	int32_t mipHeight = desc.extent.height;

	VkImageMemoryBarrier2 barrier
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
		.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
		.dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange =
		{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = desc.arrayLayers
		}
	};

	for (uint32_t i = 1; i < desc.mipLevels; i++)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;

		VkDependencyInfo info
		{
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &barrier
		};

		vkCmdPipelineBarrier2(cmd, &info);

		VkImageBlit blit
		{
			.srcSubresource =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i - 1,
				.baseArrayLayer = 0,
				.layerCount = desc.arrayLayers
			},
			.dstSubresource =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i,
				.baseArrayLayer = 0,
				.layerCount = desc.arrayLayers
			}
		};

		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };

		const int32_t nextWidth = (std::max)(1, mipWidth / 2);
		const int32_t nextHeight = (std::max)(1, mipHeight / 2);

		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { nextWidth, nextHeight, 1 };

		vkCmdBlitImage(
			cmd,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR
		);

		barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
		barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
		barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
		barrier.dstAccessMask = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		info.pImageMemoryBarriers = &barrier;

		vkCmdPipelineBarrier2(cmd, &info);

		barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
		barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
		barrier.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
		barrier.dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

		mipWidth = nextWidth;
		mipHeight = nextHeight;
	}

	barrier.subresourceRange.baseMipLevel = desc.mipLevels - 1;
	barrier.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
	barrier.srcAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
	barrier.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
	barrier.dstAccessMask = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkDependencyInfo dependency
	{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &barrier
	};

	vkCmdPipelineBarrier2(cmd, &dependency);

	commandPool->EndSingleTimeCommands(device, cmd);

	imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

void EnigmaRHI::VulkanImage::Destroy(IDevice* device)
{
	VkDevice vkDevice = device->API_Vulkan().GetHandle();

	if (sampler != VK_NULL_HANDLE)
	{
		vkDestroySampler(vkDevice, sampler, nullptr);
		sampler = VK_NULL_HANDLE;
	}

	if (view != VK_NULL_HANDLE)
	{
		vkDestroyImageView(vkDevice, view, nullptr);
		view = VK_NULL_HANDLE;
	}

	if (ownsImage)
	{
		if (image != VK_NULL_HANDLE)
		{
			vkDestroyImage(vkDevice, image, nullptr);
			image = VK_NULL_HANDLE;
		}

		if (memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(vkDevice, memory, nullptr);
			memory = VK_NULL_HANDLE;
		}
	}
	else
	{
		image = VK_NULL_HANDLE;
	}

	ownsImage = false;
}