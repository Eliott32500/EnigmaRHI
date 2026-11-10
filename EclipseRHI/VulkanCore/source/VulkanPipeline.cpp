#include "VulkanPipeline.h"
#include "VulkanPipelineTypes.h"
#include "RHILog.h"
#include "VulkanDeviceCapabilities.h"

void EnigmaRHI::VulkanPipeline::Create(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, IRenderPass* renderPass, IDescriptor* descriptor)
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

	VkDescriptorSetLayout setLayout = descriptor->API_Vulkan().GetDescriptorSetLayout();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &setLayout,
		.pushConstantRangeCount = 0, // Optional
		.pPushConstantRanges = nullptr, // Optional
	};

	if (vkCreatePipelineLayout(device->API_Vulkan().GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		LOG_ERROR(LogCategory::Vulkan, "Failed to create pipeline layout !");
		return;
	}

	VkPipelineDepthStencilStateCreateInfo depthStencil = Utilities::ToVulkanDepthStencilPipelineState(gp.depthStencilState);

	VkGraphicsPipelineCreateInfo graphicsPipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
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
		.renderPass = renderPass->API_Vulkan().GetRenderPass(),
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional
		.basePipelineIndex = -1, // Optional
	};

	if (vkCreateGraphicsPipelines(device->API_Vulkan().GetDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");
}

void EnigmaRHI::VulkanPipeline::Destroy(IDevice* device)
{
	vkDestroyPipeline(device->API_Vulkan().GetDevice(), graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device->API_Vulkan().GetDevice(), pipelineLayout, nullptr);
}

std::vector<VkPipelineShaderStageCreateInfo> EnigmaRHI::VulkanPipeline::CreateShaderInfo(const Shader& shader)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

	if (shader.vertShader)
	{
		VkPipelineShaderStageCreateInfo vertShaderStageInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = shader.vertShader->API_Vulkan().GetModule(),
			.pName = shader.entryMethod,
		};

		shaderStages.push_back(vertShaderStageInfo);
	}

	if (shader.fragShader)
	{
		VkPipelineShaderStageCreateInfo fragShaderStageInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = shader.fragShader->API_Vulkan().GetModule(),
			.pName = shader.entryMethod,
		};

		shaderStages.push_back(fragShaderStageInfo);
	}

	if (shader.computeShader)
	{
		VkPipelineShaderStageCreateInfo compShaderStageInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_COMPUTE_BIT,
			.module = shader.computeShader->API_Vulkan().GetModule(),
			.pName = shader.entryMethod,
		};

		shaderStages.push_back(compShaderStageInfo);
	}

	if (shader.geomShader)
	{
		VkPipelineShaderStageCreateInfo geomShaderStageInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_GEOMETRY_BIT,
			.module = shader.geomShader->API_Vulkan().GetModule(),
			.pName = shader.entryMethod,
		};

		shaderStages.push_back(geomShaderStageInfo);
	}

	return shaderStages;
}
