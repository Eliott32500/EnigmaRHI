set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

cmake_minimum_required(VERSION 3.0)
project(EclipseRHI)
file(COPY ForwardRenderer/Assets DESTINATION ${CMAKE_BINARY_DIR})

add_library(EclipseRHI 

"EclipseRHI/VulkanCore/include/VulkanBuffer.h" "EclipseRHI/VulkanCore/source/VulkanBuffer.cpp"
"EclipseRHI/VulkanCore/include/VulkanDevice.h" "EclipseRHI/VulkanCore/source/VulkanDevice.cpp" 
"EclipseRHI/VulkanCore/include/VulkanInstance.h" "EclipseRHI/VulkanCore/source/VulkanInstance.cpp"
"EclipseRHI/VulkanCore/include/VulkanSwapChain.h" "EclipseRHI/VulkanCore/source/VulkanSwapChain.cpp"   
"EclipseRHI/VulkanCore/include/VulkanDescriptor.h" "EclipseRHI/VulkanCore/source/VulkanDescriptor.cpp"
"EclipseRHI/VulkanCore/include/VulkanVertex.h" "EclipseRHI/VulkanCore/source/VulkanVertex.cpp"
"EclipseRHI/VulkanCore/include/VulkanShaderModule.h" "EclipseRHI/VulkanCore/source/VulkanShaderModule.cpp"
"EclipseRHI/VulkanCore/include/VulkanPipeline.h" "EclipseRHI/VulkanCore/source/VulkanPipeline.cpp"
"EclipseRHI/VulkanCore/include/VulkanSurface.h" "EclipseRHI/VulkanCore/source/VulkanSurface.cpp"
"EclipseRHI/VulkanCore/include/VulkanCommandPool.h" "EclipseRHI/VulkanCore/source/VulkanCommandPool.cpp"
"EclipseRHI/VulkanCore/include/VulkanCommandBuffer.h" "EclipseRHI/VulkanCore/source/VulkanCommandBuffer.cpp" 
"EclipseRHI/VulkanCore/include/VulkanRenderPass.h" "EclipseRHI/VulkanCore/source/VulkanRenderPass.cpp" 
"EclipseRHI/VulkanCore/include/VulkanImage.h" "EclipseRHI/VulkanCore/source/VulkanImage.cpp"    
"EclipseRHI/VulkanCore/include/VulkanRenderInterface.h" 
"EclipseRHI/VulkanCore/include/VulkanUtilities.h" "EclipseRHI/VulkanCore/source/VulkanUtilities.cpp" 
"EclipseRHI/VulkanCore/include/VulkanSync.h" "EclipseRHI/VulkanCore/source/VulkanSync.cpp"

"EclipseRHI/include/IBuffer.h"  "EclipseRHI/include/IDevice.h" "EclipseRHI/include/ICommandBuffer.h" "EclipseRHI/include/ICommandPool.h" 
"EclipseRHI/include/IDescriptor.h" "EclipseRHI/include/IFormat.h" "EclipseRHI/include/IImage.h" "EclipseRHI/include/IInstance.h" 
"EclipseRHI/include/IPipeline.h" "EclipseRHI/source/IRenderInterface.cpp" "EclipseRHI/include/IRenderPass.h"  "EclipseRHI/include/IRenderInterface.h"
"EclipseRHI/include/IShaderModule.h" "EclipseRHI/include/ISurface.h" "EclipseRHI/include/ISwapChain.h"  
"EclipseRHI/include/ISync.h" "EclipseRHI/include/Vertex.h" 
)

target_compile_definitions(EclipseRHI PUBLIC VK_NO_PROTOTYPES VULKAN_HPP_NO_STRUCT_CONSTRUCTORS GLFW_INCLUDE_VULKAN)
target_link_libraries(EclipseRHI PUBLIC Vulkan::Headers volk glfw)

add_executable(VulkanForwardRenderer 

"ForwardRenderer/Code/source/main.cpp" "ForwardRenderer/Code/source/application.cpp" "ForwardRenderer/Code/include/application.h" 
"ForwardRenderer/Code/include/Model.h" "ForwardRenderer/Code/source/Model.cpp"  
"ForwardRenderer/Code/include/camera.h" "ForwardRenderer/Code/source/camera.cpp" 
"ForwardRenderer/Code/include/InputManager.h" "ForwardRenderer/Code/source/InputManager.cpp" 
"ForwardRenderer/Code/source/texture.cpp" "ForwardRenderer/Code/include/texture.h" 
)

include(CPM.cmake)

# Dependencies
find_package(Vulkan REQUIRED)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "")

CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip" NAME glfw)
CPMAddPackage(URL "https://github.com/zeux/volk/archive/refs/tags/1.4.304.zip" NAME volk)
CPMAddPackage(URL "https://github.com/nothings/stb/archive/refs/heads/master.zip" NAME stb_image)
CPMAddPackage(URL "https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v2.0-rc1.zip" NAME tinyobjloader)

target_include_directories(VulkanForwardRenderer PRIVATE "${CMAKE_SOURCE_DIR}/out/build/x64-debug/_deps/stb_image-src")

target_link_libraries     (VulkanForwardRenderer PUBLIC Vulkan::Headers volk glfw tinyobjloader EclipseRHI)
