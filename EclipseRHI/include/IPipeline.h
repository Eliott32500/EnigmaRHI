#pragma once

namespace EnigmaRHI
{
	class VulkanPipeline;

	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;

		virtual VulkanPipeline& API_Vulkan() = 0;
	};
}