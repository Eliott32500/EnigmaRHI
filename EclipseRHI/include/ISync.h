#pragma once

#include "IDevice.h"
#include "ICommandPool.h"
#include "IRenderPass.h"
#include "RHIConfig.h"


namespace EnigmaRHI
{
	class VulkanSync;

	class ISync
	{
	public:
		virtual ~ISync() = default;

		virtual void Create(IDevice* device) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual bool AquireNextImage(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) = 0;
		virtual void PresentFrame(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) = 0;
		
		virtual VulkanSync& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanSync"); }
	};
}