#include "VulkanRenderInterface.h"

EnigmaRHI::VulkanRenderInterface::VulkanRenderInterface()
{
    if (volkInitialize() != VK_SUCCESS)
        throw std::runtime_error("ERROR : Failed to initialize Vulkan");
}

EnigmaRHI::VulkanRenderInterface::~VulkanRenderInterface()
{
    volkFinalize();
}