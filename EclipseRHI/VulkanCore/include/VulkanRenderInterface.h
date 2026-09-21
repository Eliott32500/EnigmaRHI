#pragma once

#include "../../include/IRenderInterface.h"

#include "VulkanCommandPool.h"
#include "VulkanPipeline.h"
#include "VulkanSurface.h"
#include "VulkanSync.h"

class VulkanRenderInterface : public EclipseRHI::IRenderInterface
{
public:

	VulkanRenderInterface() = default;

	EclipseRHI::IBuffer* InstantiateBuffer() override { return new VulkanBuffer(); }
	EclipseRHI::ICommandBuffer* InstantiateCommandBuffer() override { return new VulkanCommandBuffer(); }
	EclipseRHI::ICommandPool* InstantiateCommandPool() override { return new VulkanCommandPool(); }
	EclipseRHI::IDescriptor* InstantiateDescriptor() override { return new VulkanDescriptor(); }
	EclipseRHI::IDevice* InstantiateDevice() override { return new VulkanDevice(); }
	EclipseRHI::IImage* InstantiateImage() override { return new VulkanImage(); }
	EclipseRHI::IInstance* InstantiateInstance() override { return new VulkanInstance(); }
	EclipseRHI::IPipeline* InstantiatePipeline() override { return new VulkanPipeline(); }
	EclipseRHI::IShaderModule* InstantiateShaderModule() override { return new VulkanShaderModule(); }
	EclipseRHI::IRenderPass* InstantiateRenderPass() override { return new VulkanRenderPass(); }
	EclipseRHI::ISurface* InstantiateSurface() override { return new VulkanSurface(); }
	EclipseRHI::ISwapChain* InstantiateSwapChain() override { return new VulkanSwapChain(); }
	EclipseRHI::ISync* InstantiateSync() override { return new VulkanSync(); }
};