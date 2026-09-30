#pragma once

namespace EnigmaRHI
{
	enum class ImageFormat
	{
		UNDEFINED,
		R8,
		RG8,
		RGB8,
		RGBA8,
		sRGBA8,
		D24_UNORM_S8_UINT,
		D32_SFLOAT,
		D32_SFLOAT_S8_UINT,
	};

	enum ShaderStage
	{
		Vertex,
		Fragment,
		Compute,
	};
}