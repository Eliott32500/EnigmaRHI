#include "VulkanImageTypes.h"
#include "VulkanImageTypes.h"
#include "RHILog.h"

VkFormat EnigmaRHI::Utilities::ToVulkanFormat(EImageFormat format)
{
    switch (format)
    {
        using E = EImageFormat;

    case E::UNDEFINED:return VK_FORMAT_UNDEFINED;
        //UNORM
    case E::R8_UNORM:               return VK_FORMAT_R8_UNORM;
    case E::RG8_UNORM:              return VK_FORMAT_R8G8_UNORM;
    case E::RGB8_UNORM:             return VK_FORMAT_R8G8B8_UNORM;
    case E::RGBA8_UNORM:            return VK_FORMAT_R8G8B8A8_UNORM;
        //sRGB
    case E::R8_SRGB:                return VK_FORMAT_R8_SRGB;
    case E::RGBA8_SRGB:             return VK_FORMAT_R8G8B8A8_SRGB;
        //FLOAT16
    case E::R16_SFLOAT:             return VK_FORMAT_R16_SFLOAT;
    case E::RG16_SFLOAT:            return VK_FORMAT_R16G16_SFLOAT;
    case E::RGB16_SFLOAT:           return VK_FORMAT_R16G16B16_SFLOAT;
    case E::RGBA16_SFLOAT:          return VK_FORMAT_R16G16B16A16_SFLOAT;
        //FLOAT32
    case E::R32_SFLOAT:             return VK_FORMAT_R32_SFLOAT;
    case E::RG32_SFLOAT:            return VK_FORMAT_R32G32_SFLOAT;
    case E::RGB32_SFLOAT:           return VK_FORMAT_R32G32B32_SFLOAT;
    case E::RGBA32_SFLOAT:          return VK_FORMAT_R32G32B32A32_SFLOAT;
        //UINT
    case E::R8_UINT:                return VK_FORMAT_R8_UINT;
    case E::RG8_UINT:               return VK_FORMAT_R8G8_UINT;
    case E::RGB8_UINT:              return VK_FORMAT_R8G8B8_UINT;
    case E::RGBA8_UINT:             return VK_FORMAT_R8G8B8A8_UINT;
        //Depth
    case E::D24_UNORM_S8_UINT:      return VK_FORMAT_D24_UNORM_S8_UINT;
    case E::D32_SFLOAT:             return VK_FORMAT_D32_SFLOAT;
    case E::D32_SFLOAT_S8_UINT:     return VK_FORMAT_D32_SFLOAT_S8_UINT;
    default:                        LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan format !");
    }

}

VkImageType EnigmaRHI::Utilities::ToVulkanImageType(EImageType type)
{
    switch (type)
    {
        case EImageType::TYPE_1D:       return VK_IMAGE_TYPE_1D;
        case EImageType::TYPE_2D:       return VK_IMAGE_TYPE_2D;
        case EImageType::TYPE_3D:       return VK_IMAGE_TYPE_3D;
        default:                        LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan image type !");
    }
}

VkImageViewType EnigmaRHI::Utilities::ToVulkanImageViewType(EImageViewType viewType)
{
    switch (viewType)
    {
        using E = EImageViewType;

    case E::VIEW_1D:                return VK_IMAGE_VIEW_TYPE_1D;
    case E::VIEW_2D:                return VK_IMAGE_VIEW_TYPE_2D;
    case E::VIEW_3D:                return VK_IMAGE_VIEW_TYPE_3D;
    case E::VIEW_1D_ARRAY:          return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
    case E::VIEW_2D_ARRAY:          return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    case E::VIEW_CUBE:              return VK_IMAGE_VIEW_TYPE_CUBE;
    case E::VIEW_CUBE_ARRAY:        return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
    default:                        LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan image view type !");
    }
}

VkFilter EnigmaRHI::Utilities::ToVulkanFilter(EFilter mode)
{
    return mode == EFilter::NEAREST ? VK_FILTER_NEAREST : VK_FILTER_NEAREST;
}

