#pragma once

#include <iostream>

#include "IFormat.h"
#include "ISwapChain.h"

class VulkanRenderPass;

namespace EnigmaRHI
{
	class IRenderPass
	{
	public:

		virtual ~IRenderPass() = default;
		virtual void Create(IDevice* device, ISwapChain* swapChain, ImageFormat depthFormat) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual VulkanRenderPass& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanRenderPass"); }
	};
}
