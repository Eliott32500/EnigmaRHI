#pragma once

#include <iostream>

#include "IDevice.h"
#include "ICommandPool.h"
#include "ImageSampler.h"
#include "CommonTypes.h"


namespace EnigmaRHI
{
	class VulkanImage;

	struct ImageDesc
	{
		EImageType type = EImageType::TYPE_2D;
		EImageFormat format;
		EImageUsage usage;
		ImageSampler sampler;
		Extent3D extent{0, 0, 1};
		uint32_t mipLevels = 1;
		uint32_t arrayLayers = 1;
		uint32_t sampleCount = 1;
		const void* data = nullptr;
	};

	class IImage
	{
	public:

		virtual ~IImage() = default;

		virtual void Create(IDevice* device, ImageDesc imageDesc) = 0;
		virtual void CreateTextureImage(const void* data, IDevice* device, ICommandPool* commandPool, uint32_t width, uint32_t height, EImageFormat format) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual VulkanImage& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanImage"); }
	};
}