VkSamplerMipmapMode EnigmaRHI::Utilities::ToVulkanMipmapMode(EMipmapMode mode)
{
    return mode == EMipmapMode::NEAREST ? VK_SAMPLER_MIPMAP_MODE_NEAREST : VK_SAMPLER_MIPMAP_MODE_LINEAR;
}

VkSamplerAddressMode EnigmaRHI::Utilities::ToVulkanWrapMode(EWrappingMode mode)
{
    switch (mode)
    {
        using E = EWrappingMode;

    case E::REPEAT:                     return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case E::CLAMP_TO_EDGE:              return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case E::CLAMP_TO_BORDER:            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case E::MIRRORED_REPEAT:            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case E::MIRROR_CLAMP_TO_EDGE:       return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
    default:                            LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan border color !");
    }
}

VkImageUsageFlags EnigmaRHI::Utilities::ToVulkanImageUsage(EImageUsage imageUsage)
{
    VkImageUsageFlags result = 0;

    if (HasFlag(imageUsage, EImageUsage::TRANSFER_SRC))             result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (HasFlag(imageUsage, EImageUsage::TRANSFER_DST))             result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (HasFlag(imageUsage, EImageUsage::SAMPLED))                  result |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (HasFlag(imageUsage, EImageUsage::STORAGE))                  result |= VK_IMAGE_USAGE_STORAGE_BIT;
    if (HasFlag(imageUsage, EImageUsage::COLOR_ATTACHMENT))         result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (HasFlag(imageUsage, EImageUsage::DEPTH_STENCIL_ATTACHMENT)) result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (HasFlag(imageUsage, EImageUsage::TRANSIENT_ATTACHMENT))     result |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    if (HasFlag(imageUsage, EImageUsage::INPUT_ATTACHMENT))         result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

    return result;
}

VkBorderColor EnigmaRHI::Utilities::ToVulkanBorderColor(EBorderColor borderColor)
{
    switch (borderColor)
    {
        using E = EBorderColor;

    case E::OPAQUE_INT_BLACK:            return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    case E::OPAQUE_FLOAT_BLACK:          return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    case E::OPAQUE_INT_WHITE:            return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
    case E::OPAQUE_FLOAT_WHITE:          return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    case E::TRANSPARENT_INT_BLACK:       return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    case E::TRANSPARENT_FLOAT_BLACK:     return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    default:                             LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan border color !");
    }
}

EnigmaRHI::EImageFormat EnigmaRHI::Utilities::FormatFromVulkan(VkFormat format)
{
    switch (format)
    {
    case VK_FORMAT_UNDEFINED:               return EImageFormat::UNDEFINED;
    case VK_FORMAT_R8_UNORM:                return EImageFormat::R8_UNORM;
    case VK_FORMAT_R8G8_UNORM:              return EImageFormat::RG8_UNORM;
    case VK_FORMAT_R8G8B8_UNORM:            return EImageFormat::RGB8_UNORM;
    case VK_FORMAT_R8G8B8A8_UNORM:          return EImageFormat::RGBA8_UNORM;

        // sRGB
    case VK_FORMAT_R8_SRGB:                 return EImageFormat::R8_SRGB;
    case VK_FORMAT_R8G8B8A8_SRGB:           return EImageFormat::RGBA8_SRGB;

        // FLOAT16
    case VK_FORMAT_R16_SFLOAT:              return EImageFormat::R16_SFLOAT;
    case VK_FORMAT_R16G16_SFLOAT:           return EImageFormat::RG16_SFLOAT;
    case VK_FORMAT_R16G16B16_SFLOAT:        return EImageFormat::RGB16_SFLOAT;
    case VK_FORMAT_R16G16B16A16_SFLOAT:     return EImageFormat::RGBA16_SFLOAT;

        // FLOAT32
    case VK_FORMAT_R32_SFLOAT:              return EImageFormat::R32_SFLOAT;
    case VK_FORMAT_R32G32_SFLOAT:           return EImageFormat::RG32_SFLOAT;
    case VK_FORMAT_R32G32B32_SFLOAT:        return EImageFormat::RGB32_SFLOAT;
    case VK_FORMAT_R32G32B32A32_SFLOAT:     return EImageFormat::RGBA32_SFLOAT;

        // UINT
    case VK_FORMAT_R8_UINT:                 return EImageFormat::R8_UINT;
    case VK_FORMAT_R8G8_UINT:               return EImageFormat::RG8_UINT;
    case VK_FORMAT_R8G8B8_UINT:             return EImageFormat::RGB8_UINT;
    case VK_FORMAT_R8G8B8A8_UINT:           return EImageFormat::RGBA8_UINT;

        // Depth
    case VK_FORMAT_D24_UNORM_S8_UINT:       return EImageFormat::D24_UNORM_S8_UINT;
    case VK_FORMAT_D32_SFLOAT:              return EImageFormat::D32_SFLOAT;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:      return EImageFormat::D32_SFLOAT_S8_UINT;

    default:                                LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan format input !");
    }
}

