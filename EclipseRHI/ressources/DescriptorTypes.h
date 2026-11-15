#pragma once

#include "ShaderTypes.h"
#include <vector>

namespace EnigmaRHI
{
	class IBuffer;
	class IImage;

	enum class EBindingType
	{
        COMBINED_IMAGE_SAMPLER,
        UNIFORM_BUFFER,
        STORAGE_BUFFER,
	};

	struct BindingLayoutItem
	{
		uint32_t     slot;
		EBindingType type;
		EShaderType  stages;
		uint32_t     count = 1;
	};

	struct BindingLayoutDesc 
	{ 
		std::vector<BindingLayoutItem> items;
	};

	struct BindingSetItem
	{
		uint32_t     slot;
		EBindingType type;
		IBuffer* buffer = nullptr;
		IImage* image = nullptr;
		size_t       offset = 0;
		size_t       range = 0;

		static BindingSetItem UniformBuffer(uint32_t slot, IBuffer* b, size_t offset = 0, size_t range = 0)
		{
			return { slot, EBindingType::UNIFORM_BUFFER, b, nullptr, offset, range };
		}

		static BindingSetItem Texture(uint32_t slot, IImage* img)
		{
			return { slot, EBindingType::COMBINED_IMAGE_SAMPLER, nullptr, img, 0, 0 };
		}
	};

	struct BindingSetDesc 
	{ 
		std::vector<BindingSetItem> items; 
	};
}