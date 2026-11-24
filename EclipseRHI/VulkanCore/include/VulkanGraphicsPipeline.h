#pragma once
#include "VulkanShaderModule.h"
#include "VulkanSwapChain.h"
#include "IGraphicsPipeline.h"
#include "VulkanVertex.h"
#include "VulkanPipeline.h"
#include <vector>

namespace EnigmaRHI
{
	class VulkanGraphicsPipeline : public IGraphicsPipeline
	{
	public:

		VulkanGraphicsPipeline() = default;

		void Create(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout, EImageFormat colorFormat) override;
		void Destroy(IDevice* device) override;

		VulkanPipeline& API_Vulkan() override { return pipeline; }

	private:

		std::vector<VkPipelineShaderStageCreateInfo> CreateShaderInfo(const Shader& shader);
		VulkanPipeline pipeline;
	};
}