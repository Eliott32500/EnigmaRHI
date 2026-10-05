#include "VulkanUtilities.h"
#include <iostream>

VkFormat EnigmaRHI::UtilitiesVulkan::FormatToVulkan(EImageFormat format)
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

VkShaderStageFlags EnigmaRHI::UtilitiesVulkan::ShaderStageToVulkan(EShaderType stage)
{
	switch (stage)
	{
		case EShaderType::VERTEX:
			return VK_SHADER_STAGE_VERTEX_BIT;

		case EShaderType::FRAGMENT:
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		case EShaderType::COMPUTE:
			return VK_SHADER_STAGE_COMPUTE_BIT;

		default:
			throw std::runtime_error("Unsupported ShaderStage value");
	}
}

EnigmaRHI::EImageFormat EnigmaRHI::UtilitiesVulkan::FormatFromVulkan(VkFormat format)
{
	switch (format)
	{
	case VK_FORMAT_R8G8B8A8_UNORM:
		return EImageFormat::RGBA8_UNORM;

	case VK_FORMAT_R8G8B8A8_SRGB:
		return EImageFormat::RGBA8_SRGB;

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
