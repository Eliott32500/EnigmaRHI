#pragma once

#include "IRenderInterface.h"
#include "VulkanCommandPool.h"
#include "VulkanPipeline.h"
#include "VulkanSurface.h"
#include "VulkanSync.h"

namespace EnigmaRHI
{
	class VulkanRenderInterface : public IRenderInterface
	{
	public:

		VulkanRenderInterface();
		~VulkanRenderInterface();

		//IBuffer* InstantiateBuffer() override { return new VulkanBuffer(); }
		ICommandBuffer* InstantiateCommandBuffer() override { return new VulkanCommandBuffer(); }
		ICommandPool* InstantiateCommandPool() override { return new VulkanCommandPool(); }
		IDescriptor* InstantiateDescriptor() override { return new VulkanDescriptor(); }
		IDevice* InstantiateDevice() override { return new VulkanDevice(); }
		//IImage* InstantiateImage() override { return new VulkanImage(); }
		IInstance* InstantiateInstance() override { return new VulkanInstance(); }
		IPipeline* InstantiatePipeline() override { return new VulkanPipeline(); }
		IShaderModule* InstantiateShaderModule() override { return new VulkanShaderModule(); }
		IRenderPass* InstantiateRenderPass() override { return new VulkanRenderPass(); }
		ISurface* InstantiateSurface() override { return new VulkanSurface(); }
		ISwapChain* InstantiateSwapChain() override { return new VulkanSwapChain(); }
		ISync* InstantiateSync() override { return new VulkanSync(); }
	};
}