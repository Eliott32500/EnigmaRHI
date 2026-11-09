#pragma once

#include <cstdint>

namespace EnigmaRHI
{
    enum class EVertexInputRate
    {
        VERTEX,
        INSTANCE
    };

    enum class EVertexFormat
    {
        // Float
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,

        // Signed Integer
        INT,
        INT2,
        INT3,
        INT4,

        // Unsigned Integer
        UINT,
        UINT2,
        UINT3,
        UINT4,

        // Signed Byte
        BYTE4,

        // Unsigned Byte
        UBYTE4,

        // Normalized Unsigned Byte
        UBYTE4_NORM
    };

    struct VertexBindingDesc
    {
        uint32_t binding = 0;
        uint32_t stride = 0;
        EVertexInputRate inputRate = EVertexInputRate::VERTEX;
    };

    struct VertexAttributeDesc
    {
        uint32_t location = 0;
        uint32_t binding = 0;
        EVertexFormat format = EVertexFormat::FLOAT3;
        uint32_t offset = 0;
    };
}