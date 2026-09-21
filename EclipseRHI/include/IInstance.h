#pragma once

#include <iostream>

class VulkanInstance;

namespace EclipseRHI
{
	class IInstance
	{
	public:
		virtual ~IInstance() = default;
		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual VulkanInstance& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanInstance"); }
	};
}