uint32_t EnigmaRHI::Utilities::BytesPerPixel(EImageFormat format)
{
    switch (format)
    {
        using E = EImageFormat;

    case E::R8_UNORM:
    case E::R8_SRGB:
    case E::R8_UINT:
        return 1;

    case E::RG8_UNORM:
    case E::RG8_UINT:
    case E::R16_SFLOAT:
    case E::D16_UNORM:
        return 2;

    case E::RGB8_UNORM:
    case E::RGB8_UINT:
        return 3;

    case E::RGBA8_UNORM:
    case E::RGBA8_SRGB:
    case E::RGBA8_UINT:
    case E::RG16_SFLOAT:
    case E::R32_SFLOAT:
    case E::D32_SFLOAT:
        return 4;

    case E::RGB16_SFLOAT:
        return 6;

    case E::RGBA16_SFLOAT:
    case E::RG32_SFLOAT:
        return 8;

    case E::RGB32_SFLOAT:
        return 12;
    case E::RGBA32_SFLOAT:
        return 16;

    case E::D24_UNORM_S8_UINT:
    case E::D32_SFLOAT_S8_UINT:
    case E::UNDEFINED:
    default:
        return 0;
    }
}

bool EnigmaRHI::Utilities::HasStencilComponent(EImageFormat format)
{
    return format == EImageFormat::D24_UNORM_S8_UINT || format == EImageFormat::D32_SFLOAT_S8_UINT;
}

bool EnigmaRHI::Utilities::HasDepthComponent(EImageFormat format)
{
    return HasStencilComponent(format) || 
        format == EImageFormat::D16_UNORM || format == EImageFormat::D32_SFLOAT;
}

VkAttachmentLoadOp EnigmaRHI::Utilities::ToVulkanLoadOp(ELoadOp loadOp)
{
    switch (loadOp)
    {
    case ELoadOp::LOAD:         return VK_ATTACHMENT_LOAD_OP_LOAD;
    case ELoadOp::CLEAR:        return VK_ATTACHMENT_LOAD_OP_CLEAR;
    case ELoadOp::DONT_CARE:    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    default:                    LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan load operation !");
    }
}

VkAttachmentStoreOp EnigmaRHI::Utilities::ToVulkanStoreOp(EStoreOp storeOp)
{
    return storeOp == EStoreOp::STORE ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
}

VkImageLayout EnigmaRHI::Utilities::ToVulkanImageLayout(EImageLayout imageLayout)
{
    switch (imageLayout)
    {
        using E = EImageLayout;
    case E::UNDEFINED:                              return VK_IMAGE_LAYOUT_UNDEFINED;
    case E::GENERAL:                                return VK_IMAGE_LAYOUT_GENERAL;
    case E::COLOR_ATTACHMENT_OPTIMAL:               return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case E::DEPTH_STENCIL_ATTACHMENT_OPTIMAL:       return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case E::DEPTH_STENCIL_READ_ONLY_OPTIMAL:        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case E::SHADER_READ_ONLY_OPTIMAL:               return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case E::TRANSFER_SRC_OPTIMAL:                   return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case E::TRANSFER_DST_OPTIMAL:                   return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case E::PRESENT_SRC:                            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    case E::DEPTH_ATTACHMENT_OPTIMAL:               return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    default:                                        LOG_ERROR(LogCategory::Vulkan, "Unsupported vulkan image layout !");
    }
}

