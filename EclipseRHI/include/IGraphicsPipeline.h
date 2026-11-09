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
		EFrontFaceMode frontFaceMode = EFrontFaceMode::CLOCKWISE;

		//Bias
		bool depthBiasEnable = false;
		float depthBiasConstantFactor = 0.0f;
		float depthBiasClamp = 0.0f;
		float depthBiasSlopeFactor = 0.0f;

		float lineWidth = 1.f;
	};

	struct BlendPipelineState
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
		bool stencilTestEnable = false;
	};

	//struct MultisamplePipelineState
	//{
	//	ESampleCount sampleCount = ESampleCount::X1;
	//	bool alphaToCoverageEnable = false;
	//};

	struct GraphicsPipelineDesc
	{
		std::vector<EDynamicState> dynamicStates;

		EPrimitiveTopologyMode topologyMode = EPrimitiveTopologyMode::TRIANGLE_LIST;

		RasterizerPipelineState rasterizerState;
		BlendPipelineState blendState;

		bool depthTestEnable = false;
		bool depthWriteEnable = false;

		ECompareOp compareMode = ECompareOp::LESS;
	};
}