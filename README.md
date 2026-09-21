# 🖼️ EclipseRHI

## 📍Description

**EclipseRHI** is a rendering hardware abstraction layer designed to simplify the usage of low-level graphics APIs such as Vulkan.

## 🎯 Goals 

- **Abstraction of low-level graphics APIs**  
  Applications interact with EclipseRHI instead of Vulkan or another rendering API.

- **Reduce Vulkan boilerplate**  
  EclipseRHI simplifies most object creations into compact calls.

- **Extensibility**  
  Additional backends (e.g., DirectX12, Metal, WebGPU) can be added without changing client code.

- **Clean Architecture & Maintainability**  
  Rendering code becomes easier to read, debug, and scale.

- **Strong logical separation**  
  The rendering backend is hidden behind an interface layer.

## ⚙️ Supported API

- **Vulkan**

## 🧩 Entities

| EclipseRHI                       | Vulkan                        |
|----------------------------------|-------------------------------|
| `IBuffer`                        | `VulkanBuffer`                |
| `ICommandBuffer`                 | `VulkanCommandBuffer`         |
| `ICommandPool`                   | `VulkanCommandPool`           |
| `IDescriptor`                    | `VulkanDescriptor`            |
| `IDevice`                        | `VulkanDevice`                |
| `IImage`                         | `VulkanImage`                 |
| `IInstance`                      | `VulkanInstance`              |
| `IPipeline`                      | `VulkanPipeline`              |
| `IRenderInterface`               | `VulkanRenderInterface`       |
| `IRenderPass`                    | `VulkanRenderPass`            |
| `IShaderModule`                  | `VulkanShaderModule`          |
| `ISurface`                       | `VulkanSurface`               |
| `ISwapChain`                     | `VulkanSwapChain`             |
| `ISync`                          | `VulkanSync`                  |
| `Vertex`                         | `VulkanVertex`                |
| `IFormat`                        | `VulkanUtilities`             |

## 🚀 Getting Started

**Create Render Interface :**

````cpp
	EclipseRHI::IRenderInterface* rhi = EclipseRHI::IRenderInterface::CreateRenderInterface(EclipseRHI::ERenderAPI::Vulkan);
````

**Create an object :**
````cpp
    IObject* object = rhi->InstantiateObject();
    object->Create(...);
````

**Destroy an object :**
````cpp
    object->Destroy(...);
    rhi->DeleteObject();
````

## ⚙️ Requirements

- C++ 20 compiler
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [volk](https://github.com/zeux/volk)

## Contributor

* [Eliott Blesz](https://gitlabstudents.isartintra.com/e.blesz)