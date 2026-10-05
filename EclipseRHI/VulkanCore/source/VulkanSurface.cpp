#include "VulkanSurface.h"

void EnigmaRHI::VulkanSurface::Create(IInstance* instance, WindowInfo windowInfo)
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

void EnigmaRHI::VulkanSurface::Destroy(IInstance* instance)
{
	vkDestroySurfaceKHR(instance->API_Vulkan().GetInstance(), surface, nullptr);
}
