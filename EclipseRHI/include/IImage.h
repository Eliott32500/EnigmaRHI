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
		EImageViewType viewType = EImageViewType::VIEW_2D;
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
		virtual void CreateTextureImage(IDevice* device, ICommandPool* commandPool, ImageDesc imageDesc) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual VulkanImage& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanImage"); }
	};
}