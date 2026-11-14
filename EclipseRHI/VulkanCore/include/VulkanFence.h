#pragma once

#include "IFence.h"

namespace EnigmaRHI
{
	class VulkanFence : public IFence
	{
	public:

		void Wait(uint64_t timeout = UINT64_MAX) override;
		bool IsSignaled() const override;
		void Reset() override;

		uint64_t GetValue() const override;

		IFence& API_Vulkan() override { return *this; };
	};
}