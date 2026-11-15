#pragma once

#include <cstdint>
#include <stdexcept>

namespace EnigmaRHI
{
	class VulkanFence;
    class IDevice;

	class IFence
	{
    public:
        virtual ~IFence() = default;

        virtual void Create(IDevice* device, bool signaled = false) = 0;
        virtual void Destroy(IDevice* device) = 0;
        virtual bool Wait(IDevice* device, uint64_t timeout = UINT64_MAX) = 0;
        virtual void Reset(IDevice* device) = 0;
        virtual bool IsSignaled(IDevice* device) const = 0;

        virtual VulkanFence& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanFence"); }
	};
}
