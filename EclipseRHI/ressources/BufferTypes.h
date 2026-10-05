#pragma once

namespace EnigmaRHI
{
    enum class EBufferType
    {
        VERTEX,
        INDEX,
        UNIFORM,
        STORAGE
    };

    enum class EBufferUsage
    {
        STATIC,
        DYNAMIC,
        STREAM
    };
}