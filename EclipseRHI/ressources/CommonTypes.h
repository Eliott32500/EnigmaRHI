#pragma once

#include <cstdint>

namespace EnigmaRHI
{
    struct Extent2D
    {
        uint32_t width;
        uint32_t height;

        Extent2D() { width = 0; height = 0; }
        Extent2D(uint32_t w, uint32_t h) { width = w; height = h; };
    };

    struct Extent3D
    {
        uint32_t width;
        uint32_t height;
        uint32_t depth;

        Extent3D() { width = 0; height = 0; depth = 0; }
        Extent3D(uint32_t w, uint32_t h, uint32_t d) { width = w; height = h; depth = d; };
        Extent3D(Extent2D e) { width = e.width; height = e.height; depth = 1; };
    };

    struct Offset2D
    {
        int32_t x;
        int32_t y;

        Offset2D() { x = 0; y = 0; }
        Offset2D(int32_t _x, int32_t _y) { x = _x; y = _y; };
    };

    struct Offset3D
    {
        int32_t x;
        int32_t y;
        int32_t z;

        Offset3D() { x = 0; y = 0; z = 0; }
        Offset3D(int32_t _x, int32_t _y, int32_t _z) { x = _x; y = _y; z = _z; };
    };
}