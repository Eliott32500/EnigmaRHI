#pragma once

namespace EnigmaRHI
{
    enum class EFramebufferTarget
    {
        DRAW,
        READ,
        READ_DRAW
    };

    enum class EAttachment
    {
        DEPTH_ATTACHMENT,
        STENCIL_ATTACHMENT,
        DEPTH_STENCIL_ATTACHMENT,
    };
}