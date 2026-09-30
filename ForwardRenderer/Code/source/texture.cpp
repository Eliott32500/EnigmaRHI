#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../include/texture.h"

void Texture::LoadTexture(const char* filePath)
{
	data = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);

	switch (channels)
	{
	case 3: imageFormat = EnigmaRHI::ImageFormat::sRGBA8;
		break;
	}

	if (!data)
		throw std::runtime_error("failed to load texture image!");
}

void Texture::FreeTextureData()
{
	stbi_image_free(data);
}