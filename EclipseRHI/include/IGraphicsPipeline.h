#pragma once

#include <iostream>
#include "IDevice.h"
#include "PipelineStates.h"
#include "IShader.h"
#include "IPipeline.h"

namespace EnigmaRHI
{
	class VulkanGraphicsPipeline;

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

	class IGraphicsPipeline : public IPipeline
	{
	public:

		virtual ~IGraphicsPipeline() = default;
		virtual void Create(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout, EnigmaRHI::EImageFormat colorFormat) = 0;
		virtual void Destroy(IDevice* device) = 0;
	};
}
