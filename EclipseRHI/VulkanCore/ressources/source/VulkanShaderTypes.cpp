#include "VulkanShaderTypes.h"
#include "RHILog.h"

VkShaderStageFlags EnigmaRHI::Utilities::ToVulkanShaderType(EShaderType type)
{
    VkShaderStageFlags result = 0;

    if (HasFlag(type, EShaderType::VERTEX))                  result |= VK_SHADER_STAGE_VERTEX_BIT;
    if (HasFlag(type, EShaderType::TESSELLATION_CONTROL))    result |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    if (HasFlag(type, EShaderType::TESSELLATION_EVALUATION)) result |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    if (HasFlag(type, EShaderType::GEOMETRY))                result |= VK_SHADER_STAGE_GEOMETRY_BIT;
    if (HasFlag(type, EShaderType::FRAGMENT))                result |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if (HasFlag(type, EShaderType::COMPUTE))                 result |= VK_SHADER_STAGE_COMPUTE_BIT;

    return result;
}
