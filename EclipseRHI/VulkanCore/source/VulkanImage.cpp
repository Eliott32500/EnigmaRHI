#include "../include/VulkanImage.h"
#include "../include/VulkanUtilities.h"

void VulkanImage::Create(EclipseRHI::IDevice* device, uint32_t width, uint32_t height, EclipseRHI::ImageFormat format, bool isTexture)
{
	VkImageUsageFlags usage = 0;

	if(!isTexture)
		usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	else
		 usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	VkImageCreateInfo imageInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = UtilitiesVulkan::FormatToVulkan(format),
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};

	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;

	if (vkCreateImage(device->API_Vulkan().GetDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
		throw std::runtime_error("failed to create image!");

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device->API_Vulkan().GetDevice(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = device->API_Vulkan().FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
	};

	if (vkAllocateMemory(device->API_Vulkan().GetDevice(), &allocInfo, nullptr, &memory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate image memory!");

	vkBindImageMemory(device->API_Vulkan().GetDevice(), image, memory, 0);
}

void VulkanImage::CreateTextureImage(const void* textureData, EclipseRHI::IDevice* device, EclipseRHI::ICommandPool* commandPool, uint32_t width, uint32_t height, EclipseRHI::ImageFormat format)
{
	VkDeviceSize imageSize = width * height * 4;
	VulkanBuffer stagingBuffer;

	stagingBuffer.Create(&device->API_Vulkan(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer.UploadData(&device->API_Vulkan(), 0, imageSize, textureData, 0);


	Create(
		device,
		width, height,
		format,
		true
	);

	TransitionLayout(&device->API_Vulkan(), &commandPool->API_Vulkan(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyFromBuffer(&device->API_Vulkan(), &commandPool->API_Vulkan(), stagingBuffer.GetBuffer(), static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	TransitionLayout(&device->API_Vulkan(), &commandPool->API_Vulkan(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stagingBuffer.Destroy(&device->API_Vulkan());

	CreateView(&device->API_Vulkan(), image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	CreateSampler(&device->API_Vulkan());
}

void VulkanImage::CreateView(VulkanDevice* device, VkImage imageInfo, VkFormat format, VkImageAspectFlags aspect)
{
	VkImageViewCreateInfo viewInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = imageInfo,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
	};

	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	if (vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &view) != VK_SUCCESS)
		throw std::runtime_error("failed to create image view!");
}

void VulkanImage::CreateSampler(VulkanDevice* device, VkFilter filter)
{
	VkSamplerCreateInfo samplerInfo
	{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		// Can be VK_FILTER_NEAREST
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,

		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,

		// UVW -> Convension for texture space coordinates
		// VK_SAMPLER_ADDRESS_MODE_REPEAT: Repeat the texture when going beyond the image dimensions.
		// VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: Like repeat, but inverts the coordinates to mirror the image when going beyond the dimensions.
		// VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE : Take the color of the edge closest to the coordinate beyond the image dimensions.
		// VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE : Like clamp to edge, but instead uses the edge opposite to the closest edge.
		// VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER : Return a solid color when sampling beyond the dimensions of the image.

		// X
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		// Y
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		// Z
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,

		.mipLodBias = 0.0f,

		// Anisotropic filter
		.anisotropyEnable = VK_TRUE,

		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,

		.minLod = 0.f,
		.maxLod = 0.f,

		// Field specifies which color is returned when sampling beyond the image with clamp to border addressing mode
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,

	};

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(device->GetPhysicalDevice(), &properties);

	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	if (vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
		throw std::runtime_error("failed to create texture sampler!");
}


void VulkanImage::TransitionLayout(VulkanDevice* device, VulkanCommandPool* commandPool, VkImageLayout oldLayout, VkImageLayout newLayout)
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

		if (device->HasStencilComponent(format))
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

void VulkanImage::CopyFromBuffer(VulkanDevice* device, VulkanCommandPool* commandPool, VkBuffer buffer, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = commandPool->BeginSingleTimeCommands(device);

	// specify which part of the buffer is going to be copied to which part of the image.
	VkBufferImageCopy region
	{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageOffset = { 0, 0, 0 },
		.imageExtent = {width ,height ,1},
	};

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	vkCmdCopyBufferToImage(commandBuffer, buffer, image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	commandPool->EndSingleTimeCommands(device, commandBuffer);
}

void VulkanImage::Destroy(EclipseRHI::IDevice* device)
{
	if (sampler != VK_NULL_HANDLE)
		vkDestroySampler(device->API_Vulkan().GetDevice(), sampler, nullptr);
	vkDestroyImageView(device->API_Vulkan().GetDevice(), view, nullptr);
	vkDestroyImage(device->API_Vulkan().GetDevice(), image, nullptr);
	vkFreeMemory(device->API_Vulkan().GetDevice(), memory, nullptr);
}
