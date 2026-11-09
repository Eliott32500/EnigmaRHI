#pragma once

#include "IShaderModule.h"

namespace EnigmaRHI
{
	struct Shader
	{
		IShaderModule* vertShader = nullptr;
		IShaderModule* fragShader = nullptr;
		IShaderModule* geomShader = nullptr;
		IShaderModule* computeShader = nullptr;
		const char* entryMethod = "main";
	};
}