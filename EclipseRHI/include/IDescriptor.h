#pragma once

#include <iostream>

#include "IDevice.h"
#include "IImage.h"
#include "IBuffer.h"

#include <vector>

class VulkanDescriptor;

namespace EclipseRHI
{
	class IDescriptor
	{
	public:

		struct FrameDescriptorInfo
		{
			std::vector<EclipseRHI::IBuffer::DescriptorBufferInfo> buffers;
		};

		virtual ~IDescriptor() = default;

		virtual void Create(IDevice* device) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual void CreateDescriptorSets(EclipseRHI::IDevice* device, std::vector<EclipseRHI::IDescriptor::FrameDescriptorInfo> bufferInfos) = 0;

		virtual void AddBufferBinding(uint32_t binding, ShaderStage stageFlags) = 0;
		virtual void AddImageBinding(uint32_t binding) = 0;

		virtual void AddImageInfo(EclipseRHI::IImage* imageInfo) = 0;

		virtual VulkanDescriptor& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanDescriptor"); }
	};
}
