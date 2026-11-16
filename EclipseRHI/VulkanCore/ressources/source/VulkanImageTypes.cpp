#include "VulkanImageTypes.h"
#include "RHILog.h"

VkFormat EnigmaRHI::Utilities::ToVulkanFormat(EImageFormat format)
{
    using E = EImageFormat;

    switch (format)
    {
    case E::UNDEFINED:return VK_FORMAT_UNDEFINED;
        //UNORM
    case E::R8_UNORM:               return VK_FORMAT_R8_UNORM;
    case E::RG8_UNORM:              return VK_FORMAT_R8G8_UNORM;
    case E::RGB8_UNORM:             return VK_FORMAT_R8G8B8_UNORM;
    case E::RGBA8_UNORM:            return VK_FORMAT_R8G8B8A8_UNORM;
    case E::BGRA8_UNORM:            return VK_FORMAT_B8G8R8A8_UNORM;
        //sRGB
    case E::R8_SRGB:                return VK_FORMAT_R8_SRGB;
    case E::RGBA8_SRGB:             return VK_FORMAT_R8G8B8A8_SRGB;
    case E::BGRA8_SRGB:             return VK_FORMAT_B8G8R8A8_SRGB;
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
    case E::BGRA8_UINT:             return VK_FORMAT_B8G8R8A8_UINT;
        //Depth
    case E::D16_UNORM:              return VK_FORMAT_D16_UNORM;
    case E::D16_UNORM_S8_UINT:      return VK_FORMAT_D16_UNORM_S8_UINT;
    case E::D24_UNORM_S8_UINT:      return VK_FORMAT_D24_UNORM_S8_UINT;
    case E::D32_SFLOAT:             return VK_FORMAT_D32_SFLOAT;
    case E::D32_SFLOAT_S8_UINT:     return VK_FORMAT_D32_SFLOAT_S8_UINT;
    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan format !");
    return VK_FORMAT_UNDEFINED;
}

VkImageType EnigmaRHI::Utilities::ToVulkanImageType(EImageType type)
{
    switch (type)
    {
        case EImageType::TYPE_1D:       return VK_IMAGE_TYPE_1D;
        case EImageType::TYPE_2D:       return VK_IMAGE_TYPE_2D;
        case EImageType::TYPE_3D:       return VK_IMAGE_TYPE_3D;
    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan image type !");
    return VK_IMAGE_TYPE_2D;
}

VkImageViewType EnigmaRHI::Utilities::ToVulkanImageViewType(EImageViewType viewType)
{
    using E = EImageViewType;

    switch (viewType)
    {
    case E::VIEW_1D:                return VK_IMAGE_VIEW_TYPE_1D;
    case E::VIEW_2D:                return VK_IMAGE_VIEW_TYPE_2D;
    case E::VIEW_3D:                return VK_IMAGE_VIEW_TYPE_3D;
    case E::VIEW_1D_ARRAY:          return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
    case E::VIEW_2D_ARRAY:          return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    case E::VIEW_CUBE:              return VK_IMAGE_VIEW_TYPE_CUBE;
    case E::VIEW_CUBE_ARRAY:        return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan image view type !");
    return VK_IMAGE_VIEW_TYPE_2D;
}

VkFilter EnigmaRHI::Utilities::ToVulkanFilter(EFilter mode)
{
    return mode == EFilter::NEAREST ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
}

VkSamplerMipmapMode EnigmaRHI::Utilities::ToVulkanMipmapMode(EMipmapMode mode)
{
    return mode == EMipmapMode::NEAREST ? VK_SAMPLER_MIPMAP_MODE_NEAREST : VK_SAMPLER_MIPMAP_MODE_LINEAR;
}

VkSamplerAddressMode EnigmaRHI::Utilities::ToVulkanWrapMode(EWrappingMode mode)
{
    using E = EWrappingMode;

    switch (mode)
    {
    case E::REPEAT:                     return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case E::CLAMP_TO_EDGE:              return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case E::CLAMP_TO_BORDER:            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case E::MIRRORED_REPEAT:            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case E::MIRROR_CLAMP_TO_EDGE:       return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan wrap mode !");
    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
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
    using E = EBorderColor;

    switch (borderColor)
    {
    case E::OPAQUE_INT_BLACK:            return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    case E::OPAQUE_FLOAT_BLACK:          return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    case E::OPAQUE_INT_WHITE:            return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
    case E::OPAQUE_FLOAT_WHITE:          return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    case E::TRANSPARENT_INT_BLACK:       return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    case E::TRANSPARENT_FLOAT_BLACK:     return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan border color !");
    return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
}

EnigmaRHI::EImageFormat EnigmaRHI::Utilities::FormatFromVulkan(VkFormat format)
{
    using E = EImageFormat;

    switch (format)
    {
    case VK_FORMAT_UNDEFINED:               return E::UNDEFINED;
    case VK_FORMAT_R8_UNORM:                return E::R8_UNORM;
    case VK_FORMAT_R8G8_UNORM:              return E::RG8_UNORM;
    case VK_FORMAT_R8G8B8_UNORM:            return E::RGB8_UNORM;
    case VK_FORMAT_R8G8B8A8_UNORM:          return E::RGBA8_UNORM;
    case VK_FORMAT_B8G8R8A8_UNORM:          return E::BGRA8_UNORM;
        // sRGB
    case VK_FORMAT_R8_SRGB:                 return E::R8_SRGB;
    case VK_FORMAT_R8G8B8A8_SRGB:           return E::RGBA8_SRGB;
    case VK_FORMAT_B8G8R8A8_SRGB:           return E::BGRA8_SRGB;
        // FLOAT16
    case VK_FORMAT_R16_SFLOAT:              return E::R16_SFLOAT;
    case VK_FORMAT_R16G16_SFLOAT:           return E::RG16_SFLOAT;
    case VK_FORMAT_R16G16B16_SFLOAT:        return E::RGB16_SFLOAT;
    case VK_FORMAT_R16G16B16A16_SFLOAT:     return E::RGBA16_SFLOAT;
        // FLOAT32
    case VK_FORMAT_R32_SFLOAT:              return E::R32_SFLOAT;
    case VK_FORMAT_R32G32_SFLOAT:           return E::RG32_SFLOAT;
    case VK_FORMAT_R32G32B32_SFLOAT:        return E::RGB32_SFLOAT;
    case VK_FORMAT_R32G32B32A32_SFLOAT:     return E::RGBA32_SFLOAT;
        // UINT
    case VK_FORMAT_R8_UINT:                 return E::R8_UINT;
    case VK_FORMAT_R8G8_UINT:               return E::RG8_UINT;
    case VK_FORMAT_R8G8B8_UINT:             return E::RGB8_UINT;
    case VK_FORMAT_R8G8B8A8_UINT:           return E::RGBA8_UINT;
    case VK_FORMAT_B8G8R8A8_UINT:           return E::BGRA8_UINT;
        // Depth
    case VK_FORMAT_D16_UNORM:               return E::D16_UNORM;
    case VK_FORMAT_D16_UNORM_S8_UINT:       return E::D16_UNORM_S8_UINT;
    case VK_FORMAT_D24_UNORM_S8_UINT:       return E::D24_UNORM_S8_UINT;
    case VK_FORMAT_D32_SFLOAT:              return E::D32_SFLOAT;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:      return E::D32_SFLOAT_S8_UINT;

    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan format input !");
    return EImageFormat::UNDEFINED;
}

uint32_t EnigmaRHI::Utilities::BytesPerPixel(EImageFormat format)
{
    using E = EImageFormat;

    switch (format)
    {
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
    case E::BGRA8_UNORM:
    case E::BGRA8_SRGB:
    case E::BGRA8_UINT:
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
    return format == EImageFormat::D16_UNORM_S8_UINT
        || format == EImageFormat::D24_UNORM_S8_UINT 
        || format == EImageFormat::D32_SFLOAT_S8_UINT;
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
    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan load operation !");
    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
}

VkAttachmentStoreOp EnigmaRHI::Utilities::ToVulkanStoreOp(EStoreOp storeOp)
{
    return storeOp == EStoreOp::STORE ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
}

VkImageLayout EnigmaRHI::Utilities::ToVulkanImageLayout(EImageLayout imageLayout)
{
    using E = EImageLayout;

    switch (imageLayout)
    {
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
    }

    LOG_ERROR(ELogCategory::VULKAN, "Unsupported vulkan image layout !");
    return VK_IMAGE_LAYOUT_UNDEFINED;
}

VkAccessFlags2 EnigmaRHI::Utilities::ToVulkanAccessFlag(EAccessFlag accessFlag)
{
    VkAccessFlags2 result = VK_ACCESS_2_NONE;

    using E = EAccessFlag;

    if (HasFlag(accessFlag, E::INDIRECT_COMMAND_READ))            result |= VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT;
    if (HasFlag(accessFlag, E::INDEX_READ))                       result |= VK_ACCESS_2_INDEX_READ_BIT;
    if (HasFlag(accessFlag, E::VERTEX_ATTRIBUTE_READ))            result |= VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
    if (HasFlag(accessFlag, E::UNIFORM_READ))                     result |= VK_ACCESS_2_UNIFORM_READ_BIT;
    if (HasFlag(accessFlag, E::INPUT_ATTACHMENT_READ))            result |= VK_ACCESS_2_INPUT_ATTACHMENT_READ_BIT;
    if (HasFlag(accessFlag, E::SHADER_READ))                      result |= VK_ACCESS_2_SHADER_READ_BIT;
    if (HasFlag(accessFlag, E::SHADER_WRITE))                     result |= VK_ACCESS_2_SHADER_WRITE_BIT;
    if (HasFlag(accessFlag, E::COLOR_ATTACHMENT_WRITE))           result |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
    if (HasFlag(accessFlag, E::DEPTH_STENCIL_ATTACHMENT_READ))    result |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    if (HasFlag(accessFlag, E::DEPTH_STENCIL_ATTACHMENT_WRITE))   result |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    if (HasFlag(accessFlag, E::TRANSFER_READ))                    result |= VK_ACCESS_2_TRANSFER_READ_BIT;
    if (HasFlag(accessFlag, E::TRANSFER_WRITE))                   result |= VK_ACCESS_2_TRANSFER_WRITE_BIT;
    if (HasFlag(accessFlag, E::HOST_READ))                        result |= VK_ACCESS_2_HOST_READ_BIT;
    if (HasFlag(accessFlag, E::HOST_WRITE))                       result |= VK_ACCESS_2_HOST_WRITE_BIT;
    if (HasFlag(accessFlag, E::MEMORY_READ))                      result |= VK_ACCESS_2_MEMORY_READ_BIT;
    if (HasFlag(accessFlag, E::MEMORY_WRITE))                     result |= VK_ACCESS_2_MEMORY_WRITE_BIT;
    if (HasFlag(accessFlag, E::COLOR_ATTACHMENT_READ))            result |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;

    return result;
}

VkPipelineStageFlags2 EnigmaRHI::Utilities::ToVulkanPipelineFlag(EPipelineStageFlag pipelineStateFlag)
{
    VkPipelineStageFlags2 result = VK_PIPELINE_STAGE_2_NONE;

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
