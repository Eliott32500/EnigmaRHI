#pragma once

#include "ImageTypes.h"
#include "DepthStencilTypes.h"

namespace EnigmaRHI
{
	struct ImageSampler
	{
		EFilter minFilter = EFilter::LINEAR;
		EFilter magFilter = EFilter::LINEAR;
		EMipmapMode mipMapMode = EMipmapMode::LINEAR;
		EWrappingMode wrapU = EWrappingMode::REPEAT;
		EWrappingMode wrapV = EWrappingMode::REPEAT;
		EWrappingMode wrapW = EWrappingMode::REPEAT;
		EBorderColor borderColor = EBorderColor::OPAQUE_INT_BLACK;

		float mipLodBias = 0.0f;
		float minLod = 0.0f;
		float maxLod = 0.0f;

		// 0 disable / 1 maxAnisotropy available / 2, 4, 8 ... override value ( clamp to max available )
		float maxAnisotropy = 1.0f;

		bool compareEnable = false;
		ECompareOp compareOp = ECompareOp::ALWAYS;
	};
};