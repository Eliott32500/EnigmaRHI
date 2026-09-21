#include "../include/VulkanUtilities.h"
#include <iostream>

VkFormat UtilitiesVulkan::FormatToVulkan(EclipseRHI::ImageFormat format)
{
	switch (format)
	{
		case EclipseRHI::ImageFormat::R8:
			return VK_FORMAT_R8_UNORM;

		case EclipseRHI::ImageFormat::RG8:
			return VK_FORMAT_R8G8_UNORM;

		case EclipseRHI::ImageFormat::RGB8:
			return VK_FORMAT_R8G8B8_UNORM;

		case EclipseRHI::ImageFormat::RGBA8:
			return VK_FORMAT_R8G8B8A8_UNORM;

		case EclipseRHI::ImageFormat::sRGBA8:
			return VK_FORMAT_R8G8B8A8_SRGB;

		case EclipseRHI::ImageFormat::D24_UNORM_S8_UINT:
			return VK_FORMAT_D24_UNORM_S8_UINT;

		case EclipseRHI::ImageFormat::D32_SFLOAT:
			return VK_FORMAT_D32_SFLOAT;

		case EclipseRHI::ImageFormat::D32_SFLOAT_S8_UINT:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;

		default:
			return VK_FORMAT_UNDEFINED;
	}
}

VkShaderStageFlags UtilitiesVulkan::ShaderStageToVulkan(EclipseRHI::ShaderStage stage)
{
	switch (stage)
	{
		case EclipseRHI::ShaderStage::Vertex:
			return VK_SHADER_STAGE_VERTEX_BIT;

		case EclipseRHI::ShaderStage::Fragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		case EclipseRHI::ShaderStage::Compute:
			return VK_SHADER_STAGE_COMPUTE_BIT;

		default:
			throw std::runtime_error("Unsupported ShaderStage value");
	}
}

EclipseRHI::ImageFormat UtilitiesVulkan::FormatFromVulkan(VkFormat format)
{
	switch (format)
	{
	case VK_FORMAT_R8G8B8A8_UNORM:
		return EclipseRHI::ImageFormat::RGBA8;

	case VK_FORMAT_R8G8B8A8_SRGB:
		return EclipseRHI::ImageFormat::sRGBA8;

	case VK_FORMAT_D24_UNORM_S8_UINT:
		return EclipseRHI::ImageFormat::D24_UNORM_S8_UINT;

	case VK_FORMAT_D32_SFLOAT:
		return EclipseRHI::ImageFormat::D32_SFLOAT;

	case VK_FORMAT_D32_SFLOAT_S8_UINT:
		return EclipseRHI::ImageFormat::D32_SFLOAT_S8_UINT;

	default:
		return EclipseRHI::ImageFormat::UNDEFINED;
	}
}
