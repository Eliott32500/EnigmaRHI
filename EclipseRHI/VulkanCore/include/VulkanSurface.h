#pragma once

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "ISurface.h"

class VulkanSurface : public EnigmaRHI::ISurface
{
public:
	void Create(EnigmaRHI::IInstance* instance, EnigmaRHI::WindowInfo windowInfo) override;
	void Destroy(EnigmaRHI::IInstance* instance) override;

	VkSurfaceKHR GetSurface() const { return surface; }

	VulkanSurface& API_Vulkan() override { return (*this); }
	
private:

	VkSurfaceKHR surface;
};