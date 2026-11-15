#pragma once

#include "DescriptorTypes.h"
#include <stdexcept>

namespace EnigmaRHI
{
	class IDevice;
	class VulkanBindingLayout;

	class IBindingLayout
	{
	public:
		virtual ~IBindingLayout() = default;
		virtual void Create(IDevice* device, const BindingLayoutDesc& desc) = 0;
		virtual void Destroy(IDevice* device) = 0;
		virtual VulkanBindingLayout& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanBindingLayout"); }
	};
}
