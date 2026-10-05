#pragma once

#include <iostream>

#include "IDevice.h"
#include "IImage.h"
#include "IBuffer.h"

#include <vector>


namespace EnigmaRHI
{
	class VulkanDescriptor;

	class IDescriptor
	{
	public:

		struct FrameDescriptorInfo
		{
			std::vector<EnigmaRHI::IBuffer::DescriptorBufferInfo> buffers;
		};

		virtual ~IDescriptor() = default;

		virtual void Create(IDevice* device) = 0;
		virtual void Destroy(IDevice* device) = 0;

		virtual void CreateDescriptorSets(EnigmaRHI::IDevice* device, std::vector<EnigmaRHI::IDescriptor::FrameDescriptorInfo> bufferInfos) = 0;

		virtual void AddBufferBinding(uint32_t binding, ShaderStage stageFlags) = 0;
		virtual void AddImageBinding(uint32_t binding) = 0;

		virtual void AddImageInfo(EnigmaRHI::IImage* imageInfo) = 0;

		virtual VulkanDescriptor& API_Vulkan() { throw std::runtime_error("Bad API Call: object is not a VulkanDescriptor"); }
	};
}
