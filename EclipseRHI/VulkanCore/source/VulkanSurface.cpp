#include "../include/VulkanSurface.h"

void VulkanSurface::Create(EnigmaRHI::IInstance* instance, GLFWwindow* mainWindow)
{
	if (glfwCreateWindowSurface(instance->API_Vulkan().GetInstance(), mainWindow, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
}

void VulkanSurface::Destroy(EnigmaRHI::IInstance* instance)
{
	vkDestroySurfaceKHR(instance->API_Vulkan().GetInstance(), surface, nullptr);
}
