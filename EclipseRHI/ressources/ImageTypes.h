#pragma once

#include <cstdint>
#include "RHIBitMask.h"

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
    ENIGMA_ENABLE_BITMASK_OPERATORS(EImageUsage);

    enum class EBorderColor
    {
        OPAQUE_INT_BLACK,
        OPAQUE_FLOAT_BLACK,
        OPAQUE_INT_WHITE,
        OPAQUE_FLOAT_WHITE,
        TRANSPARENT_INT_BLACK,
        TRANSPARENT_FLOAT_BLACK
    };

    enum class EImageLayout
    {
        UNDEFINED,
        GENERAL,
        COLOR_ATTACHMENT_OPTIMAL,
        DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        DEPTH_STENCIL_READ_ONLY_OPTIMAL,
        SHADER_READ_ONLY_OPTIMAL,
        TRANSFER_SRC_OPTIMAL,
        TRANSFER_DST_OPTIMAL,
        PRESENT_SRC,
    };

    enum class ELoadOp
    {
        LOAD,
        CLEAR,
        DONT_CARE,
    };

    enum class EStoreOp
    {
        STORE,
        DONT_CARE,
    };

    enum class EAccessFlag
    {
        NONE = 0,
        INDIRECT_COMMAND_READ = 0x00000001,
        INDEX_READ = 0x00000002,
        VERTEX_ATTRIBUTE_READ = 0x00000004,
        UNIFORM_READ = 0x00000008,
        INPUT_ATTACHMENT_READ = 0x00000010,
        SHADER_READ = 0x00000020,
        SHADER_WRITE = 0x00000040,
        COLOR_ATTACHMENT_READ = 0x00000080,
        COLOR_ATTACHMENT_WRITE = 0x00000100,
        DEPTH_STENCIL_ATTACHMENT_READ = 0x00000200,
        DEPTH_STENCIL_ATTACHMENT_WRITE = 0x00000400,
        TRANSFER_READ = 0x00000800,
        TRANSFER_WRITE = 0x00001000,
        HOST_READ = 0x00002000,
        HOST_WRITE = 0x00004000,
        MEMORY_READ = 0x00008000,
        MEMORY_WRITE = 0x00010000,
    };
    ENIGMA_ENABLE_BITMASK_OPERATORS(EAccessFlag);

    enum class EPipelineStageFlag
    {
        NONE = 0,
        TOP_OF_PIPE = 1 << 0,
        VERTEX_SHADER = 1 << 1,
        EARLY_FRAGMENT_TESTS = 1 << 2,
        FRAGMENT_SHADER = 1 << 3,
        LATE_FRAGMENT_TESTS = 1 << 4,
        COLOR_ATTACHMENT_OUTPUT = 1 << 5,
        COMPUTE_SHADER = 1 << 6,
        TRANSFER = 1 << 7,
        BOTTOM_OF_PIPE = 1 << 8,
        ALL_COMMANDS = 1 << 9,
    };
    ENIGMA_ENABLE_BITMASK_OPERATORS(EPipelineStageFlag);
}