#pragma once

#include "ImageTypes.h"
#include "DepthStencilTypes.h"

namespace EnigmaRHI
{
	struct ImageSampler
	{
		EFilter minFilter = EFilter::NEAREST;
		EFilter magFilter = EFilter::NEAREST;
		EMipmapMode mipMapMode = EMipmapMode::NEAREST;
		EWrappingMode wrapU = EWrappingMode::REPEAT;
		EWrappingMode wrapV = EWrappingMode::REPEAT;
		EWrappingMode wrapW = EWrappingMode::REPEAT;
		EBorderColor borderColor = EBorderColor::OPAQUE_INT_BLACK;

		float mipLodBias = 0.0f;
		float minLod = 0.0f;
		float maxLod = 0.0f;

		// 0 disable / 1 maxAnisotropy available / 2, 4, 8 ... override value ( clamp to max available )
		float maxAnisotropy = 0.0f;

		bool compareEnable = false;
		ECompareOp compareOp = ECompareOp::NEVER;
	};
};