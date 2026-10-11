#pragma once
#include <cstdint>
#include "CommandTypes.h"
#include <iostream>

namespace EnigmaRHI
{
    class VulkanQueue;

    class IQueue
    {
    public:
        virtual ~IQueue() = default;

        virtual ECommandQueueType GetType() const = 0;
        virtual uint32_t FamilyIndex() const = 0;
        virtual void WaitIdle() const = 0;

        virtual VulkanQueue& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanQueue"); }
    };
}