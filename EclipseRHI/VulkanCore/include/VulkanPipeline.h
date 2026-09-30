#pragma once
#include "VulkanShaderModule.h"
#include "VulkanSwapChain.h"
#include "VulkanDescriptor.h"
#include "VulkanRenderPass.h"
#include "../../include/IPipeline.h"
#include "../include/VulkanVertex.h"


class VulkanPipeline : public EnigmaRHI::IPipeline
{
public:

	VulkanPipeline() = default;

	void Create(EnigmaRHI::IShaderModule* vertShader, EnigmaRHI::IShaderModule* fragShader, EnigmaRHI::IDevice* device, EnigmaRHI::ISwapChain* swapchain, EnigmaRHI::IRenderPass* renderPass, EnigmaRHI::IDescriptor* descriptor) override;
	void Destroy(EnigmaRHI::IDevice* device) override;

	VkPipeline GetGraphicsPipeline() const { return graphicsPipeline; }
	VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }

	VulkanPipeline& API_Vulkan() override { return (*this); }

private:

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
};