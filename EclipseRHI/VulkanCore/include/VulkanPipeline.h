#pragma once
#include "VulkanShaderModule.h"
#include "VulkanSwapChain.h"
#include "VulkanDescriptor.h"
#include "VulkanRenderPass.h"
#include "IPipeline.h"
#include "VulkanVertex.h"
#include <vector>

namespace EnigmaRHI
{
	class VulkanPipeline : public IPipeline
	{
	public:

		VulkanPipeline() = default;

		void Create(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IRenderPass* renderPass, IDescriptor* descriptor, ISwapChain* swapChain) override;
		void Destroy(IDevice* device) override;

		VkPipeline GetGraphicsPipeline() const { return graphicsPipeline; }
		VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }

		VulkanPipeline& API_Vulkan() override { return (*this); }

	private:

		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;

		std::vector<VkPipelineShaderStageCreateInfo> CreateShaderInfo(const Shader& shader);
	};
}