#pragma once

#include <iostream>
#include "ISwapChain.h"
#include "IDevice.h"
#include "IRenderPass.h"
#include "IDescriptor.h"
#include "IGraphicsPipeline.h"
#include "IShader.h"


namespace EnigmaRHI
{
	class VulkanPipeline;

	class IPipeline
	{
	public:

		virtual ~IPipeline() = default;
		virtual void Create(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, ISwapChain* swapchain, IRenderPass* renderPass, IDescriptor* descriptor) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual VulkanPipeline& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanPipeline"); }
	};
}
