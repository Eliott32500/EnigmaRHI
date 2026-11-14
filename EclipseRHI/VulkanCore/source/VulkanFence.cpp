#include "VulkanFence.h"

void EnigmaRHI::VulkanFence::Wait(uint64_t timeout)
{
}

bool EnigmaRHI::VulkanFence::IsSignaled() const
{
	return false;
}

void EnigmaRHI::VulkanFence::Reset()
{
}

uint64_t EnigmaRHI::VulkanFence::GetValue() const
{
	return IsSignaled() ? 1 : 0;;
}
