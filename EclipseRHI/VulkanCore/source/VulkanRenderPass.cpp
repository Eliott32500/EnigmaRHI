#include "../include/VulkanRenderPass.h"
#include "../include/VulkanUtilities.h"


void VulkanRenderPass::Create(EclipseRHI::IDevice* device, EclipseRHI::ISwapChain* swapChain, EclipseRHI::ImageFormat depthFormat)
{
	VkAttachmentDescription colorAttachment
	{
		.format = UtilitiesVulkan::FormatToVulkan(swapChain->GetSwapChainImageFormat()),
		.samples = VK_SAMPLE_COUNT_1_BIT,

		// What to do before Rendering for color and depth data
		// VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
		// VK_ATTACHMENT_LOAD_OP_CLEAR : Clear the values to a constant at the start
		// VK_ATTACHMENT_LOAD_OP_DONT_CARE : Existing contents are undefined; we don't care about them
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		// What to do after Rendering for color and depth data
		// VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memory and can be read later
		// VK_ATTACHMENT_STORE_OP_DONT_CARE : Contents of the framebuffer will be undefined after the rendering operation
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,

		// What to do before Rendering for stencil data
		// Same VK_ATTACHEMENT_..... as before
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		// What to do after Rendering for stencil data
		// Same VK_ATTACHEMENT_..... as before
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

		// Specifies which layout the image will have before the render pass begins
		// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
		// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : Images to be presented in the swap chain
		// VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL : Images to be used as destination for a memory copy operation
		// VK_IMAGE_LAYOUT_UNDEFINED : Don't care what previous layout the image was in
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		// Specifies the layout to automatically transition to when the render pass finishes
		// Same VK_IMAGE_... as before
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	};

	VkSubpassDependency dependency
	{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
	};

	VkAttachmentDescription depthAttachment
	{
		.format = UtilitiesVulkan::FormatToVulkan(depthFormat),
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};

	VkAttachmentReference colorAttachmentRef
	{
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};

	VkAttachmentReference depthAttachmentRef
	{
		.attachment = 1,
		.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};

	VkSubpassDescription subpass
	{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentRef,
		.pDepthStencilAttachment = &depthAttachmentRef,
	};

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

	VkRenderPassCreateInfo renderPassInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = static_cast<uint32_t>(attachments.size()),
		.pAttachments = attachments.data(),
		.subpassCount = 1,
		.pSubpasses = &subpass,
		.dependencyCount = 1,
		.pDependencies = &dependency,
	};

	if (vkCreateRenderPass(device->API_Vulkan().GetDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass!");

	swapChain->API_Vulkan().CreateSwapChainFramebuffers(&device->API_Vulkan(), renderPass);
}

void VulkanRenderPass::Destroy(EclipseRHI::IDevice* device)
{
	vkDestroyRenderPass(device->API_Vulkan().GetDevice(), renderPass, nullptr);
}

