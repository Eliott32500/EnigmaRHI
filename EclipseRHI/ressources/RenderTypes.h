#pragma once

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

    enum EMask : unsigned int
    {
        COLOR = 0x01,
        DEPTH = 0x02,
        STENCIL = 0x04,
        ALL = COLOR | DEPTH | STENCIL
    };
}