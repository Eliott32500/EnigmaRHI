#pragma once

#include <vulkan/vulkan.h>

namespace EnigmaRHI
{
    struct VulkanPipeline
    {
        VkPipeline handle = VK_NULL_HANDLE;
        VkPipelineLayout layout = VK_NULL_HANDLE;
        VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    };
}