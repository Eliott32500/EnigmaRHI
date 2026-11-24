#include "VulkanComputePipeline.h"
#include "VulkanShaderModule.h"
#include "VulkanPipelineTypes.h"
#include "RHILog.h"
#include "VulkanDeviceCapabilities.h"
#include "VulkanBindingLayout.h"
#include "VulkanImageTypes.h"

void EnigmaRHI::VulkanComputePipeline::Create(IShaderModule& shader, IDevice* device, IBindingLayout* layout)
{
	VkPipelineShaderStageCreateInfo shaderStage
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_COMPUTE_BIT,
		.module = shader.API_Vulkan().GetHandle(),
		.pName = shader.GetEntryName(),
	};

	VkDescriptorSetLayout setLayout = layout->API_Vulkan().GetHandle();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &setLayout,
	};

	if (vkCreatePipelineLayout(device->API_Vulkan().GetHandle(), &pipelineLayoutInfo, nullptr, &pipeline.layout) != VK_SUCCESS)
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create compute pipeline layout!");

	VkComputePipelineCreateInfo pipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.stage = shaderStage,
		.layout = nullptr,
	};

	if (vkCreateComputePipelines(device->API_Vulkan().GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.handle))
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create compute pipeline !");
}

void EnigmaRHI::VulkanComputePipeline::Destroy(IDevice* device)
{
	vkDestroyPipeline(device->API_Vulkan().GetHandle(), pipeline.handle, nullptr);
	vkDestroyPipelineLayout(device->API_Vulkan().GetHandle(), pipeline.layout, nullptr);
}
