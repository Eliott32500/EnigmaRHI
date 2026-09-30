#pragma once

#include "VulkanDevice.h"
#include "../../include/IRenderPass.h"
#include "../include/VulkanSwapChain.h"
#include <array>

class VulkanRenderPass : public EnigmaRHI::IRenderPass
{
public:

	void Create(EnigmaRHI::IDevice* device, EnigmaRHI::ISwapChain* swapChain, EnigmaRHI::ImageFormat depthFormat) override;
	void Destroy(EnigmaRHI::IDevice* device) override;

	VkRenderPass GetRenderPass() const { return renderPass; }

	VulkanRenderPass& API_Vulkan() override { return (*this); }

private:

	VkRenderPass renderPass = VK_NULL_HANDLE;
};