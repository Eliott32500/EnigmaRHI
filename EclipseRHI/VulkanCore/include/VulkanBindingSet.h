#pragma once

#include "IBindingSet.h"
#include "volk.h"

namespace EnigmaRHI
{
	class VulkanBindingSet : public IBindingSet
	{
	public:
		void Create(IDevice* device, IBindingLayout* layout, const BindingSetDesc& desc) override;
		void Destroy(IDevice* device) override;

		VkDescriptorSet GetHandle() const { return set; }
		VulkanBindingSet& API_Vulkan() override { return *this; }

	private:
		VkDescriptorSet set = VK_NULL_HANDLE;
	};
}