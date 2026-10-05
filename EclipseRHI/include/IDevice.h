#pragma once

#include "ISurface.h"
#include "IFormat.h"
#include "IBuffer.h"
#include "IImage.h"
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
		virtual IImage* CreateImage(uint32_t width, uint32_t height, ImageFormat format, bool isTexture = false) = 0;
		virtual void DeleteBuffer(IBuffer* buffer) = 0;
		virtual void DeleteImage(IImage* image) = 0;

		virtual void DeviceWaitIdle() = 0;

		virtual ImageFormat FindDepthFormat() = 0;

		virtual VulkanDevice& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanDevice"); }
	};
}
