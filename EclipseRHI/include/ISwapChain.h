#pragma once

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "ICommandPool.h"
#include "ImageTypes.h"
#include "CommonTypes.h"


namespace EnigmaRHI
{
	class VulkanSwapChain;

	class ISwapChain
	{
	public:

		virtual ~ISwapChain() = default;
		virtual void Create(IDevice* device, class ISurface* surface, ICommandPool* commandPool, Extent2D windowExtent) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual EImageFormat GetSwapChainImageFormat() = 0;
		Extent2D GetSwapChainExtent() const { return swapChainExtent; };

		virtual VulkanSwapChain& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanSwapChain"); }

	protected:

		Extent2D swapChainExtent;
	};
}