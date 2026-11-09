#pragma once

#include "IShaderModule.h"

namespace EnigmaRHI
{
	struct Shader
	{
		const char* name;
		IShaderModule* vertShader;
		IShaderModule* fragShader;
		IShaderModule* geomShader;
		IShaderModule* computeShader;
	};
}