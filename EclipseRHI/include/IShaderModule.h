#pragma once

#include <iostream>
#include "IDevice.h"
#include "ISwapChain.h"
#include "ShaderTypes.h"


namespace EnigmaRHI
{
	class VulkanShaderModule;

	class IShaderModule
	{
	public:

		virtual ~IShaderModule() = default;
		virtual void Create(IDevice* device, const std::string& filename, const char* entryName = "main") = 0;
		virtual void Destroy(IDevice* device) = 0;

		const char* GetEntryName() { return entryName; };

		virtual VulkanShaderModule& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanShaderModule"); }

	protected:

		const char* entryName = "main";
	};
}
