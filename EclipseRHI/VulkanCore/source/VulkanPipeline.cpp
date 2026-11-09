#include "VulkanPipeline.h"
#include "VulkanPipelineTypes.h"
#include "RHILog.h"
#include "VulkanDeviceCapabilities.h"

void EnigmaRHI::VulkanPipeline::Create(const GraphicsPipelineDesc& gp, const Shader& shader, IDevice* device, ISwapChain* swapChain, IRenderPass* renderPass, IDescriptor* descriptor)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = CreateShaderInfo(shader);

	//// DONE ////
	auto bindingDescription = VulkanVertex::GetBindingDescription();
	auto attributeDescriptions = VulkanVertex::GetAttributeDescriptions();

	//NEW VERSION NON IMPLEMENTED
	
	//std::vector<VkVertexInputBindingDescription> bindings;
	//
	//for (const auto& binding : gp.vertexBindings)
	//{
	//	bindings.push_back({
	//		.binding = binding.binding,
	//		.stride = binding.stride,
	//		.inputRate = Utilities::ToVulkanVertexInputRate(binding.inputRate)
	//		});
	//}
	//
	//std::vector<VkVertexInputAttributeDescription> attributes;
	//
	//for (const auto& attribute : gp.vertexAttributes)
	//{
	//	attributes.push_back({
	//		.location = attribute.location,
	//		.binding = attribute.binding,
	//		.format = Utilities::ToVulkanVertexFormat(attribute.format),
	//		.offset = attribute.offset
	//		});
	//}
	//
	//VkPipelineVertexInputStateCreateInfo vertexInputInfo
	//{
	//	.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	//	.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size()),
	//	.pVertexBindingDescriptions = bindings.data(),
	//	.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size()),
	//	.pVertexAttributeDescriptions = attributes.data()
	//};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &bindingDescription,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
		.pVertexAttributeDescriptions = attributeDescriptions.data(),
	};
	///////

	std::vector<VkDynamicState> dynamicStates = Utilities::ToVulkanDynamicStates(gp.dynamicStates);

	VkPipelineDynamicStateCreateInfo dynamicState
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data(),
	};

	///////// INPUT ASSEMBLY ///////////

	VkPipelineInputAssemblyStateCreateInfo inputAssembly
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = Utilities::ToVulkanTopology(gp.topologyMode),
		.primitiveRestartEnable = VK_FALSE,
	};

	/////////////////////////////////////

	//VkViewport viewport
	//{
	//	.x = 0.0f,
	//	.y = 0.0f,
	//	.width = (float)swapChain->API_Vulkan().GetSwapChainExtent().width,
	//	.height = (float)swapChain->API_Vulkan().GetSwapChainExtent().height,
	//	.minDepth = 0.0f,
	//	.maxDepth = 1.0f,
	//};
	//
	//Extent2D extent = swapChain->GetSwapChainExtent();
	//
	//VkRect2D scissor
	//{
	//	.offset = { 0, 0 },
	//	.extent = {extent.width, extent.height},
	//};

	VkPipelineViewportStateCreateInfo viewportState
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = gp.viewportCount,
		//.pViewports = &viewport,
		.scissorCount = gp.scissorCount,
		//.pScissors = &scissor,
	};

	//////// PIPELINE RASTERIZATION //////// 

	VkPipelineRasterizationStateCreateInfo rasterizer = Utilities::ToVulkanRasterizationState(gp.rasterizerState);
	//{
	//	.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	//	.depthClampEnable = VK_FALSE,
	//	.rasterizerDiscardEnable = VK_FALSE,

	//	//	VK_POLYGON_MODE_FILL: fill the area of the polygon with fragments
	//	//	VK_POLYGON_MODE_LINE : polygon edges are drawn as lines
	//	//	VK_POLYGON_MODE_POINT : polygon vertices are drawn as points
	//	.polygonMode = VK_POLYGON_MODE_FILL,
	//	.cullMode = VK_CULL_MODE_BACK_BIT,
	//	.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,

	//	//Bias
	//	.depthBiasEnable = VK_FALSE,
	//	.depthBiasConstantFactor = 0.0f, // Optional
	//	.depthBiasClamp = 0.0f, // Optional
	//	.depthBiasSlopeFactor = 0.0f, // Optional

	//	//Epaisseur de ligne (pour par ex mode wireframe)
	//	.lineWidth = 1.0f,
	//};

	///////////////////////////////////////////

	//////// MULTISAMPLIG ////////

	VkPipelineMultisampleStateCreateInfo multisampling = Utilities::ToVulkanMultisampleState(gp.multisampleState);
	//{
	//	.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	//	.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	//	.sampleShadingEnable = VK_FALSE,
	//	.minSampleShading = 1.0f, // Optional
	//	.pSampleMask = nullptr, // Optional
	//	.alphaToCoverageEnable = VK_FALSE, // Optional
	//	.alphaToOneEnable = VK_FALSE, // Optional
	//};	

	////////////////////////////////

	//////// COLOR-BLENDING ////////

	//VkPipelineColorBlendAttachmentState colorBlendAttachment
	//{
	//	.blendEnable = VK_TRUE,
	//	.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA, // Optional
	//	.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, // Optional
	//	.colorBlendOp = VK_BLEND_OP_ADD, // Optional
	//	.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
	//	.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
	//	.alphaBlendOp = VK_BLEND_OP_ADD, // Optional
	//	.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	//};

	EnigmaRHI::Utilities::VulkanColorBlendState colorBlending = Utilities::ToVulkanColorBlendState(gp.blendStates);
	//{
	//	.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	//	.logicOpEnable = VK_FALSE,
	//	.logicOp = VK_LOGIC_OP_COPY, // Optional
	//	.attachmentCount = 1,
	//	.pAttachments = &colorBlendAttachment,
	//};
	//
	//colorBlending.blendConstants[0] = 0.0f; //Optionnal
	//colorBlending.blendConstants[1] = 0.0f;	//Optionnal
	//colorBlending.blendConstants[2] = 0.0f;	//Optionnal
	//colorBlending.blendConstants[3] = 0.0f;	//Optionnal

	////////////////////////////////


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

	///////////////////////////////////

	if (vkCreatePipelineLayout(device->API_Vulkan().GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		LOG_ERROR(LogCategory::Vulkan, "Failed to create pipeline layout !");
		return;
	}

	VkPipelineDepthStencilStateCreateInfo depthStencil = Utilities::ToVulkanDepthStencilPipelineState(gp.depthStencilState);
	//{
	//	.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
	//	.depthTestEnable = VK_TRUE,
	//	.depthWriteEnable = VK_TRUE,
	//	.depthCompareOp = VK_COMPARE_OP_LESS,
	//	.depthBoundsTestEnable = VK_FALSE,
	//	.stencilTestEnable = VK_FALSE,
	//	.front = {}, // Optional
	//	.back = {}, // Optional
	//	.minDepthBounds = 0.0f, // Optional
	//	.maxDepthBounds = 1.0f, // Optional
	//};

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
