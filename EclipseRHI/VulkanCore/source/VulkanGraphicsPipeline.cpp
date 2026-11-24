#include "VulkanGraphicsPipeline.h"
#include "VulkanPipelineTypes.h"
#include "RHILog.h"
#include "VulkanDeviceCapabilities.h"
#include "VulkanBindingLayout.h"
#include "VulkanImageTypes.h"

void EnigmaRHI::VulkanGraphicsPipeline::Create(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout, EImageFormat colorFormat)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = CreateShaderInfo(shader);

	//NEW VERSION IMPLEMENTED

	std::vector<VkVertexInputBindingDescription> bindings;

	for (const auto& binding : gp.vertexBindings)
	{
		bindings.push_back({
			.binding = binding.binding,
			.stride = binding.stride,
			.inputRate = Utilities::ToVulkanVertexInputRate(binding.inputRate)
			});
	}

	std::vector<VkVertexInputAttributeDescription> attributes;

	for (const auto& attribute : gp.vertexAttributes)
	{
		attributes.push_back({
			.location = attribute.location,
			.binding = attribute.binding,
			.format = Utilities::ToVulkanVertexFormat(attribute.format),
			.offset = attribute.offset
			});
	}

	VkPipelineVertexInputStateCreateInfo vertexInputInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size()),
		.pVertexBindingDescriptions = bindings.data(),
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size()),
		.pVertexAttributeDescriptions = attributes.data()
	};

	std::vector<VkDynamicState> dynamicStates = Utilities::ToVulkanDynamicStates(gp.dynamicStates);

	VkPipelineDynamicStateCreateInfo dynamicState
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data(),
	};

	//Input Assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = Utilities::ToVulkanTopology(gp.topologyMode),
		.primitiveRestartEnable = VK_FALSE,
	};

	//Viewport
	VkPipelineViewportStateCreateInfo viewportState
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = gp.viewportCount,
		//.pViewports = &viewport,
		.scissorCount = gp.scissorCount,
		//.pScissors = &scissor,
	};

	//Rasterization
	VkPipelineRasterizationStateCreateInfo rasterizer = Utilities::ToVulkanRasterizationState(gp.rasterizerState);

	//Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling = Utilities::ToVulkanMultisampleState(gp.multisampleState);

	//Color blending
	EnigmaRHI::Utilities::VulkanColorBlendState colorBlending = Utilities::ToVulkanColorBlendState(gp.blendStates);

	///////// PIPELINE LAYOUT ////////  (Uniform in OGL)

	VkDescriptorSetLayout setLayout = layout->API_Vulkan().GetHandle();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &setLayout,
		.pushConstantRangeCount = 0, // Optional
		.pPushConstantRanges = nullptr, // Optional
	};

	if (vkCreatePipelineLayout(device->API_Vulkan().GetHandle(), &pipelineLayoutInfo, nullptr, &pipeline.layout) != VK_SUCCESS)
	{
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create pipeline layout !");
		return;
	}

	VkPipelineDepthStencilStateCreateInfo depthStencil = Utilities::ToVulkanDepthStencilPipelineState(gp.depthStencilState);

	const DeviceFeatures features = IDevice::GetCapabilities().features;

	if (rasterizer.polygonMode != VK_POLYGON_MODE_FILL && !features.fillModeNonSolid)
	{
		LOG_WARNING(ELogCategory::VULKAN, "fillModeNonSolid is not supported : polygon mode forced to FILL");
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	}

	if (rasterizer.lineWidth != 1.0f && !features.wideLines)
	{
		LOG_WARNING(ELogCategory::VULKAN, "wideLines is not supported : line width forced to 1.0");
		rasterizer.lineWidth = 1.0f;
	}

	if (multisampling.sampleShadingEnable && !features.sampleRateShading)
	{
		LOG_WARNING(ELogCategory::VULKAN, "sampleRateShading is not supported : sample shading disabled");
		multisampling.sampleShadingEnable = VK_FALSE;
	}

	if (depthStencil.depthBoundsTestEnable && !features.depthBounds)
	{
		LOG_WARNING(ELogCategory::VULKAN, "depthBounds is not supported : depth bounds test disabled");
		depthStencil.depthBoundsTestEnable = VK_FALSE;
	}

	VkFormat vkColorFormat = Utilities::ToVulkanFormat(colorFormat);

	const EImageFormat depthImageFormat = device->FindDepthFormat();
	VkFormat depthFormat = Utilities::ToVulkanFormat(depthImageFormat);
	VkFormat stencilFormat = Utilities::HasStencilComponent(depthImageFormat) ? depthFormat : VK_FORMAT_UNDEFINED;

	VkPipelineRenderingCreateInfoKHR renderingPipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR,
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &vkColorFormat,
		.depthAttachmentFormat = depthFormat,
		.stencilAttachmentFormat = stencilFormat,
	};

	VkGraphicsPipelineCreateInfo graphicsPipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &renderingPipelineInfo,
		// Only if the two last Optionnal parameters are non-NULL and non-equal -1 (Create a Graphics Pipeline based on another one)
		//.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT,
		.stageCount = static_cast<uint32_t>(shaderStages.size()),
		.pStages = shaderStages.data(),
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pDepthStencilState = &depthStencil,
		.pColorBlendState = &colorBlending.info,
		.pDynamicState = &dynamicState,
		.layout = pipeline.layout,
		.renderPass = nullptr,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional
		.basePipelineIndex = -1, // Optional
	};

	if (vkCreateGraphicsPipelines(device->API_Vulkan().GetHandle(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &pipeline.handle) != VK_SUCCESS)
		LOG_FATAL(ELogCategory::VULKAN, "Failed to create vulkan pipeline !");
}

void EnigmaRHI::VulkanGraphicsPipeline::Destroy(IDevice* device)
{
	VkDevice vkDevice = device->API_Vulkan().GetHandle();

	if (pipeline.handle != VK_NULL_HANDLE)
		vkDestroyPipeline(vkDevice, pipeline.handle, nullptr);

	if (pipeline.layout != VK_NULL_HANDLE)
		vkDestroyPipelineLayout(vkDevice, pipeline.layout, nullptr);

	pipeline.handle = VK_NULL_HANDLE;
	pipeline.layout = VK_NULL_HANDLE;
}

std::vector<VkPipelineShaderStageCreateInfo> EnigmaRHI::VulkanGraphicsPipeline::CreateShaderInfo(const Shader& shader)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	if (shader.vertShader)
	{
		VkPipelineShaderStageCreateInfo vertShaderStageInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = shader.vertShader->API_Vulkan().GetHandle(),
			.pName = shader.vertShader->GetEntryName(),
		};

		shaderStages.push_back(vertShaderStageInfo);
	}

	if (shader.fragShader)
	{
		VkPipelineShaderStageCreateInfo fragShaderStageInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = shader.fragShader->API_Vulkan().GetHandle(),
			.pName = shader.fragShader->GetEntryName(),
		};

		shaderStages.push_back(fragShaderStageInfo);
	}

	if (shader.geomShader && !IDevice::GetCapabilities().features.geometryShader)
	{
		LOG_ERROR(ELogCategory::VULKAN, "Shader::geomShader ignored : geometryShader is not supported by this device");
	}
	else if (shader.geomShader)
	{
		VkPipelineShaderStageCreateInfo geomShaderStageInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_GEOMETRY_BIT,
			.module = shader.geomShader->API_Vulkan().GetHandle(),
			.pName = shader.geomShader->GetEntryName(),
		};

		shaderStages.push_back(geomShaderStageInfo);
	}

	return shaderStages;
}
