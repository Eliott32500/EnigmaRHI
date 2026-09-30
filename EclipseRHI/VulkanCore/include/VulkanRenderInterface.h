#pragma once

#include "IRenderInterface.h"
#include "VulkanCommandPool.h"
#include "VulkanPipeline.h"
#include "VulkanSurface.h"
#include "VulkanSync.h"

class VulkanRenderInterface : public EnigmaRHI::IRenderInterface
{
public:

	VulkanRenderInterface();
	~VulkanRenderInterface();

	EnigmaRHI::IBuffer* InstantiateBuffer() override { return new VulkanBuffer(); }
	EnigmaRHI::ICommandBuffer* InstantiateCommandBuffer() override { return new VulkanCommandBuffer(); }
	EnigmaRHI::ICommandPool* InstantiateCommandPool() override { return new VulkanCommandPool(); }
	EnigmaRHI::IDescriptor* InstantiateDescriptor() override { return new VulkanDescriptor(); }
	EnigmaRHI::IDevice* InstantiateDevice() override { return new VulkanDevice(); }
	EnigmaRHI::IImage* InstantiateImage() override { return new VulkanImage(); }
	EnigmaRHI::IInstance* InstantiateInstance() override { return new VulkanInstance(); }
	EnigmaRHI::IPipeline* InstantiatePipeline() override { return new VulkanPipeline(); }
	EnigmaRHI::IShaderModule* InstantiateShaderModule() override { return new VulkanShaderModule(); }
	EnigmaRHI::IRenderPass* InstantiateRenderPass() override { return new VulkanRenderPass(); }
	EnigmaRHI::ISurface* InstantiateSurface() override { return new VulkanSurface(); }
	EnigmaRHI::ISwapChain* InstantiateSwapChain() override { return new VulkanSwapChain(); }
	EnigmaRHI::ISync* InstantiateSync() override { return new VulkanSync(); }
};