cmake_minimum_required(VERSION 3.20)

project(EnigmaRHI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# ============================================================
# Dependencies
# ============================================================

find_package(Vulkan REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.5.1
)

FetchContent_Declare(
    volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG 1.4.350
)

FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG v1.0.6
)

FetchContent_MakeAvailable(
    glfw
    volk
    stb_image
    tinyobjloader
)

# ============================================================
# EnigmaRHI
# ============================================================

file(GLOB_RECURSE EclipseRHI_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/EclipseRHI/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/EclipseRHI/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/EclipseRHI/*.hpp"
)

add_library(EnigmaRHI STATIC ${EclipseRHI_SOURCES})

target_include_directories(EnigmaRHI
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/EclipseRHI/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/EclipseRHI/VulkanCore/include"
)

target_compile_definitions(EnigmaRHI
    PUBLIC
        VK_NO_PROTOTYPES
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
        GLFW_INCLUDE_VULKAN
)

target_link_libraries(EnigmaRHI
    PUBLIC
        Vulkan::Headers
        volk
        glfw
)

# ============================================================
# VulkanForwardRenderer
# ============================================================

file(GLOB_RECURSE FORWARD_RENDERER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/ForwardRenderer/Code/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ForwardRenderer/Code/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ForwardRenderer/Code/*.hpp"
)

add_executable(VulkanForwardRenderer
    ${FORWARD_RENDERER_SOURCES}
 "EclipseRHI/VulkanCore/source/VulkanRenderInterface.cpp")

target_include_directories(VulkanForwardRenderer
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/ForwardRenderer/Code/include"
        "${stb_image_SOURCE_DIR}"
)

target_link_libraries(VulkanForwardRenderer
    PRIVATE
        EnigmaRHI
        Vulkan::Headers
        volk
        glfw
        tinyobjloader
)

# ============================================================
# Assets
# ============================================================

file(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/ForwardRenderer/Assets"
    DESTINATION "${CMAKE_BINARY_DIR}"
)