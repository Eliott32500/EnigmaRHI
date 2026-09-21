#pragma once
#include "VulkanShaderModule.h"
#include "VulkanSwapChain.h"
#include "VulkanDescriptor.h"
#include "VulkanRenderPass.h"
#include "../../include/IPipeline.h"
#include "../include/VulkanVertex.h"


class VulkanPipeline : public EclipseRHI::IPipeline
{
public:

	VulkanPipeline() = default;

	void Create(EclipseRHI::IShaderModule* vertShader, EclipseRHI::IShaderModule* fragShader, EclipseRHI::IDevice* device, EclipseRHI::ISwapChain* swapchain, EclipseRHI::IRenderPass* renderPass, EclipseRHI::IDescriptor* descriptor) override;
	void Destroy(EclipseRHI::IDevice* device) override;

	VkPipeline GetGraphicsPipeline() const { return graphicsPipeline; }
	VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }

	VulkanPipeline& API_Vulkan() override { return (*this); }

private:

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
};