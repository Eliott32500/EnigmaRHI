#pragma once

#include <cstdint>
#include <string>
#include "ImageTypes.h"

enum class ETextureColorSpace
{
    SRGB,
    LINEAR
};

struct TextureLoadOptions
{
    ETextureColorSpace colorSpace = ETextureColorSpace::SRGB;
    bool flipVertically = false;
    bool hdr = false;
};

class TextureData
{
public:
    TextureData() = default;
    ~TextureData() { Free(); }

    TextureData(const TextureData&) = delete;
    TextureData& operator=(const TextureData&) = delete;

    TextureData(TextureData&& o) noexcept { *this = std::move(o); }
    TextureData& operator=(TextureData&& o) noexcept
    {
        if (this != &o)
        {
            Free();
            pixels = o.pixels;  width = o.width;  height = o.height;
            format = o.format;  sourceChannels = o.sourceChannels;
            o.pixels = nullptr;
        }
        return *this;
    }

    bool IsValid() const { return pixels != nullptr; }
    void Free();

    void* pixels = nullptr;
    uint32_t     width = 0;
    uint32_t     height = 0;
    uint32_t     sourceChannels = 0;
    EnigmaRHI::EImageFormat format = EnigmaRHI::EImageFormat::UNDEFINED;
};

TextureData LoadTexture(const std::string& path, const TextureLoadOptions& options = {});