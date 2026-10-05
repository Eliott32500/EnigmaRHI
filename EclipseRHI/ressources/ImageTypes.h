#pragma once

namespace EnigmaRHI
{
    enum class EImageFormat
    {
        UNDEFINED,

        //UNORM
        R8_UNORM,
        RG8_UNORM,
        RGB8_UNORM,
        RGBA8_UNORM,

        //SRGB
        R8_SRGB,
        RGBA8_SRGB,

        //FLOAT16
        R16_SFLOAT,
        RG16_SFLOAT,
        RGB16_SFLOAT,
        RGBA16_SFLOAT,

        //FLOAT32
        R32_SFLOAT,
        RG32_SFLOAT,
        RGB32_SFLOAT,
        RGBA32_SFLOAT,

        //UINT
        R8_UINT,
        RG8_UINT,
        RGB8_UINT,
        RGBA8_UINT,

        //Depth
        D16_UNORM,
        D24_UNORM_S8_UINT,
        D32_SFLOAT,
        D32_SFLOAT_S8_UINT,
    };

    enum class EImageType
    {
        TYPE_1D,
        TYPE_2D,
        TYPE_3D,
        TYPE_2D_ARRAY,
        TYPE_CUBE_MAP,
    };

    enum class EFilter
    {
        NEAREST,
        LINEAR
    };

    enum class EMipmapMode
    {
        NEAREST,
        LINEAR
    };

    enum class EWrappingMode
    {
        REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        MIRROR_CLAMP_TO_EDGE,
    };
}