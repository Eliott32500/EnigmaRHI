#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "IInstance.h"
#include <iostream>

class VulkanSurface;

namespace EclipseRHI
{
	class ISurface
	{
	public:

		virtual ~ISurface() = default;
		virtual void Create(IInstance* instance, GLFWwindow* window) = 0;
		virtual void Destroy(IInstance* instance) = 0;

		virtual VulkanSurface& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanSurface"); }
	};
}
