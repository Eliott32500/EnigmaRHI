#pragma once
#include <stdexcept>
#include "DescriptorTypes.h"

namespace EnigmaRHI
{
	class IDevice;
	class IBindingLayout;
	class VulkanBindingSet;

	class IBindingSet
	{
	public:
		virtual ~IBindingSet() = default;
		virtual void Create(IDevice* device, IBindingLayout* layout, const BindingSetDesc& desc) = 0;
		virtual void Destroy(IDevice* device) = 0;
		virtual VulkanBindingSet& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanBindingSet"); }
	};
}