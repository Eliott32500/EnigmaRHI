#pragma once
#include "RHIBitMask.h"

namespace EnigmaRHI
{
    enum class EShaderType : uint32_t
    {
        NONE = 0,
        VERTEX = 1 << 0,
        TESSELLATION_CONTROL = 1 << 1,
        TESSELLATION_EVALUATION = 1 << 2,
        GEOMETRY = 1 << 3,
        FRAGMENT = 1 << 4,
        COMPUTE = 1 << 5,

        GRAPHICS = VERTEX | TESSELLATION_CONTROL | TESSELLATION_EVALUATION | GEOMETRY | FRAGMENT,
        ALL = GRAPHICS | COMPUTE,
    };
    ENIGMA_ENABLE_BITMASK_OPERATORS(EShaderType);
}