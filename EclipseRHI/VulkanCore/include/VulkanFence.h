#pragma once

#include "IFence.h"
#include <vulkan/vulkan.h>

namespace EnigmaRHI
{
	class VulkanFence : public IFence
	{
	public:

		void Create(IDevice* device, bool signaled = false) override;
		void Destroy(IDevice* device) override;
		bool Wait(IDevice* device, uint64_t timeout = UINT64_MAX) override;
		void Reset(IDevice* device) override;
		bool IsSignaled(IDevice* device) const override;

		VkFence GetHandle() const { return fence; }

		VulkanFence& API_Vulkan() override { return *this; };

	private:

		VkFence fence = VK_NULL_HANDLE;
	};
}