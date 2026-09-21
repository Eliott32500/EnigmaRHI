#pragma once

#include "IDevice.h"
#include "ICommandPool.h"
#include "IRenderPass.h"

class VulkanSync;

namespace EclipseRHI
{
	class ISync
	{
	public:
		virtual ~ISync() = default;

		virtual void Create(IDevice* device) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual void AquireNextImage(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) = 0;
		virtual void PresentFrame(IDevice* device, ISwapChain* swapChain, ICommandPool* commandPool, ISurface* surface, IRenderPass* renderPass, GLFWwindow* window, uint32_t* imageIndex) = 0;
		
		void MoveToNextFrame(IDevice* device) { currentFrame = (currentFrame + 1) % device->MAX_FRAMES_IN_FLIGHT; }
		uint32_t GetCurrentFrame() const { return currentFrame; }

		virtual VulkanSync& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanSync"); }

	protected:

		uint32_t currentFrame = 0;
	};
}