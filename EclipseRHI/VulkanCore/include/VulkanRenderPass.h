#pragma once

#include "VulkanDevice.h"
#include "IRenderPass.h"
#include "VulkanSwapChain.h"
#include <array>

namespace EnigmaRHI
{
	class VulkanRenderPass : public IRenderPass
	{
	public:

		void Create(IDevice* device, ISwapChain* swapChain, EImageFormat depthFormat) override;
		void Destroy(IDevice* device) override;

		VkRenderPass GetRenderPass() const { return renderPass; }

		VulkanRenderPass& API_Vulkan() override { return (*this); }

	private:

		VkRenderPass renderPass = VK_NULL_HANDLE;
	};
}