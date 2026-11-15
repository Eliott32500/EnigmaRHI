#pragma once

namespace EnigmaRHI
{
    enum class ECullMode
    {
        NONE,
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    enum class EPolygonMode
    {
        FILL,
        LINE,
        POINT
    };

    enum class EFrontFaceMode
    {
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };

    enum class ETopologyMode
    {
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP
    };
}