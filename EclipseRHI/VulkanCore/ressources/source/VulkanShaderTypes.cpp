#include "VulkanShaderTypes.h"

VkShaderStageFlags EnigmaRHI::Utilities::ToVulkanShaderType(EShaderType type)
{
    switch (type)
    {
    case EShaderType::VERTEX:
        return VK_SHADER_STAGE_VERTEX_BIT;

    case EShaderType::FRAGMENT:
        return VK_SHADER_STAGE_FRAGMENT_BIT;

    case EShaderType::COMPUTE:
        return VK_SHADER_STAGE_COMPUTE_BIT;

    case EShaderType::GEOMETRY:
        return VK_SHADER_STAGE_GEOMETRY_BIT;

    default:
        return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
}
