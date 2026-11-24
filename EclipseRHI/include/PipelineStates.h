#pragma once

#include "RasterizationTypes.h"
#include "BlendTypes.h"
#include "RenderTypes.h"
#include "DepthStencilTypes.h"
#include "PipelineDynamicState.h"
#include "VertexInputTypes.h"
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

}