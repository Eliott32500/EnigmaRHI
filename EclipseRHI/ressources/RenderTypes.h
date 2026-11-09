#pragma once
#include "RHIBitMask.h"

namespace EnigmaRHI
{
    enum class EDrawMode
    {
        POINTS,
        LINES,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP
    };

    enum class EMask : unsigned int
    {
        COLOR = 0x01,
        DEPTH = 0x02,
        STENCIL = 0x04,
        ALL = COLOR | DEPTH | STENCIL
    };
    ENIGMA_ENABLE_BITMASK_OPERATORS(EMask);

    enum class EColorMask : unsigned int
    {
        COLOR_NONE = 0,
        COLOR_R = 1 << 0,
        COLOR_G = 1 << 1,
        COLOR_B = 1 << 2,
        COLOR_A = 1 << 3,
        COLOR_ALL = COLOR_R | COLOR_G | COLOR_B | COLOR_A
    };
    ENIGMA_ENABLE_BITMASK_OPERATORS(EColorMask);
}