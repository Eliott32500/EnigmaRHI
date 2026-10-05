#pragma once

#include <iostream>

#include "IDevice.h"
#include "ICommandPool.h"


namespace EnigmaRHI
{
	class VulkanImage;

	class IImage
	{
	public:

		virtual ~IImage() = default;

		virtual void Create(IDevice* device, uint32_t width, uint32_t height, ImageFormat format, bool isTexture = false) = 0;
		virtual void CreateTextureImage(const void* data, IDevice* device, ICommandPool* commandPool, uint32_t width, uint32_t height, ImageFormat format) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual VulkanImage& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanImage"); }
	};
}