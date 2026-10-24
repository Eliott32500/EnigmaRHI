// TextureLoader.cpp
#define STB_IMAGE_IMPLEMENTATION   // dans UN SEUL .cpp du projet
#include <stb_image.h>

#include "../include/texture.h"
#include "RHILog.h"

void TextureData::Free()
{
    if (pixels)
    {
        stbi_image_free(pixels);
        pixels = nullptr;
    }
    width = height = sourceChannels = 0;
    format = EnigmaRHI::EImageFormat::UNDEFINED;
}

TextureData LoadTexture(const std::string& path, const TextureLoadOptions& options)
{
    TextureData result;

    int w = 0, h = 0, channels = 0;
    if (!stbi_info(path.c_str(), &w, &h, &channels))
    {
        LOG_ERRORF(LogCategory::Vulkan, "LoadTexture: cannot read '{}' : {}",
            path.c_str(), stbi_failure_reason());
        return result;
    }

    if (w <= 0 || h <= 0)
    {
        LOG_ERRORF(LogCategory::Vulkan, "LoadTexture: '{}' has invalid size {}x{}",
            path.c_str(), w, h);
        return result;
    }

    stbi_set_flip_vertically_on_load_thread(options.flipVertically ? 1 : 0);

    const bool isHDR = options.hdr || stbi_is_hdr(path.c_str());

    if (isHDR)
    {
        result.pixels = stbi_loadf(path.c_str(), &w, &h, &channels, STBI_rgb_alpha);
        result.format = EnigmaRHI::EImageFormat::RGBA32_SFLOAT;
    }
    else
    {
        result.pixels = stbi_load(path.c_str(), &w, &h, &channels, STBI_rgb_alpha);
        result.format = (options.colorSpace == ETextureColorSpace::SRGB)
            ? EnigmaRHI::EImageFormat::RGBA8_SRGB
            : EnigmaRHI::EImageFormat::RGBA8_UNORM;
    }

    if (!result.pixels)
    {
        LOG_ERRORF(LogCategory::Vulkan, "LoadTexture: failed to decode '{}' : {}",
            path.c_str(), stbi_failure_reason());
        result.format = EnigmaRHI::EImageFormat::UNDEFINED;
        return result;
    }

    result.width = static_cast<uint32_t>(w);
    result.height = static_cast<uint32_t>(h);
    result.sourceChannels = static_cast<uint32_t>(channels);

    LOG_INFOF(LogCategory::Vulkan, "Texture loaded: '{}' ({}x{}, {} ch source, {})",
        path, w, h, channels, isHDR ? "HDR" : "LDR");

    return result;
}