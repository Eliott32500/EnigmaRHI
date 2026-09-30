#pragma once
#include "IDevice.h"
#include "ICommandBuffer.h"
#include <iostream>
#include <vector>

class VulkanCommandPool;

namespace EnigmaRHI
{
	class ICommandPool
	{
	public:

		virtual ~ICommandPool() = default;
		virtual void Create(IDevice* device, ISurface* surface) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual ICommandBuffer* GetCommandBuffer(uint32_t index) = 0;

		virtual VulkanCommandPool& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanCommandPool"); }
	};
}
