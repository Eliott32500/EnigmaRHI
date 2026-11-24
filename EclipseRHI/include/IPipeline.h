#pragma once

#include <iostream>
#include "IDevice.h"
#include "PipelineStates.h"
#include "IShader.h"

namespace EnigmaRHI
{
	class VulkanPipeline;

	struct GraphicsPipelineDesc
	{
		std::vector<EDynamicState> dynamicStates = { EDynamicState::VIEWPORT, EDynamicState::SCISSOR };

		std::vector<VertexBindingDesc> vertexBindings;
		std::vector<VertexAttributeDesc> vertexAttributes;

		ETopologyMode topologyMode = ETopologyMode::TRIANGLE_LIST;

		RasterizerPipelineState rasterizerState;

		MultisamplePipelineState multisampleState;

		//One for each color attachment used
		std::vector<ColorBlendAttachmentState> blendStates;

		DepthStencilPipelineState depthStencilState;

		uint32_t viewportCount = 1;
		uint32_t scissorCount = 1;
	};

	struct ComputePipelineDesc
	{

	};

	enum class EPipelineType
	{
		GRAPHICS,
		COMPUTE
	};

	class IPipeline
	{
	public:

		virtual ~IPipeline() = default;
		virtual void CreateGraphicsPipeline(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout, EnigmaRHI::EImageFormat colorFormat) = 0;
		virtual void CreateComputePipeline(const ComputePipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual VulkanPipeline& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanGraphicsPipeline"); }
	};
}
