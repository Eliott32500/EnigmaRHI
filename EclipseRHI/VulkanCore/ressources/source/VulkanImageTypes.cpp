#include "VulkanImageTypes.h"
#include "VulkanImageTypes.h"
#include "VulkanImageTypes.h"
#include "VulkanImageTypes.h"
#include "VulkanImageTypes.h"
#include "VulkanImageTypes.h"

VkFormat EnigmaRHI::Utilities::ToVulkanFormat(EImageFormat format)
{
    switch (format)
    {
    case EImageFormat::UNDEFINED:
        return VK_FORMAT_UNDEFINED;

        //UNORM
    case EImageFormat::R8_UNORM:
        return VK_FORMAT_R8_UNORM;

    case EImageFormat::RG8_UNORM:
        return VK_FORMAT_R8G8_UNORM;

    case EImageFormat::RGB8_UNORM:
        return VK_FORMAT_R8G8B8_UNORM;

    case EImageFormat::RGBA8_UNORM:
        return VK_FORMAT_R8G8B8A8_UNORM;

        //sRGB
    case EImageFormat::R8_SRGB:
        return VK_FORMAT_R8_SRGB;

    case EImageFormat::RGBA8_SRGB:
        return VK_FORMAT_R8G8B8A8_SRGB;

        //FLOAT16
    case EImageFormat::R16_SFLOAT:
        return VK_FORMAT_R16_SFLOAT;

    case EImageFormat::RG16_SFLOAT:
        return VK_FORMAT_R16G16_SFLOAT;

    case EImageFormat::RGB16_SFLOAT:
        return VK_FORMAT_R16G16B16_SFLOAT;

    case EImageFormat::RGBA16_SFLOAT:
        return VK_FORMAT_R16G16B16A16_SFLOAT;

        //FLOAT32
    case EImageFormat::R32_SFLOAT:
        return VK_FORMAT_R32_SFLOAT;

    case EImageFormat::RG32_SFLOAT:
        return VK_FORMAT_R32G32_SFLOAT;

    case EImageFormat::RGB32_SFLOAT:
        return VK_FORMAT_R32G32B32_SFLOAT;

    case EImageFormat::RGBA32_SFLOAT:
        return VK_FORMAT_R32G32B32A32_SFLOAT;

        //UINT
    case EImageFormat::R8_UINT:
        return VK_FORMAT_R8_UINT;

    case EImageFormat::RG8_UINT:
        return VK_FORMAT_R8G8_UINT;

    case EImageFormat::RGB8_UINT:
        return VK_FORMAT_R8G8B8_UINT;

    case EImageFormat::RGBA8_UINT:
        return VK_FORMAT_R8G8B8A8_UINT;

        //Depth
    case EImageFormat::D24_UNORM_S8_UINT:
        return VK_FORMAT_D24_UNORM_S8_UINT;

    case EImageFormat::D32_SFLOAT:
        return VK_FORMAT_D32_SFLOAT;

    case EImageFormat::D32_SFLOAT_S8_UINT:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
    }

    return VK_FORMAT_UNDEFINED;
}

VkImageType EnigmaRHI::Utilities::ToVulkanImageType(EImageType type)
{
    switch (type)
    {
        case EImageType::TYPE_1D:
            return VK_IMAGE_TYPE_1D;

        case EImageType::TYPE_2D:
            return VK_IMAGE_TYPE_2D;

        case EImageType::TYPE_3D:
            return VK_IMAGE_TYPE_3D;

        default:
            return VK_IMAGE_TYPE_2D;
    }
}

VkImageViewType EnigmaRHI::Utilities::ToVulkanImageViewType(EImageViewType viewType)
{
    switch (viewType)
    {
    case EImageViewType::VIEW_1D:
        return VK_IMAGE_VIEW_TYPE_1D;

    case EImageViewType::VIEW_2D:
        return VK_IMAGE_VIEW_TYPE_2D;

    case EImageViewType::VIEW_3D:
        return VK_IMAGE_VIEW_TYPE_3D;

    case EImageViewType::VIEW_1D_ARRAY:
        return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

    case EImageViewType::VIEW_2D_ARRAY:
        return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

    case EImageViewType::VIEW_CUBE:
        return VK_IMAGE_VIEW_TYPE_CUBE;

    case EImageViewType::VIEW_CUBE_ARRAY:
        return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

    default:
        return VK_IMAGE_VIEW_TYPE_2D;
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
    case EWrappingMode::REPEAT:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;

    case EWrappingMode::CLAMP_TO_EDGE:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    case EWrappingMode::CLAMP_TO_BORDER:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

    case EWrappingMode::MIRRORED_REPEAT:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

    case EWrappingMode::MIRROR_CLAMP_TO_EDGE:
        return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;

    default:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
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
    case EBorderColor::OPAQUE_INT_BLACK:
        return VK_BORDER_COLOR_INT_OPAQUE_BLACK;

    case EBorderColor::OPAQUE_FLOAT_BLACK:
        return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

    case EBorderColor::OPAQUE_INT_WHITE:
        return VK_BORDER_COLOR_INT_OPAQUE_WHITE;

    case EBorderColor::OPAQUE_FLOAT_WHITE:
        return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

    case EBorderColor::TRANSPARENT_INT_BLACK:
        return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;

    case EBorderColor::TRANSPARENT_FLOAT_BLACK:
        return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

    default:
        return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    }
}

