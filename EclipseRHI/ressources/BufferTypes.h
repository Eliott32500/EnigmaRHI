#pragma once
#include <cstdint>
#include <cstddef>
#include "RHIBitMask.h"

namespace EnigmaRHI
{
    enum class EMemoryAccess
    {
        GPU_ONLY,
        CPU_TO_GPU,
        GPU_TO_CPU,
    };

    enum class EBufferUsage : uint32_t
    {
        NONE = 0,
        VERTEX = 1 << 0,
        INDEX = 1 << 1,
        UNIFORM = 1 << 2,
        STORAGE = 1 << 3,
        TRANSFER_SRC = 1 << 4,
        TRANSFER_DST = 1 << 5,
        INDIRECT = 1 << 6,
    };
    ENIGMA_ENABLE_BITMASK_OPERATORS(EBufferUsage);
}