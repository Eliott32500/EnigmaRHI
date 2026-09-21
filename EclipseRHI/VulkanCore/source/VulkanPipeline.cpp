#include "../include/VulkanPipeline.h"

void VulkanPipeline::Create(EclipseRHI::IShaderModule* vertShader, EclipseRHI::IShaderModule* fragShader, EclipseRHI::IDevice* device, EclipseRHI::ISwapChain* swapChain, EclipseRHI::IRenderPass* renderPass, EclipseRHI::IDescriptor* descriptor)
{
	VkPipelineShaderStageCreateInfo vertShaderStageInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vertShader->API_Vulkan().GetModule(),
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo fragShaderStageInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fragShader->API_Vulkan().GetModule(),
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


	//// DONE ////
	auto bindingDescription = VulkanVertex::GetBindingDescription();
	auto attributeDescriptions = VulkanVertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &bindingDescription,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
		.pVertexAttributeDescriptions = attributeDescriptions.data(),
	};
	///////

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

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

		//	Comment les vertex sont utilisés
		//	VK_PRIMITIVE_TOPOLOGY_POINT_LIST: points from vertices
		//	VK_PRIMITIVE_TOPOLOGY_LINE_LIST: line from every 2 vertices without reuse
		//	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP : the end vertex of every line is used as start vertex for the next line
		//	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : triangle from every 3 vertices without reuse
		//	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP : the second and third vertex of every triangle are used as first two vertices of the next triangle
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,

		.primitiveRestartEnable = VK_FALSE,
	};

	/////////////////////////////////////

	VkViewport viewport
	{
		.x = 0.0f,
		.y = 0.0f,
		.width = (float)swapChain->API_Vulkan().GetSwapChainExtent().width,
		.height = (float)swapChain->API_Vulkan().GetSwapChainExtent().height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	VkRect2D scissor
	{
		.offset = { 0, 0 },
		.extent = swapChain->API_Vulkan().GetSwapChainExtent(),
	};

	VkPipelineViewportStateCreateInfo viewportState
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor,
	};

	//////// PIPELINE RASTERIZATION //////// 

	VkPipelineRasterizationStateCreateInfo rasterizer
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,

		//	VK_POLYGON_MODE_FILL: fill the area of the polygon with fragments
		//	VK_POLYGON_MODE_LINE : polygon edges are drawn as lines
		//	VK_POLYGON_MODE_POINT : polygon vertices are drawn as points
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,

		//Bias
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f, // Optional
		.depthBiasClamp = 0.0f, // Optional
		.depthBiasSlopeFactor = 0.0f, // Optional

		//Epaisseur de ligne (pour par ex mode wireframe)
		.lineWidth = 1.0f,
	};

	///////////////////////////////////////////

	//////// MULTISAMPLIG ////////

	VkPipelineMultisampleStateCreateInfo multisampling
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
		.minSampleShading = 1.0f, // Optional
		.pSampleMask = nullptr, // Optional
		.alphaToCoverageEnable = VK_FALSE, // Optional
		.alphaToOneEnable = VK_FALSE, // Optional
	};

	////////////////////////////////

	//////// COLOR-BLENDING ////////

	VkPipelineColorBlendAttachmentState colorBlendAttachment
	{
		.blendEnable = VK_TRUE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA, // Optional
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, // Optional
		.colorBlendOp = VK_BLEND_OP_ADD, // Optional
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // Optional
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // Optional
		.alphaBlendOp = VK_BLEND_OP_ADD, // Optional
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	};

	VkPipelineColorBlendStateCreateInfo colorBlending
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY, // Optional
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment,
	};

	colorBlending.blendConstants[0] = 0.0f; //Optionnal
	colorBlending.blendConstants[1] = 0.0f;	//Optionnal
	colorBlending.blendConstants[2] = 0.0f;	//Optionnal
	colorBlending.blendConstants[3] = 0.0f;	//Optionnal

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
		vkDestroyShaderModule(device->API_Vulkan().GetDevice(), fragShader->API_Vulkan().GetModule(), nullptr);
		vkDestroyShaderModule(device->API_Vulkan().GetDevice(), vertShader->API_Vulkan().GetModule(), nullptr);
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkPipelineDepthStencilStateCreateInfo depthStencil
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
		.front = {}, // Optional
		.back = {}, // Optional
		.minDepthBounds = 0.0f, // Optional
		.maxDepthBounds = 1.0f, // Optional
	};

	VkGraphicsPipelineCreateInfo graphicsPipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		// Only if the two last Optionnal parameters are non-NULL and non-equal -1 (Create a Graphics Pipeline based on another one)
		//.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT,
		.stageCount = 2,
		.pStages = shaderStages,
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pDepthStencilState = &depthStencil,
		.pColorBlendState = &colorBlending,
		.pDynamicState = &dynamicState,
		.layout = pipelineLayout,
		.renderPass = renderPass->API_Vulkan().GetRenderPass(),
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional
		.basePipelineIndex = -1, // Optional
	};

	if (vkCreateGraphicsPipelines(device->API_Vulkan().GetDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");

	vkDestroyShaderModule(device->API_Vulkan().GetDevice(), fragShader->API_Vulkan().GetModule(), nullptr);
	vkDestroyShaderModule(device->API_Vulkan().GetDevice(), vertShader->API_Vulkan().GetModule(), nullptr);
}

void VulkanPipeline::Destroy(EclipseRHI::IDevice* device)
{
	vkDestroyPipeline(device->API_Vulkan().GetDevice(), graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device->API_Vulkan().GetDevice(), pipelineLayout, nullptr);
}