EnigmaRHI::EImageFormat EnigmaRHI::Utilities::FormatFromVulkan(VkFormat format)
{
    switch (format)
    {
    case VK_FORMAT_UNDEFINED:
        return EImageFormat::UNDEFINED;

        // UNORM
    case VK_FORMAT_R8_UNORM:
        return EImageFormat::R8_UNORM;

    case VK_FORMAT_R8G8_UNORM:
        return EImageFormat::RG8_UNORM;

    case VK_FORMAT_R8G8B8_UNORM:
        return EImageFormat::RGB8_UNORM;

    case VK_FORMAT_R8G8B8A8_UNORM:
        return EImageFormat::RGBA8_UNORM;

        // sRGB
    case VK_FORMAT_R8_SRGB:
        return EImageFormat::R8_SRGB;

    case VK_FORMAT_R8G8B8A8_SRGB:
        return EImageFormat::RGBA8_SRGB;

        // FLOAT16
    case VK_FORMAT_R16_SFLOAT:
        return EImageFormat::R16_SFLOAT;

    case VK_FORMAT_R16G16_SFLOAT:
        return EImageFormat::RG16_SFLOAT;

    case VK_FORMAT_R16G16B16_SFLOAT:
        return EImageFormat::RGB16_SFLOAT;

    case VK_FORMAT_R16G16B16A16_SFLOAT:
        return EImageFormat::RGBA16_SFLOAT;

        // FLOAT32
    case VK_FORMAT_R32_SFLOAT:
        return EImageFormat::R32_SFLOAT;

    case VK_FORMAT_R32G32_SFLOAT:
        return EImageFormat::RG32_SFLOAT;

    case VK_FORMAT_R32G32B32_SFLOAT:
        return EImageFormat::RGB32_SFLOAT;

    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return EImageFormat::RGBA32_SFLOAT;

        // UINT
    case VK_FORMAT_R8_UINT:
        return EImageFormat::R8_UINT;

    case VK_FORMAT_R8G8_UINT:
        return EImageFormat::RG8_UINT;

    case VK_FORMAT_R8G8B8_UINT:
        return EImageFormat::RGB8_UINT;

    case VK_FORMAT_R8G8B8A8_UINT:
        return EImageFormat::RGBA8_UINT;

        // Depth
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return EImageFormat::D24_UNORM_S8_UINT;

    case VK_FORMAT_D32_SFLOAT:
        return EImageFormat::D32_SFLOAT;

    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return EImageFormat::D32_SFLOAT_S8_UINT;

    default:
        return EImageFormat::UNDEFINED;
    }
}

uint32_t EnigmaRHI::Utilities::BytesPerPixel(EImageFormat format)
{
    switch (format)
    {
        // 8 bits / canal
    case EImageFormat::R8_UNORM:
    case EImageFormat::R8_SRGB:
    case EImageFormat::R8_UINT:
        return 1;

    case EImageFormat::RG8_UNORM:
    case EImageFormat::RG8_UINT:
        return 2;

    case EImageFormat::RGB8_UNORM:
    case EImageFormat::RGB8_UINT:
        return 3;

    case EImageFormat::RGBA8_UNORM:
    case EImageFormat::RGBA8_SRGB:
    case EImageFormat::RGBA8_UINT:
        return 4;

        // 16 bits / canal (float)
    case EImageFormat::R16_SFLOAT:
        return 2;
    case EImageFormat::RG16_SFLOAT:
        return 4;
    case EImageFormat::RGB16_SFLOAT:
        return 6;
    case EImageFormat::RGBA16_SFLOAT:
        return 8;

        // 32 bits / canal (float)
    case EImageFormat::R32_SFLOAT:
        return 4;
    case EImageFormat::RG32_SFLOAT:
        return 8;
    case EImageFormat::RGB32_SFLOAT:
        return 12;
    case EImageFormat::RGBA32_SFLOAT:
        return 16;

    case EImageFormat::D16_UNORM:
        return 2;
    case EImageFormat::D32_SFLOAT:
        return 4;

    case EImageFormat::D24_UNORM_S8_UINT:
    case EImageFormat::D32_SFLOAT_S8_UINT:
    case EImageFormat::UNDEFINED:
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
    case ELoadOp::LOAD:
        return VK_ATTACHMENT_LOAD_OP_LOAD;
    case ELoadOp::CLEAR:
        return VK_ATTACHMENT_LOAD_OP_CLEAR;
    case ELoadOp::DONT_CARE:
        return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    default:
        return VK_ATTACHMENT_LOAD_OP_CLEAR;
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
    case EImageLayout::UNDEFINED:
        return VK_IMAGE_LAYOUT_UNDEFINED;
    case EImageLayout::GENERAL:
        return VK_IMAGE_LAYOUT_GENERAL;
    case EImageLayout::COLOR_ATTACHMENT_OPTIMAL:
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case EImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case EImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case EImageLayout::SHADER_READ_ONLY_OPTIMAL:
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case EImageLayout::TRANSFER_SRC_OPTIMAL:
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case EImageLayout::TRANSFER_DST_OPTIMAL:
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case EImageLayout::PRESENT_SRC:
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    default:
        return VK_IMAGE_LAYOUT_UNDEFINED;;
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
