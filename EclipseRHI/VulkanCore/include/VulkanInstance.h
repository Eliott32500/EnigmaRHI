#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>
#include "volk.h"

#include <stdlib.h>
#include <vector>
#include <iostream>

#include "IInstance.h"

class VulkanInstance : public EnigmaRHI::IInstance
{
public:

	VulkanInstance() = default;

	void Create(EnigmaRHI::InstanceCreateInfo instanceInfo) override;
	void Destroy() override;

	void CreateDebugMessenger();
	void DestroyDebugMessenger();

	VulkanInstance& API_Vulkan() override { return (*this); }

	VkInstance GetInstance() const { return instance; }
	VkDebugUtilsMessengerEXT GetDebugMessenger() const { return debugMessenger; }

private:

	VkInstance instance{};

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	static constexpr uint32_t RequiredVulkanVersion = VK_MAKE_API_VERSION(0, 1, 3, 0);
	VkDebugUtilsMessengerEXT debugMessenger;

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif


	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	bool CheckValidationLayerSupport();
	void CheckSupportedVersion();
};
