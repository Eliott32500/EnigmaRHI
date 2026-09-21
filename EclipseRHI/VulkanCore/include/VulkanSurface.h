#pragma once

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "../../include/ISurface.h"

class VulkanSurface : public EclipseRHI::ISurface
{
public:
	void Create(EclipseRHI::IInstance* instance, GLFWwindow* mainWindow) override;
	void Destroy(EclipseRHI::IInstance* instance) override;

	VkSurfaceKHR GetSurface() const { return surface; }

	VulkanSurface& API_Vulkan() override { return (*this); }
	
private:

	VkSurfaceKHR surface;
};