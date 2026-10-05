#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "ICommandPool.h"
#include "IFormat.h"

class VulkanSwapChain;

namespace EnigmaRHI
{
	class ISwapChain
	{
	public:

		virtual ~ISwapChain() = default;
		virtual void Create(IDevice* device, ISurface* surface, ICommandPool* commandPool, GLFWwindow* window) = 0;
		virtual void Destroy(IDevice* device) = 0;

		float GetWidth() const { return width; }
		float GetHeight() const { return height; }

		virtual ImageFormat GetSwapChainImageFormat() = 0;

		virtual VulkanSwapChain& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanSwapChain"); }

	protected:

		float width;
		float height;
	};
}