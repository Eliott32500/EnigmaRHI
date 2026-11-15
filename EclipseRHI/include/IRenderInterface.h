#pragma once
#include "IBuffer.h"
#include "ICommandBuffer.h"
#include "ICommandPool.h"
#include "IDevice.h"
#include "IImage.h"
#include "IInstance.h"
#include "IPipeline.h"
#include "IShaderModule.h"
#include "ISurface.h"
#include "ISwapChain.h"


namespace EnigmaRHI
{
	class VulkanRenderInterface;

	enum ERenderAPI
	{
		Vulkan,
		DirectX,
		Metal,
	};

	class IRenderInterface
	{
	public:

		virtual ~IRenderInterface() = default;

		static IRenderInterface* CreateRenderInterface(ERenderAPI api);

		virtual ICommandBuffer* InstantiateCommandBuffer() = 0;
		virtual void DeleteCommandBuffer(ICommandBuffer* commandBuffer) { delete commandBuffer; }

		virtual ICommandPool* InstantiateCommandPool() = 0;
		virtual void DeleteCommandPool(ICommandPool* commandPool) { delete commandPool; }

		virtual IDevice* InstantiateDevice() = 0;
		virtual void DeleteDevice(IDevice* device) { delete device; }

		virtual IInstance* InstantiateInstance() = 0;
		virtual void DeleteInstance(IInstance* instance) { delete instance; }

		virtual IPipeline* InstantiatePipeline() = 0;
		virtual void DeletePipeline(IPipeline* pipeline) { delete pipeline; }

		virtual IShaderModule* InstantiateShaderModule() = 0;
		virtual void DeleteShaderModule(IShaderModule* shaderModule) { delete shaderModule; }

		virtual ISurface* InstantiateSurface() = 0;
		virtual void DeleteSurface(ISurface* surface) { delete surface; }

		virtual ISwapChain* InstantiateSwapChain() = 0;
		virtual void DeleteSwapChain(ISwapChain* swapChain) { delete swapChain; }
	};
}

