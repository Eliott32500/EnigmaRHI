#include "VulkanPipeline.h"
#include "VulkanPipelineTypes.h"
#include "RHILog.h"
#include "VulkanDeviceCapabilities.h"
#include "VulkanBindingLayout.h"
#include "VulkanImageTypes.h"

void EnigmaRHI::VulkanPipeline::CreateGraphicsPipeline(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout, EImageFormat colorFormat)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = CreateShaderInfo(shader, EPipelineType::GRAPHICS);

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

	if (vkCreatePipelineLayout(device->API_Vulkan().GetHandle(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
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
		.layout = pipelineLayout,
		.renderPass = nullptr,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional
		.basePipelineIndex = -1, // Optional
	};

	if (vkCreateGraphicsPipelines(device->API_Vulkan().GetHandle(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		LOG_FATAL(ELogCategory::VULKAN, "Failed to create vulkan pipeline !");
}

void EnigmaRHI::VulkanPipeline::CreateComputePipeline(const ComputePipelineDesc& gp, const Shader& shader, IDevice* device, IBindingLayout* layout)
{
	VkPipelineShaderStageCreateInfo shaderStage = CreateShaderInfo(shader, EPipelineType::COMPUTE)[0];

	VkDescriptorSetLayout setLayout = layout->API_Vulkan().GetHandle();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &setLayout,
	};

	if (vkCreatePipelineLayout(device->API_Vulkan().GetHandle(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create compute pipeline layout!");
	}

	VkComputePipelineCreateInfo pipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.stage = shaderStage,
		.layout = nullptr,
	};

	if (vkCreateComputePipelines(device->API_Vulkan().GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline))
		LOG_ERROR(ELogCategory::VULKAN, "Failed to create compute pipeline !");
}

void EnigmaRHI::VulkanPipeline::Destroy(IDevice* device)
{
	vkDestroyPipeline(device->API_Vulkan().GetHandle(), pipeline, nullptr);
	vkDestroyPipelineLayout(device->API_Vulkan().GetHandle(), pipelineLayout, nullptr);
}

std::vector<VkPipelineShaderStageCreateInfo> EnigmaRHI::VulkanPipeline::CreateShaderInfo(const Shader& shader, EPipelineType type)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	if (shader.vertShader)
	{
		if (type == EPipelineType::GRAPHICS)
		{
			VkPipelineShaderStageCreateInfo vertShaderStageInfo
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_VERTEX_BIT,
				.module = shader.vertShader->API_Vulkan().GetHandle(),
				.pName = shader.entryMethod,
			};

			shaderStages.push_back(vertShaderStageInfo);
		}
		else
			LOG_ERROR(ELogCategory::VULKAN, "Shader::vertShader ignored : a compute pipeline cannot contain a graphics stage");
	}

	if (shader.fragShader)
	{
		if (type == EPipelineType::GRAPHICS)
		{
			VkPipelineShaderStageCreateInfo fragShaderStageInfo
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = shader.fragShader->API_Vulkan().GetHandle(),
				.pName = shader.entryMethod,
			};

			shaderStages.push_back(fragShaderStageInfo);
		}
		else
			LOG_ERROR(ELogCategory::VULKAN, "Shader::fragShader ignored : a compute pipeline cannot contain a graphics stage");
	}

	if (shader.computeShader)
	{
		if (type == EPipelineType::COMPUTE)
		{
			VkPipelineShaderStageCreateInfo compShaderStageInfo
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_COMPUTE_BIT,
				.module = shader.computeShader->API_Vulkan().GetHandle(),
				.pName = shader.entryMethod,
			};

			shaderStages.push_back(compShaderStageInfo);
		}
		else
			LOG_ERROR(ELogCategory::VULKAN, "Shader::computeShader ignored : a graphics pipeline cannot contain a compute stage");
	}

	if (shader.geomShader && !IDevice::GetCapabilities().features.geometryShader)
	{
		LOG_ERROR(ELogCategory::VULKAN, "Shader::geomShader ignored : geometryShader is not supported by this device");
	}
	else if (shader.geomShader)
	{
		if (type == EPipelineType::GRAPHICS)
		{
			VkPipelineShaderStageCreateInfo geomShaderStageInfo
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_GEOMETRY_BIT,
				.module = shader.geomShader->API_Vulkan().GetHandle(),
				.pName = shader.entryMethod,
			};

			shaderStages.push_back(geomShaderStageInfo);
		}
		else
			LOG_ERROR(ELogCategory::VULKAN, "Shader::geomShader ignored : a compute pipeline cannot contain a graphics stage");
	}

	return shaderStages;
}
