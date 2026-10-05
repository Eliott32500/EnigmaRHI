#pragma once

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "ISurface.h"

namespace EnigmaRHI
{
	class VulkanSurface : public ISurface
	{
	public:
		void Create(IInstance* instance, WindowInfo windowInfo) override;
		void Destroy(IInstance* instance) override;

		VkSurfaceKHR GetSurface() const { return surface; }

		VulkanSurface& API_Vulkan() override { return (*this); }

	private:

		VkSurfaceKHR surface;
	};
}