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

        case EImageType::TYPE_2D_ARRAY:
            return VK_IMAGE_TYPE_2D;

        case EImageType::TYPE_CUBE_MAP:
			return VK_IMAGE_TYPE_2D;
    }
}

VkFilter EnigmaRHI::Utilities::ToVulkanFilter(EFilter mode)
{
    switch (mode)
    {
        case EFilter::LINEAR:
            return VK_FILTER_LINEAR;

        case EFilter::NEAREST:
            return VK_FILTER_NEAREST;
	}
}

VkSamplerMipmapMode EnigmaRHI::Utilities::ToVulkanMipmapMode(EMipmapMode mode)
{
    switch (mode)
    {
    case EMipmapMode::LINEAR:
        return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case EMipmapMode::NEAREST:
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }
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
    }

    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
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
