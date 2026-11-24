#pragma once

#include "IComputePipeline.h"
#include "VulkanPipeline.h"

namespace EnigmaRHI
{
	class VulkanComputePipeline : public IComputePipeline
	{
	public:

		void Create(IShaderModule& computeShaderModule, IDevice* device, IBindingLayout* layout) override;
		void Destroy(IDevice* device) override;

		VulkanPipeline& API_Vulkan() { return pipeline; }

	private:

		VulkanPipeline pipeline;
	};
}