VkAccessFlags EnigmaRHI::Utilities::ToVulkanAccessFlag(EAccessFlag accessFlag)
{
    VkAccessFlags result = 0;

    if (HasFlag(accessFlag, EAccessFlag::NONE))                              result |= VK_ACCESS_NONE;
    if (HasFlag(accessFlag, EAccessFlag::INDIRECT_COMMAND_READ))             result |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::INDEX_READ))                        result |= VK_ACCESS_INDEX_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::VERTEX_ATTRIBUTE_READ))             result |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::UNIFORM_READ))                      result |= VK_ACCESS_UNIFORM_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::INPUT_ATTACHMENT_READ))             result |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::SHADER_READ))                       result |= VK_ACCESS_SHADER_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::SHADER_WRITE))                      result |= VK_ACCESS_SHADER_WRITE_BIT;
    if (HasFlag(accessFlag, EAccessFlag::COLOR_ATTACHMENT_WRITE))            result |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    if (HasFlag(accessFlag, EAccessFlag::DEPTH_STENCIL_ATTACHMENT_READ))     result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::DEPTH_STENCIL_ATTACHMENT_WRITE))    result |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    if (HasFlag(accessFlag, EAccessFlag::TRANSFER_READ))                     result |= VK_ACCESS_TRANSFER_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::TRANSFER_WRITE))                    result |= VK_ACCESS_TRANSFER_WRITE_BIT;
    if (HasFlag(accessFlag, EAccessFlag::HOST_READ))                         result |= VK_ACCESS_HOST_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::HOST_WRITE))                        result |= VK_ACCESS_HOST_WRITE_BIT;
    if (HasFlag(accessFlag, EAccessFlag::MEMORY_READ))                       result |= VK_ACCESS_MEMORY_READ_BIT;
    if (HasFlag(accessFlag, EAccessFlag::MEMORY_WRITE))                      result |= VK_ACCESS_MEMORY_WRITE_BIT;

    return result;
}

VkPipelineStageFlagBits2 EnigmaRHI::Utilities::ToVulkanPipelineFlag(EPipelineStageFlag pipelineStateFlag)
{
    VkPipelineStageFlagBits2 result = 0;

    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::NONE))                               result |= VK_PIPELINE_STAGE_2_NONE;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::TOP_OF_PIPE))                        result |= VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::VERTEX_SHADER))                      result |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::EARLY_FRAGMENT_TESTS))               result |= VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::FRAGMENT_SHADER))                    result |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::LATE_FRAGMENT_TESTS))                result |= VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::COLOR_ATTACHMENT_OUTPUT))            result |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::COMPUTE_SHADER))                     result |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::TRANSFER))                           result |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::BOTTOM_OF_PIPE))                     result |= VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
    if (HasFlag(pipelineStateFlag, EPipelineStageFlag::ALL_COMMANDS))                       result |= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;

    return result;
}

EnigmaRHI::Utilities::LayoutSync EnigmaRHI::Utilities::GetLayoutSync(VkImageLayout layout, bool asSource)
{
    switch (layout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:                             return { VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_2_NONE };
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:                       return asSource ? LayoutSync{ VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_2_NONE } : LayoutSync{ VK_PIPELINE_STAGE_2_NONE, VK_ACCESS_2_NONE };
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:                  return { VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_ACCESS_2_TRANSFER_WRITE_BIT };
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:                  return { VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_ACCESS_2_TRANSFER_READ_BIT };
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:              return { VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT, VK_ACCESS_2_SHADER_SAMPLED_READ_BIT };
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:              return { VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT };
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:      return { VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT, 
                                                                            VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT };
    default:                                                    return { VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,  VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT };
    }
}
