#pragma once

#include <iostream>

namespace EnigmaRHI
{
	class VulkanFence;

	class IFence
	{
    public:
        virtual ~IFence() = default;

        virtual void Wait(uint64_t timeout = UINT64_MAX) = 0;
        virtual bool IsSignaled() const = 0;
        virtual void Reset() = 0;

        virtual uint64_t GetValue() const = 0;

        virtual IFence& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanFence"); }
	};
}
