#pragma once

namespace EnigmaRHI
{
    enum class ECompareOp
    {
        NEVER,
        LESS,
        EQUAL,
        LESS_OR_EQUAL,
        GREATER,
        GREATER_OR_EQUAL,
        NOT_EQUAL,
        ALWAYS
    };

    enum class EStencilFace
    {
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    enum class EStencilOp
    {
        KEEP,
        ZERO,
        REPLACE,
        INCREMENT_AND_CLAMP,
        DECREMENT_AND_CLAMP,
        INVERT,
        INCREMENT_AND_WRAP,
        DECREMENT_AND_WRAP
    };
}