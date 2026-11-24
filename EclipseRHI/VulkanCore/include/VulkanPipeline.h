#pragma once
#include "VulkanShaderModule.h"
#include "VulkanSwapChain.h"
#include "IPipeline.h"
#include "VulkanVertex.h"
#include <vector>

namespace EnigmaRHI
{
	class VulkanPipeline : public IPipeline
	{
	public:

		VulkanPipeline() = default;

		void CreateGraphicsPipeline(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout, EImageFormat colorFormat) override;
		void CreateComputePipeline(const ComputePipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout) override;
		void Destroy(IDevice* device) override;

		VkPipeline GetHandle() const { return pipeline; }
		VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }

		VulkanPipeline& API_Vulkan() override { return (*this); }

	private:

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;

		std::vector<VkPipelineShaderStageCreateInfo> CreateShaderInfo(const Shader& shader, EPipelineType type);
	};
}