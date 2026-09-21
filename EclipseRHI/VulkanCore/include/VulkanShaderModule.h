#pragma once
#include "VulkanDevice.h"
#include "../../../EclipseRHI/include/IShaderModule.h"
#include <fstream>

class VulkanShaderModule : public EclipseRHI::IShaderModule
{
public:

	VulkanShaderModule() = default;

	void Create(EclipseRHI::IDevice* device, const std::string& filename) override;

	VkShaderModule GetModule() const { return module; }

	VulkanShaderModule& API_Vulkan() override { return (*this); }

private:

	std::vector<char> code;
	std::vector<char> ReadShader(const std::string& filename);

	VkShaderModule module = VK_NULL_HANDLE;
	VkShaderModule CreateShaderModule(VkDevice device);
};