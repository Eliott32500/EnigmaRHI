#pragma once

#include "IBindingLayout.h"
#include "volk.h"

namespace EnigmaRHI
{
	class VulkanBindingLayout : public IBindingLayout
	{
	public:
		void Create(IDevice* device, const BindingLayoutDesc& desc) override;
		void Destroy(IDevice* device) override;

		VkDescriptorSetLayout GetHandle() const { return layout; }
		VulkanBindingLayout& API_Vulkan() override { return *this; }

	private:
		VkDescriptorSetLayout layout = VK_NULL_HANDLE;
	};
}