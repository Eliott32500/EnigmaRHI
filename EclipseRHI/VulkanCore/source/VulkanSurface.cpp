#include "VulkanSurface.h"

void VulkanSurface::Create(EnigmaRHI::IInstance* instance, EnigmaRHI::WindowInfo windowInfo)
{
	VkWin32SurfaceCreateInfoKHR createInfo
	{
		.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		.hinstance = windowInfo.hInstance,
		.hwnd = windowInfo.hwnd,
	};

	if(vkCreateWin32SurfaceKHR(instance->API_Vulkan().GetInstance(), &createInfo, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
}

void VulkanSurface::Destroy(EnigmaRHI::IInstance* instance)
{
	vkDestroySurfaceKHR(instance->API_Vulkan().GetInstance(), surface, nullptr);
}
