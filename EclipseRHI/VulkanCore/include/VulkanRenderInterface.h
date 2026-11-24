#pragma once

#include "IRenderInterface.h"
#include "VulkanCommandPool.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanSurface.h"

namespace EnigmaRHI
{
	class VulkanRenderInterface : public IRenderInterface
	{
	public:

		VulkanRenderInterface();
		~VulkanRenderInterface();

		ICommandBuffer* InstantiateCommandBuffer() override { return new VulkanCommandBuffer(); }
		ICommandPool* InstantiateCommandPool() override { return new VulkanCommandPool(); }
		IDevice* InstantiateDevice() override { return new VulkanDevice(); }
		IInstance* InstantiateInstance() override { return new VulkanInstance(); }
		IGraphicsPipeline* InstantiateGraphicsPipeline() override { return new VulkanGraphicsPipeline(); }
		IShaderModule* InstantiateShaderModule() override { return new VulkanShaderModule(); }
		ISurface* InstantiateSurface() override { return new VulkanSurface(); }
		ISwapChain* InstantiateSwapChain() override { return new VulkanSwapChain(); }
	};
}