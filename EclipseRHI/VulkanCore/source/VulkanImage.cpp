#include "VulkanImage.h"
#include "RHILog.h"
#include "VulkanCommonTypes.h"
#include "VulkanDepthStencilTypes.h"
#include <assert.h>
#include <algorithm>

void EnigmaRHI::VulkanImage::Create(IDevice* device, ImageDesc imageDesc)
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
		LOG_ERROR(LogCategory::Vulkan, "Image extent exceeds device limit (" + std::to_string(maxDim) + ")");
		return;
	}

	if (imageDesc.arrayLayers > limits.maxImageArrayLayers)
	{
		LOG_ERROR(LogCategory::Vulkan, "arrayLayers exceeds device limit");
		return;
	}

	const bool isDepth = Utilities::HasDepthComponent(imageDesc.format);
	const uint32_t maxSamples = isDepth ? limits.maxDepthSampleCount : limits.maxColorSampleCount;

	if (imageDesc.sampleCount > maxSamples)
	{
		LOG_WARNING(LogCategory::Vulkan, "sampleCount clamped to " + std::to_string(maxSamples));
		imageDesc.sampleCount = maxSamples;
	}

	VkImageCreateFlags flags = 0;

	if (imageDesc.viewType == EImageViewType::VIEW_CUBE ||
		imageDesc.viewType == EImageViewType::VIEW_CUBE_ARRAY)
	{
		flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	}

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

	const VulkanDevice& myDevice = device->API_Vulkan();
	VkDevice vulkanDevice = myDevice.GetDevice();

	if (vkCreateImage(vulkanDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to create image !");

	AllocateImageMemory(myDevice, image);

	ownsImage = true;
}


void EnigmaRHI::VulkanImage::AllocateImageMemory(const VulkanDevice& device, const VkImage& image)
{
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device.GetDevice(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = device.GetPhysicalDevice().FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
	};

	if (vkAllocateMemory(device.GetDevice(), &allocInfo, nullptr, &memory) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to allocate image memory!");

	vkBindImageMemory(device.GetDevice(), image, memory, 0);
}

void EnigmaRHI::VulkanImage::CreateTextureImage(IDevice* device, ICommandPool* commandPool, ImageDesc imageDesc)
{
	if (!imageDesc.data)
	{
		LOG_ERROR(LogCategory::Vulkan, "Try to create a textureimage without data inside !");

		VulkanDevice& dev = device->API_Vulkan();
		Create(device, imageDesc);
		CreateView(&dev, imageDesc, VK_IMAGE_ASPECT_COLOR_BIT);
		CreateSampler(&dev, imageDesc.sampler);

		ownsImage = true;

		return;
	}

	VulkanDevice& dev = device->API_Vulkan();
	VkDeviceSize imageSize = static_cast<VkDeviceSize>(imageDesc.extent.width) * imageDesc.extent.height * Utilities::BytesPerPixel(imageDesc.format);

	VulkanBuffer stagingBuffer;

	stagingBuffer.Create(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingBuffer.UploadData(device, 0, imageSize, imageDesc.data, 0);

	imageDesc.usage |= EImageUsage::SAMPLED | EImageUsage::TRANSFER_DST;
	Create(device, imageDesc);

	TransitionLayout(&dev, &commandPool->API_Vulkan(), imageDesc.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyFromBuffer(&dev, &commandPool->API_Vulkan(), stagingBuffer.GetBuffer(), imageDesc.extent);
	TransitionLayout(&dev, &commandPool->API_Vulkan(), imageDesc.format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stagingBuffer.Destroy(&dev);

	CreateView(&dev, imageDesc, VK_IMAGE_ASPECT_COLOR_BIT);
	CreateSampler(&dev, imageDesc.sampler);

	ownsImage = true;
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
	VkImageViewCreateInfo viewInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
	};

	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &view) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to create image view!");
}

void EnigmaRHI::VulkanImage::CreateView(VulkanDevice* device, ImageDesc imageDesc, VkImageAspectFlags aspect)
{
	VkImageViewCreateInfo viewInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = image,
		.viewType = Utilities::ToVulkanImageViewType(imageDesc.viewType),
		.format = Utilities::ToVulkanFormat(imageDesc.format),
	};

	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = imageDesc.arrayLayers;

	if (vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &view) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to create image view!");
}

void EnigmaRHI::VulkanImage::CreateSampler(VulkanDevice* device, ImageSampler imageSampler)
{
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

	if (vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to create texture sampler!");
}


void EnigmaRHI::VulkanImage::TransitionLayout(VulkanDevice* device, VulkanCommandPool* commandPool, EImageFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = commandPool->BeginSingleTimeCommands(device);

	VkImageMemoryBarrier imageMemoryBarrier
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.srcAccessMask = 0, // TODO
		.dstAccessMask = 0, // TODO
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
	};

	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (Utilities::HasStencilComponent(format))
			imageMemoryBarrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	else
	{
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}


	vkCmdPipelineBarrier
	(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &imageMemoryBarrier
	);

	commandPool->EndSingleTimeCommands(device, commandBuffer);
}

void EnigmaRHI::VulkanImage::CopyFromBuffer(VulkanDevice* device, VulkanCommandPool* commandPool, VkBuffer buffer, Extent3D extent)
{
	VkCommandBuffer commandBuffer = commandPool->BeginSingleTimeCommands(device);

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
	region.imageSubresource.layerCount = 1;

	vkCmdCopyBufferToImage(commandBuffer, buffer, image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	commandPool->EndSingleTimeCommands(device, commandBuffer);
}

void EnigmaRHI::VulkanImage::Destroy(IDevice* device)
{
	VkDevice vkDevice = device->API_Vulkan().GetDevice();

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

