#include "VulkanRenderInterface.h"

VulkanRenderInterface::VulkanRenderInterface()
{
    if (volkInitialize() != VK_SUCCESS)
        throw std::runtime_error("ERROR : Failed to initialize Vulkan");
}

VulkanRenderInterface::~VulkanRenderInterface()
{
    volkFinalize();
}