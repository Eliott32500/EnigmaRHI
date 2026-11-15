#pragma once

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "IDevice.h"
#include "ImageTypes.h"
#include "CommonTypes.h"

namespace EnigmaRHI
{
	class VulkanSwapChain;

	enum class EAcquireResult 
	{
		SUCCESS, 
		SUBOPTIMAL, 
		OUT_OF_DATE
	};

	enum class EPresentResult 
	{ 
		SUCCESS,
		SUBOPTIMAL,
		OUT_OF_DATE
	};

	class ISwapChain
	{
	public:

		virtual ~ISwapChain() = default;
		virtual void Create(IDevice* device, class ISurface* surface, Extent2D windowExtent) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual void Resize(IDevice* device, ISurface* surface, Extent2D newExtent) = 0;

		virtual EAcquireResult Acquire(IDevice* device, uint32_t frame) = 0;
		virtual EPresentResult Present(IDevice* device, IQueue* queue) = 0;

		virtual EImageFormat GetSwapChainImageFormat() = 0;
		Extent2D GetSwapChainExtent() const { return swapChainExtent; };
		virtual IImage* GetCurrentImage() = 0;

		virtual VulkanSwapChain& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanSwapChain"); }

	protected:

		Extent2D swapChainExtent;
		std::vector<IImage*> swapChainImages;
	};
}