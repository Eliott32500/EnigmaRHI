#include "../include/VulkanUtilities.h"
#include <iostream>

VkFormat UtilitiesVulkan::FormatToVulkan(EnigmaRHI::ImageFormat format)
{
	switch (format)
	{
		case EnigmaRHI::ImageFormat::R8:
			return VK_FORMAT_R8_UNORM;

		case EnigmaRHI::ImageFormat::RG8:
			return VK_FORMAT_R8G8_UNORM;

		case EnigmaRHI::ImageFormat::RGB8:
			return VK_FORMAT_R8G8B8_UNORM;

		case EnigmaRHI::ImageFormat::RGBA8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case EnigmaRHI::ImageFormat::sRGBA8:
			return VK_FORMAT_R8G8B8A8_SRGB;

		case EnigmaRHI::ImageFormat::D24_UNORM_S8_UINT:
			return VK_FORMAT_D24_UNORM_S8_UINT;

		case EnigmaRHI::ImageFormat::D32_SFLOAT:
			return VK_FORMAT_D32_SFLOAT;

		case EnigmaRHI::ImageFormat::D32_SFLOAT_S8_UINT:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;

		default:
			return VK_FORMAT_UNDEFINED;
	}
}

VkShaderStageFlags UtilitiesVulkan::ShaderStageToVulkan(EnigmaRHI::ShaderStage stage)
{
	switch (stage)
	{
		case EnigmaRHI::ShaderStage::Vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;

		case EnigmaRHI::ShaderStage::Fragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		case EnigmaRHI::ShaderStage::Compute:
			return VK_SHADER_STAGE_COMPUTE_BIT;

		default:
			throw std::runtime_error("Unsupported ShaderStage value");
	}
}

EnigmaRHI::ImageFormat UtilitiesVulkan::FormatFromVulkan(VkFormat format)
{
	switch (format)
	{
	case VK_FORMAT_R8G8B8A8_UNORM:
		return EnigmaRHI::ImageFormat::RGBA8;

	case VK_FORMAT_R8G8B8A8_SRGB:
		return EnigmaRHI::ImageFormat::sRGBA8;

	case VK_FORMAT_D24_UNORM_S8_UINT:
		return EnigmaRHI::ImageFormat::D24_UNORM_S8_UINT;

	case VK_FORMAT_D32_SFLOAT:
		return EnigmaRHI::ImageFormat::D32_SFLOAT;

	case VK_FORMAT_D32_SFLOAT_S8_UINT:
		return EnigmaRHI::ImageFormat::D32_SFLOAT_S8_UINT;

	default:
		return EnigmaRHI::ImageFormat::UNDEFINED;
	}
}
