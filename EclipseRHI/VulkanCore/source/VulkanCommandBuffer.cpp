#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanPipeline.h"
#include "RHILog.h"
#include "ISync.h"
#include "CommonTypes.h"

void EnigmaRHI::VulkanCommandBuffer::Create(IDevice* device, ICommandPool* commandPool)
{
	VkCommandBufferAllocateInfo allocInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool->API_Vulkan().GetHandle(),
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};

	if (vkAllocateCommandBuffers(device->API_Vulkan().GetHandle(), &allocInfo, &commandBuffer) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to allocate command buffers !");
}

void EnigmaRHI::VulkanCommandBuffer::Begin()
{
	VkCommandBufferBeginInfo beginInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0,
		.pInheritanceInfo = nullptr,
	};

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to begin command buffer!");
}

void EnigmaRHI::VulkanCommandBuffer::End()
{
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to end command buffer!");
}

void EnigmaRHI::VulkanCommandBuffer::Reset()
{
	if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS)
		LOG_FATAL(LogCategory::Vulkan, "Failed to reset command buffer!");
}

void EnigmaRHI::VulkanCommandBuffer::BeginRendering(
    const RenderingInfo& renderingInfo)
{
    std::vector<VkRenderingAttachmentInfo> colorAttachments;

    for (const auto& attachment : renderingInfo.colorAttachments)
    {
        VkRenderingAttachmentInfo colorAttachment
        {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .imageView = attachment.image->API_Vulkan().GetImageView(),
            .imageLayout = Utilities::ToVulkanImageLayout(attachment.imageLayout),
            .resolveMode = VK_RESOLVE_MODE_NONE,
            .resolveImageView = VK_NULL_HANDLE,
            .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .loadOp = Utilities::ToVulkanLoadOp(attachment.loadOp),
            .storeOp = Utilities::ToVulkanStoreOp(attachment.storeOp),
            .clearValue = {
                .color = {
                    {
                        attachment.clearColor.r,
                        attachment.clearColor.g,
                        attachment.clearColor.b,
                        attachment.clearColor.a
                    }
                }
            }
        };

        colorAttachments.push_back(colorAttachment);
    }

    VkRenderingInfo info
    {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea =
        {
            .offset =
            {
                renderingInfo.renderArea.offset.x,
                renderingInfo.renderArea.offset.y
            },
            .extent =
            {
                renderingInfo.renderArea.extent.width,
                renderingInfo.renderArea.extent.height
            }
        },
        .layerCount = 1,
        .viewMask = 0,
        .colorAttachmentCount = static_cast<uint32_t>(
            colorAttachments.size()
        ),
        .pColorAttachments = colorAttachments.data(),
        .pDepthAttachment = nullptr,
        .pStencilAttachment = nullptr
    };

    vkCmdBeginRendering(commandBuffer, &info);
}

void EnigmaRHI::VulkanCommandBuffer::EndRendering()
{
    vkCmdEndRendering(commandBuffer);
}

void EnigmaRHI::VulkanCommandBuffer::BindPipeline(IPipeline* pipeline)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->API_Vulkan().GetHandle());
}

void EnigmaRHI::VulkanCommandBuffer::BindDescriptorSet(IPipeline* pipeline, IDescriptor* descriptor, ISync* sync)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->API_Vulkan().GetPipelineLayout(), 0, 1, &descriptor->API_Vulkan().GetDescriptorSets()[sync->GetCurrentFrame()], 0, nullptr);
}

void EnigmaRHI::VulkanCommandBuffer::BindVertexBuffer(IBuffer* buffer)
{
    VkBuffer vertexBuffer = buffer->API_Vulkan().GetHandle();
    VkDeviceSize offset = buffer->API_Vulkan().bufferInfo.offset;

    vkCmdBindVertexBuffers(
        commandBuffer,
        0,
        1,
        &vertexBuffer,
        &offset
    );
}

void EnigmaRHI::VulkanCommandBuffer::BindIndexBuffer(IBuffer* buffer)
{
    vkCmdBindIndexBuffer(
        commandBuffer,
        buffer->API_Vulkan().GetHandle(),
        0,
        VK_INDEX_TYPE_UINT32
    );
}

void EnigmaRHI::VulkanCommandBuffer::DrawIndexed(
    uint32_t indexCount,
    uint32_t instanceCount)
{
    vkCmdDrawIndexed(
        commandBuffer,
        indexCount,
        instanceCount,
        0,
        0,
        0
    );
}

void EnigmaRHI::VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount)
{
    vkCmdDraw(commandBuffer, vertexCount, instanceCount, 0, 0);
}

void EnigmaRHI::VulkanCommandBuffer::TransitionImageLayout(IImage* image, EImageLayout oldLayout, EImageLayout newLayout,
    EAccessFlag srcAccessMask, EAccessFlag destAccessMask, EPipelineStageFlag srcStageMask,
    EPipelineStageFlag dstStageMask)
{
    VkImageMemoryBarrier2 barrier
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcStageMask = Utilities::ToVulkanPipelineFlag(srcStageMask),
        .srcAccessMask = Utilities::ToVulkanAccessFlag(srcAccessMask),
        .dstStageMask = Utilities::ToVulkanPipelineFlag(dstStageMask),
        .dstAccessMask = Utilities::ToVulkanAccessFlag(destAccessMask),
        .oldLayout = Utilities::ToVulkanImageLayout(oldLayout),
        .newLayout = Utilities::ToVulkanImageLayout(newLayout),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image->API_Vulkan().GetImage(),
        .subresourceRange = {
               .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
               .baseMipLevel = 0,
               .levelCount = 1,
               .baseArrayLayer = 0,
               .layerCount = 1} 
    };

    VkDependencyInfo dependencyInfo
    {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &barrier
    };

    vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}

void EnigmaRHI::VulkanCommandBuffer::SetViewport(const Viewport& viewport)
{
    VkViewport vkViewport
    {
        .x = viewport.x,
        .y = viewport.y,
        .width = viewport.width,
        .height = viewport.height,
        .minDepth = viewport.minDepth,
        .maxDepth = viewport.maxDepth
    };

    vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);
}

void EnigmaRHI::VulkanCommandBuffer::SetScissor(const Rect2D& scissor)
{
    VkRect2D vkScissor
    {
        .offset =
        {
            scissor.offset.x,
            scissor.offset.y
        },
        .extent =
        {
            scissor.extent.width,
            scissor.extent.height
        }
    };

    vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
}
