#pragma once

#include <iostream>
#include "ISwapChain.h"
#include "IDevice.h"
#include "IRenderPass.h"
#include "IDescriptor.h"
#include "IShaderModule.h"

class VulkanPipeline;

namespace EclipseRHI
{
	class IPipeline
	{
	public:

		virtual ~IPipeline() = default;
		virtual void Create(IShaderModule* vertShader, IShaderModule* fragShader, IDevice* device, ISwapChain* swapchain, IRenderPass* renderPass, IDescriptor* descriptor) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual VulkanPipeline& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanPipeline"); }
	};
}
