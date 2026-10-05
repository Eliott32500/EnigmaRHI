#pragma once
#include "VulkanShaderModule.h"
#include "VulkanSwapChain.h"
#include "VulkanDescriptor.h"
#include "VulkanRenderPass.h"
#include "IPipeline.h"
#include "VulkanVertex.h"

namespace EnigmaRHI
{
	class VulkanPipeline : public IPipeline
	{
	public:

		VulkanPipeline() = default;

		void Create(IShaderModule* vertShader, IShaderModule* fragShader, IDevice* device, ISwapChain* swapchain, IRenderPass* renderPass, IDescriptor* descriptor) override;
		void Destroy(IDevice* device) override;

		VkPipeline GetGraphicsPipeline() const { return graphicsPipeline; }
		VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }

		VulkanPipeline& API_Vulkan() override { return (*this); }

	private:

		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;
	};
}