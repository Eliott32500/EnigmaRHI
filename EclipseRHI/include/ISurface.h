#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "IInstance.h"
#include <iostream>


namespace EnigmaRHI
{
	class VulkanSurface;

	struct WindowInfo
	{
		HINSTANCE hInstance;
		HWND hwnd;
	};

	class ISurface
	{
	public:

		virtual ~ISurface() = default;
		virtual void Create(IInstance* instance, WindowInfo windowInfo) = 0;
		virtual void Destroy(IInstance* instance) = 0;

		virtual VulkanSurface& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanSurface"); }
	};
}
