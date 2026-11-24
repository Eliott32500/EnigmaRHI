#pragma once

#include <iostream>
#include "IDevice.h"
#include "IShader.h"
#include "IPipeline.h"

namespace EnigmaRHI
{
	class VulkanComputePipeline;
	
	class IComputePipeline : public IPipeline
	{
	public:

		virtual ~IComputePipeline() = default;
		virtual void Create(IShaderModule& computeShaderModule, IDevice* device, IBindingLayout* layout) = 0;
		virtual void Destroy(IDevice* device) = 0;
	};
}
