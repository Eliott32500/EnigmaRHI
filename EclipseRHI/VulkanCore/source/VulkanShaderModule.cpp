#include "VulkanShaderModule.h"

void EnigmaRHI::VulkanShaderModule::Create(IDevice* device, const std::string& filename)
{
	code = ReadShader(filename);
	module = CreateShaderModule(device->API_Vulkan().GetDevice());
}

VkShaderModule EnigmaRHI::VulkanShaderModule::CreateShaderModule(VkDevice vulkanDevice)
{
	VkShaderModuleCreateInfo createInfo
	{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data()),
	};

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(vulkanDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");

	return shaderModule;
}

std::vector<char> EnigmaRHI::VulkanShaderModule::ReadShader(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("failed to open file!");

	//Start at the end to read file size
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	//Go to file start to read file
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	// Close opened file and return shader data buffer
	file.close();

	return buffer;
}
