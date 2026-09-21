#pragma once

#include <iostream>
#include "IDevice.h"
#include "ISwapChain.h"

class VulkanShaderModule;

namespace EclipseRHI
{
	class IShaderModule
	{
	public:

		virtual ~IShaderModule() = default;
		virtual void Create(IDevice* device, const std::string& filename) = 0;

		virtual VulkanShaderModule& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanShaderModule"); }
	};
}
