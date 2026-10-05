#pragma once

namespace EnigmaRHI
{
    enum class EBlendFactor
    {
        ZERO,
        ONE,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA
    };

    enum class EBlendOp
    {
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX
    };
}