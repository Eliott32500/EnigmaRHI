#pragma once

#include "ISurface.h"
#include "IBuffer.h"
#include "ImageTypes.h"
#include "IImage.h"
#include "DeviceCapabilities.h"
#include <iostream>


namespace EnigmaRHI
{
	class VulkanDevice;

	class IDevice
	{
	public:

		virtual ~IDevice() = default;
		virtual void Create(IInstance* instance, ISurface* surface) = 0;
		virtual void Destroy() = 0;

		virtual IBuffer* CreateBuffer(size_t size, uint32_t usage, size_t properties) = 0;
		virtual IImage* CreateImage(ImageDesc imageDesc) = 0;
		virtual IImage* CreateImageTexture(ICommandPool* commandPool, ImageDesc imageDesc) = 0;
		virtual void DeleteBuffer(IBuffer* buffer) = 0;
		virtual void DeleteImage(IImage* image) = 0;

		virtual void WaitIdle() = 0;

		virtual EImageFormat FindDepthFormat() = 0;

		DeviceSpecifications GetCapabilities() const { return capabilities; }

		virtual VulkanDevice& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanDevice"); }

	protected:
		
		DeviceSpecifications capabilities;
	};
}
