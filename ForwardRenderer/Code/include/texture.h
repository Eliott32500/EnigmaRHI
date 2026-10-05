#pragma once

#include "ImageTypes.h"
#include <iostream>

class Texture
{
public:

	Texture() = default;

	void LoadTexture(const char* filePath);

	void FreeTextureData();

	uint32_t GetWidth() const { return static_cast<uint32_t>(width); }
	uint32_t GetHeight() const { return static_cast<uint32_t>(height); }
	uint32_t GetChannels() const { return static_cast<uint32_t>(channels); }
	EnigmaRHI::EImageFormat GetImageFormat() const { return imageFormat; }
	unsigned char* GetData() const { return data; }

private:

	int width = 0;
	int height = 0;
	int channels = 0;
	EnigmaRHI::EImageFormat imageFormat = EnigmaRHI::EImageFormat::UNDEFINED;
	unsigned char* data = nullptr;
};