#pragma once

#include "ISurface.h"
#include "IFormat.h"
#include <iostream>

class VulkanDevice;

namespace EclipseRHI
{
	class IDevice
	{
	public:

		virtual ~IDevice() = default;
		virtual void Create(IInstance* instance, ISurface* surface) = 0;
		virtual void Destroy() = 0;

		virtual void DeviceWaitIdle() = 0;

		virtual ImageFormat FindDepthFormat() = 0;

		const int MAX_FRAMES_IN_FLIGHT = 2;

		virtual VulkanDevice& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanDevice"); }
	};
}
