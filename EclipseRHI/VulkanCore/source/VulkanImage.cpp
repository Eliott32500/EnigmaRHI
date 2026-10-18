#include "VulkanImage.h"
#include "RHILog.h"
#include "VulkanCommonTypes.h"
#include <assert.h>
#include <algorithm>

void EnigmaRHI::VulkanImage::Create(IDevice* device, ImageDesc imageDesc)
{
	VkImageCreateInfo imageInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
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

	if (vkCreateImage(device->API_Vulkan().GetDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to create image !");

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device->API_Vulkan().GetDevice(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = device->API_Vulkan().GetPhysicalDevice().FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
	};

	if (vkAllocateMemory(device->API_Vulkan().GetDevice(), &allocInfo, nullptr, &memory) != VK_SUCCESS)
		LOG_ERROR(LogCategory::Vulkan, "Failed to allocate image memory!");

	vkBindImageMemory(device->API_Vulkan().GetDevice(), image, memory, 0);

	ownsImage = true;
}

void EnigmaRHI::VulkanImage::CreateTextureImage(IDevice* device, ICommandPool* commandPool, ImageDesc imageDesc)
{
	if (!imageDesc.data)
	{
		LOG_ERROR(LogCategory::Vulkan, "Try to create a textureimage without data inside !");

		VulkanDevice& dev = device->API_Vulkan();
		VkFormat vkFormat = Utilities::ToVulkanFormat(imageDesc.format);
		Create(device, imageDesc);
		CreateView(&dev, vkFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		CreateSampler(&dev);

		ownsImage = true;

		return;
	}

	VulkanDevice& dev = device->API_Vulkan();
	VkFormat vkFormat = Utilities::ToVulkanFormat(imageDesc.format);
	VkDeviceSize imageSize = static_cast<VkDeviceSize>(imageDesc.extent.width) * imageDesc.extent.height * Utilities::BytesPerPixel(imageDesc.format);

	VulkanBuffer stagingBuffer;

	stagingBuffer.Create(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	stagingBuffer.UploadData(device, 0, imageSize, imageDesc.data, 0);

	Create(device, imageDesc);

	TransitionLayout(&dev, &commandPool->API_Vulkan(), imageDesc.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyFromBuffer(&dev, &commandPool->API_Vulkan(), stagingBuffer.GetBuffer(), imageDesc.extent);
	TransitionLayout(&dev, &commandPool->API_Vulkan(), imageDesc.format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stagingBuffer.Destroy(&dev);

	CreateView(&dev, vkFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	CreateSampler(&dev);

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

void EnigmaRHI::VulkanImage::CreateSampler(VulkanDevice* device, VkFilter filter)
{
	VkSamplerCreateInfo samplerInfo
	{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,

		// X
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		// Y
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		// Z
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,

		.mipLodBias = 0.0f,

		// Anisotropic filter
		.anisotropyEnable = VK_TRUE,
		.maxAnisotropy = device->GetPhysicalDevice().GetLimits().maxSamplerAnisotropy,

		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,

		.minLod = 0.f,
		.maxLod = 0.f,

		// Field specifies which color is returned when sampling beyond the image with clamp to border addressing mode
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(device->GetPhysicalDevice().Handle(), &properties);

	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

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

		if (VulkanPhysicalDevice::HasStencilComponent(Utilities::ToVulkanFormat(format)))
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
