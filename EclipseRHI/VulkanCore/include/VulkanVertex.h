#include "Vertex.h"
#include <array>
#include "vulkan/vulkan.h"

namespace EnigmaRHI
{
	struct VulkanVertex
	{
		static VkVertexInputBindingDescription GetBindingDescription();

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();
	};
}