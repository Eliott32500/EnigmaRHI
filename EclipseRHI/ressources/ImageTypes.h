#pragma once

#include <cstdint>

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
    };

    enum class EImageViewType
    {
        VIEW_1D,
        VIEW_2D,
        VIEW_3D,

        VIEW_1D_ARRAY,
        VIEW_2D_ARRAY,

        VIEW_CUBE,
        VIEW_CUBE_ARRAY,
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

    enum class EImageUsage : uint8_t
    {
        NONE = 0,
        TRANSFER_SRC = 1 << 0,
        TRANSFER_DST = 1 << 1,
        SAMPLED = 1 << 2,
        STORAGE = 1 << 3,
        COLOR_ATTACHMENT = 1 << 4,
        DEPTH_STENCIL_ATTACHMENT = 1 << 5,
        TRANSIENT_ATTACHMENT = 1 << 6,
        INPUT_ATTACHMENT = 1 << 7,
    };

    constexpr EnigmaRHI::EImageUsage operator|(EnigmaRHI::EImageUsage lhs, EnigmaRHI::EImageUsage rhs)
    {
        return static_cast<EnigmaRHI::EImageUsage>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
    }

    constexpr EnigmaRHI::EImageUsage operator&(EnigmaRHI::EImageUsage lhs, EnigmaRHI::EImageUsage rhs)
    {
        return static_cast<EnigmaRHI::EImageUsage>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }

    constexpr EnigmaRHI::EImageUsage& operator|=(EnigmaRHI::EImageUsage& lhs, EnigmaRHI::EImageUsage rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    enum class EBorderColor
    {
        OPAQUE_INT_BLACK,
        OPAQUE_FLOAT_BLACK,
        OPAQUE_INT_WHITE,
        OPAQUE_FLOAT_WHITE,
        TRANSPARENT_INT_BLACK,
        TRANSPARENT_FLOAT_BLACK
    };
}