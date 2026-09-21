#include "../include/IRenderInterface.h"
#include "../VulkanCore/include/VulkanRenderInterface.h"

EclipseRHI::IRenderInterface* EclipseRHI::IRenderInterface::CreateRenderInterface(ERenderAPI api)
{
	switch (api)
	{
		case ERenderAPI::Vulkan:
			return new VulkanRenderInterface();
			break;

		default:
			throw std::runtime_error("Unsupported API");
			break;
	}
}