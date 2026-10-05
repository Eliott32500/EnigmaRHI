#include "VulkanUtilities.h"
#include <iostream>

VkFormat EnigmaRHI::UtilitiesVulkan::FormatToVulkan(ImageFormat format)
{
	switch (format)
	{
		case ImageFormat::R8:
			return VK_FORMAT_R8_UNORM;

		case ImageFormat::RG8:
			return VK_FORMAT_R8G8_UNORM;

		case ImageFormat::RGB8:
			return VK_FORMAT_R8G8B8_UNORM;

		case ImageFormat::RGBA8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case ImageFormat::sRGBA8:
			return VK_FORMAT_R8G8B8A8_SRGB;

		case ImageFormat::D24_UNORM_S8_UINT:
			return VK_FORMAT_D24_UNORM_S8_UINT;

		case ImageFormat::D32_SFLOAT:
			return VK_FORMAT_D32_SFLOAT;

		case ImageFormat::D32_SFLOAT_S8_UINT:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;

		default:
			return VK_FORMAT_UNDEFINED;
	}
}

VkShaderStageFlags EnigmaRHI::UtilitiesVulkan::ShaderStageToVulkan(ShaderStage stage)
{
	switch (stage)
	{
		case ShaderStage::Vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;

		case ShaderStage::Fragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		case ShaderStage::Compute:
			return VK_SHADER_STAGE_COMPUTE_BIT;

		default:
			throw std::runtime_error("Unsupported ShaderStage value");
	}
}

EnigmaRHI::ImageFormat EnigmaRHI::UtilitiesVulkan::FormatFromVulkan(VkFormat format)
{
	switch (format)
	{
	case VK_FORMAT_R8G8B8A8_UNORM:
		return ImageFormat::RGBA8;

	case VK_FORMAT_R8G8B8A8_SRGB:
		return ImageFormat::sRGBA8;

	case VK_FORMAT_D24_UNORM_S8_UINT:
		return ImageFormat::D24_UNORM_S8_UINT;

	case VK_FORMAT_D32_SFLOAT:
		return ImageFormat::D32_SFLOAT;

	case VK_FORMAT_D32_SFLOAT_S8_UINT:
		return ImageFormat::D32_SFLOAT_S8_UINT;

	default:
		return ImageFormat::UNDEFINED;
	}
}
