#pragma once

#include "VulkanDevice.h"
#include "../../include/IRenderPass.h"
#include "../include/VulkanSwapChain.h"
#include <array>

class VulkanRenderPass : public EclipseRHI::IRenderPass
{
public:

	void Create(EclipseRHI::IDevice* device, EclipseRHI::ISwapChain* swapChain, EclipseRHI::ImageFormat depthFormat) override;
	void Destroy(EclipseRHI::IDevice* device) override;

	VkRenderPass GetRenderPass() const { return renderPass; }

	VulkanRenderPass& API_Vulkan() override { return (*this); }

private:

	VkRenderPass renderPass = VK_NULL_HANDLE;
};