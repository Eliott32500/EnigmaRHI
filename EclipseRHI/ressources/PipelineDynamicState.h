#pragma once

namespace EnigmaRHI
{
	enum class EDynamicState
	{
        VIEWPORT,
        SCISSOR,
        LINE_WIDTH,
        DEPTH_BIAS,
        BLEND_CONSTANTS,
        DEPTH_BOUNDS,
        STENCIL_COMPARE_MASK,
        STENCIL_WRITE_MASK,
        STENCIL_REFERENCE,
        CULL_MODE,
        FRONT_FACE,
        PRIMITIVE_TOPOLOGY,
        DEPTH_TEST_ENABLE,
        DEPTH_WRITE_ENABLE,
        DEPTH_COMPARE_OP,
        DEPTH_BOUNDS_TEST_ENABLE,
        STENCIL_TEST_ENABLE,
        STENCIL_OP,
        RASTERIZER_DISCARD_ENABLE,
        DEPTH_BIAS_ENABLE,
	};
}