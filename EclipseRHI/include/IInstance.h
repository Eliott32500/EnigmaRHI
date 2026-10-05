#pragma once

#include <vector>
#include <iostream>

namespace EnigmaRHI
{
	class VulkanInstance;

	struct InstanceCreateInfo
	{
		std::vector<const char*> extensions;
		std::vector<const char*> layers;

		const char* applicationName;
		const char* engineName;

		uint32_t applicationVersion;
		uint32_t engineVersion;
	};

	class IInstance
	{
	public:
		virtual ~IInstance() = default;
		virtual void Create(InstanceCreateInfo createInfo) = 0;
		virtual void Destroy() = 0;

		virtual VulkanInstance& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanInstance"); }
	};
}