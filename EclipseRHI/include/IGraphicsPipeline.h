#pragma once

#include "RasterizationTypes.h"
#include "BlendTypes.h"
#include "RenderTypes.h"
#include "DepthStencilTypes.h"
#include "PipelineDynamicState.h"
#include <vector>

namespace EnigmaRHI
{
	struct RasterizerPipelineState
	{
		EPolygonMode polygonMode = EPolygonMode::FILL;
		ECullMode cullMode = ECullMode::BACK;
		EFrontFaceMode frontFaceMode = EFrontFaceMode::COUNTER_CLOCKWISE;

		//Bias
		bool depthBiasEnable = false;
		float depthBiasConstantFactor = 0.0f;
		float depthBiasClamp = 0.0f;
		float depthBiasSlopeFactor = 0.0f;

		float lineWidth = 1.f;
	};

	struct ColorBlendAttachmentState
	{
		bool blendEnable = true;

		EBlendFactor srcColorBlendFactor = EBlendFactor::SRC_ALPHA;
		EBlendFactor dstColorBlendFactor = EBlendFactor::ONE_MINUS_SRC_ALPHA;
		EBlendOp colorBlendOp = EBlendOp::ADD;
		EBlendFactor srcAlphaBlendFactor = EBlendFactor::ONE;
		EBlendFactor dstAlphaBlendFactor = EBlendFactor::ZERO;
		EBlendOp alphaBlendOp = EBlendOp::ADD;

		EColorMask colorMask = EColorMask::COLOR_ALL;
	};

	struct DepthStencilPipelineState
	{
		bool depthTestEnable = true;
		bool depthWriteEnable = true;
		ECompareOp depthCompareOp = ECompareOp::LESS;
		bool depthBoundsTestEnable = false;
		bool stencilTestEnable = false;
	};

	struct MultisamplePipelineState
	{
		uint32_t sampleCount = 1;
		bool sampleShadingEnable = false;
		float minSampleShading = 1.0f;
		bool alphaToCoverageEnable = false;
		bool alphaToOneEnable = false;
	};

	struct GraphicsPipelineDesc
	{
		std::vector<EDynamicState> dynamicStates = { EDynamicState::VIEWPORT, EDynamicState::SCISSOR };

		EPrimitiveTopologyMode topologyMode = EPrimitiveTopologyMode::TRIANGLE_LIST;

		RasterizerPipelineState rasterizerState;

		MultisamplePipelineState multisampleState;

		//One for each color attachment used
		std::vector<ColorBlendAttachmentState> blendStates;

		DepthStencilPipelineState depthStencilState;

		uint32_t viewportCount = 1;
		uint32_t scissorCount = 1;
	};
}