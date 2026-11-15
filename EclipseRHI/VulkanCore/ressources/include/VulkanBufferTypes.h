#pragma once

#include "BufferTypes.h"
#include "vulkan/vulkan.h"

namespace EnigmaRHI::Utilities
{
	VkBufferUsageFlags ToVkBufferUsage(EBufferUsage usage);
	VkMemoryPropertyFlags ToVkMemory(EMemoryAccess access);
}