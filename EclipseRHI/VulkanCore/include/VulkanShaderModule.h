#pragma once
#include "VulkanDevice.h"
#include "IShaderModule.h"
#include <fstream>

class VulkanShaderModule : public EnigmaRHI::IShaderModule
{
public:

	VulkanShaderModule() = default;

	void Create(EnigmaRHI::IDevice* device, const std::string& filename) override;

	VkShaderModule GetModule() const { return module; }

	VulkanShaderModule& API_Vulkan() override { return (*this); }

private:

	std::vector<char> code;
	std::vector<char> ReadShader(const std::string& filename);

	VkShaderModule module = VK_NULL_HANDLE;
	VkShaderModule CreateShaderModule(VkDevice device);
};