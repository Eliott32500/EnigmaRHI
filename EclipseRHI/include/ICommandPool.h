#pragma once
#include "ICommandBuffer.h"
#include <iostream>
#include <vector>


namespace EnigmaRHI
{
	class VulkanCommandPool;

	class ICommandPool
	{
	public:

		virtual ~ICommandPool() = default;
		virtual void Create(class IDevice* device, class ISurface* surface) = 0;
		virtual void Destroy(class IDevice* device) = 0;

		virtual ICommandBuffer* GetCommandBuffer(uint32_t index) = 0;

		virtual VulkanCommandPool& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanCommandPool"); }
	};
}
