#include "../include/IRenderInterface.h"
#include "../VulkanCore/include/VulkanRenderInterface.h"

EnigmaRHI::IRenderInterface* EnigmaRHI::IRenderInterface::CreateRenderInterface(ERenderAPI api)
{
	switch (api)
	{
		case ERenderAPI::Vulkan:
			return new VulkanRenderInterface();

		default:
			throw std::runtime_error("Unsupported API");
	}
}