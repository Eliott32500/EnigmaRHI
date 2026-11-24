#include "../include/application.h"
#include <thread>
#include "VulkanImageTypes.h"
#include "IGraphicsPipeline.h"
#include "RHILog.h"

void Application::Run()
{
	InitWindow();
	InitAPI();
	MainLoop();
	CleanUp();
}

void Application::InitWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "EnigmaRHI", nullptr, nullptr);
	glfwSetWindowUserPointer(mainWindow, this);
	glfwSetCursorPosCallback(mainWindow, InputManager::MouseCallback);
}

void Application::InitAPI()
{
	rhi = EnigmaRHI::IRenderInterface::CreateRenderInterface(EnigmaRHI::ERenderAPI::Vulkan);

	instance = rhi->InstantiateInstance();
	surface = rhi->InstantiateSurface();
	device = rhi->InstantiateDevice();
	commandPool = rhi->InstantiateCommandPool();
	swapChain = rhi->InstantiateSwapChain();
	pipeline = rhi->InstantiateGraphicsPipeline();
	vertShader = rhi->InstantiateShaderModule();
	fragShader = rhi->InstantiateShaderModule();

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);;

	EnigmaRHI::InstanceCreateInfo instanceInfo
	{
		.extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount),
		.applicationName = "EnigmaRHI",
		.engineName = "EnigmaEngine",
		.applicationVersion = 1,
		.engineVersion = 1,
	};

	EnigmaRHI::WindowInfo windowInfo
	{
		.hInstance = GetModuleHandle(nullptr),
		.hwnd = glfwGetWin32Window(mainWindow)
	};

	instance->Create(instanceInfo);
	surface->Create(instance, windowInfo);
	device->Create(instance, surface);
	commandPool->Create(device);
	swapChain->Create(device, surface, EnigmaRHI::Extent2D{WIDTH, HEIGHT});

	EnigmaRHI::BindingLayoutDesc layoutDesc;
	layoutDesc.items = 
	{
		{ 0,  EnigmaRHI::EBindingType::UNIFORM_BUFFER,           EnigmaRHI::EShaderType::VERTEX },
		{ 1,  EnigmaRHI::EBindingType::UNIFORM_BUFFER,           EnigmaRHI::EShaderType::VERTEX },
		{ 2,  EnigmaRHI::EBindingType::COMBINED_IMAGE_SAMPLER,   EnigmaRHI::EShaderType::FRAGMENT },
	};
	bindingLayout = device->CreateBindingLayout(layoutDesc);

	vertShader->Create(device, "Assets/Shaders/vert.spv");
	fragShader->Create(device, "Assets/Shaders/frag.spv");

	EnigmaRHI::VertexBindingDesc vBinding
	{
		.binding = 0,
		.stride = sizeof(Vertex),
		.inputRate = EnigmaRHI::EVertexInputRate::PER_VERTEX
	};

	std::vector<EnigmaRHI::VertexAttributeDesc> vAttributes;

	vAttributes.push_back(EnigmaRHI::VertexAttributeDesc{ .location = 0, .binding = 0, .format = EnigmaRHI::EVertexFormat::FLOAT3, .offset = offsetof(Vertex, pos)});
	vAttributes.push_back(EnigmaRHI::VertexAttributeDesc{ .location = 1, .binding = 0, .format = EnigmaRHI::EVertexFormat::FLOAT3, .offset = offsetof(Vertex, color)});
	vAttributes.push_back(EnigmaRHI::VertexAttributeDesc{ .location = 2, .binding = 0, .format = EnigmaRHI::EVertexFormat::FLOAT2, .offset = offsetof(Vertex, texCoord)});

	EnigmaRHI::GraphicsPipelineDesc gp
	{
		.vertexBindings = {vBinding},
		.vertexAttributes = vAttributes,
		.blendStates = {EnigmaRHI::ColorBlendAttachmentState{}},
	};

	EnigmaRHI::Shader shader
	{
		.vertShader = vertShader,
		.fragShader = fragShader,
	};

	pipeline->Create(gp, shader, device, bindingLayout, swapChain->GetSwapChainImageFormat());

	cam = new Camera(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height);
	cam->CreateCameraDataBuffer(device);

	renderObjets.push_back(CreateObject(MODEL_PATH, TEXTURE_PATH, glm::vec3(0.f, 0.f, 0.f), 90.f, glm::vec3(1.f, 1.f, 1.f)));
	renderObjets.push_back(CreateObject(MODEL_PATH2, TEXTURE_PATH2, glm::vec3(5.f, 0.f, 1.f), 0.f, glm::vec3(0.5f, 0.5f, 0.5f)));

	for (int i = 0; i < frameFences.size(); i++)
	{
		frameFences[i] = device->CreateFence(true);
	}

	CreateDepthImage();
}

void Application::CreateDepthImage()
{
	if (depthImage)
	{
		device->DeleteImage(depthImage);
		depthImage = nullptr;
	}

	EnigmaRHI::ImageDesc depthDesc
	{
		.type = EnigmaRHI::EImageType::TYPE_2D,
		.viewType = EnigmaRHI::EImageViewType::VIEW_2D,
		.format = device->FindDepthFormat(),
		.usage = EnigmaRHI::EImageUsage::DEPTH_STENCIL_ATTACHMENT,
		.extent = swapChain->GetSwapChainExtent(),
		.mipLevels = 1,
	};
	depthImage = device->CreateImage(depthDesc);
}

void Application::MainLoop()
{
	while (!glfwWindowShouldClose(mainWindow)) 
	{
		glfwPollEvents();
		DrawFrame();
	}

	device->WaitIdle();
}

void Application::CleanUp()
{
	for (auto* fence : frameFences)
		device->DeleteFence(fence);

	for (RenderObject* o : renderObjets)
	{
		for (auto b : o->frameSets)
		{
			device->DeleteBindingSet(b);
		}

		device->DeleteImage(o->texture);
		o->model->Destroy(device);
		delete o->model;
	}

	device->DeleteBindingLayout(bindingLayout);

	device->DeleteImage(depthImage);

	cam->Destroy(device);
	pipeline->Destroy(device);
	swapChain->Destroy(device);
	commandPool->Destroy(device);
	fragShader->Destroy(device);
	vertShader->Destroy(device);
	device->Destroy();
	surface->Destroy(instance);
	instance->Destroy();

	rhi->DeleteSwapChain(swapChain);
	rhi->DeleteShaderModule(fragShader);
	rhi->DeleteShaderModule(vertShader);
	rhi->DeletePipeline(pipeline);
	//delete model;
	delete cam;
	rhi->DeleteCommandPool(commandPool);
	rhi->DeleteDevice(device);
	rhi->DeleteSurface(surface);
	rhi->DeleteInstance(instance);
	delete rhi;

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void Application::DrawFrame()
{
	EnigmaRHI::IQueue& queue = device->GetQueue(ECommandQueueType::GRAPHICS);

	if(!frameFences[frame]->Wait(device))
		LOG_FATAL(ELogCategory::VULKAN, "Failed to wait for fence!");

	if (swapChain->Acquire(device, frame) == EnigmaRHI::EAcquireResult::OUT_OF_DATE)
	{
		ResizeSwapChain();
		return;
	}

	EnigmaRHI::Extent2D extent = swapChain->GetSwapChainExtent();

	frameFences[frame]->Reset(device);

	cam->UpdateCameraDataBuffer(device, frame, extent, mainWindow);
	
	for (int i = 0; i < renderObjets.size(); i++)
	{
		renderObjets[i]->model->UpdateDataBuffer(device, frame, renderObjets[i]->T, renderObjets[i]->R, renderObjets[i]->S);
	}
	
	EnigmaRHI::ICommandBuffer* cmd = commandPool->GetCommandBuffer(frame);

	cmd->Begin();

	EnigmaRHI::Viewport viewport
	{
		.x = 0.0f,
		.y = 0.0f,
		.width = (float)extent.width,
		.height = (float)extent.height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	EnigmaRHI::Rect2D scissor
	{
		.offset = { 0, 0 },
		.extent = extent,
	};

	EnigmaRHI::RenderingAttachment attachement
	{
		.image = swapChain->GetCurrentImage(),
		.clearColor = {0, 0, 0, 0},
	};

	EnigmaRHI::RenderingAttachment depthAttachment
	{
		.image = depthImage,
		.imageLayout = EnigmaRHI::EImageLayout::DEPTH_ATTACHMENT_OPTIMAL,
		.loadOp = EnigmaRHI::ELoadOp::CLEAR,
		.storeOp = EnigmaRHI::EStoreOp::DONT_CARE,
		.clearDepth = 1.0f,
	};
	
	EnigmaRHI::RenderingInfo info
	{
		.renderArea = {.offset = {0, 0}, .extent = extent},
		.colorAttachments = {attachement},
		.depthAttachment = &depthAttachment,
	};

	cmd->TransitionImage(swapChain->GetCurrentImage(), EnigmaRHI::EImageLayout::COLOR_ATTACHMENT_OPTIMAL);
	cmd->TransitionImage(depthImage, EnigmaRHI::EImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

	cmd->BeginRendering(info);
	cmd->BindPipeline(pipeline);
	cmd->SetViewport(viewport);
	cmd->SetScissor(scissor);

	for (int i = 0; i < renderObjets.size(); i++)
	{
		cmd->BindBindingSet(pipeline, renderObjets[i]->frameSets[frame]);
		renderObjets[i]->model->Render(cmd);
	}

	cmd->EndRendering();

	cmd->TransitionImage(swapChain->GetCurrentImage(), EnigmaRHI::EImageLayout::PRESENT_SRC);
	
	cmd->End();

	queue.Submit(cmd, swapChain, frameFences[frame]);
	
	if (swapChain->Present(device, &queue) != EnigmaRHI::EPresentResult::SUCCESS)
		ResizeSwapChain();

	frame = (frame + 1) % EnigmaRHI::MAX_FRAMES_IN_FLIGHT;
}

void Application::ResizeSwapChain()
{
	int w = 0, h = 0;
	glfwGetFramebufferSize(mainWindow, &w, &h);
    while ((w == 0 || h == 0) && !glfwWindowShouldClose(mainWindow)) 
	{
        glfwGetFramebufferSize(mainWindow, &w, &h);
        glfwWaitEvents();
    }
    if (glfwWindowShouldClose(mainWindow))
        return;

	swapChain->Resize(device, surface, EnigmaRHI::Extent2D{ (uint32_t)w, (uint32_t)h });
	CreateDepthImage();
}

RenderObject* Application::CreateObject(const char* modelPath, const char* texturePath, glm::vec3 translate, float rotate, glm::vec3 scale)
{
	RenderObject* obj = new RenderObject();

	TextureLoadOptions opts;
	opts.colorSpace = ETextureColorSpace::SRGB;   // Linear pour normal/roughness/metallic...

	TextureData tex = LoadTexture(texturePath, opts);

	if (!tex.IsValid())
		return obj;

	EnigmaRHI::ImageSampler sampler
	{
		.minFilter = EnigmaRHI::EFilter::LINEAR,
		.magFilter = EnigmaRHI::EFilter::LINEAR,
		.mipMapMode = EnigmaRHI::EMipmapMode::LINEAR,
		.wrapU = EnigmaRHI::EWrappingMode::CLAMP_TO_BORDER,
		.wrapV = EnigmaRHI::EWrappingMode::CLAMP_TO_BORDER,
		.wrapW = EnigmaRHI::EWrappingMode::CLAMP_TO_BORDER,
		.borderColor = EnigmaRHI::EBorderColor::OPAQUE_INT_BLACK,
		.maxAnisotropy = 1.0f,
		.compareEnable = false,
		.compareOp = EnigmaRHI::ECompareOp::ALWAYS,
	};

	EnigmaRHI::ImageDesc imageDesc
	{
		.type = EnigmaRHI::EImageType::TYPE_2D,
		.viewType = EnigmaRHI::EImageViewType::VIEW_2D,
		.format = tex.format,
		.usage = EnigmaRHI::EImageUsage::SAMPLED | EnigmaRHI::EImageUsage::TRANSFER_DST | EnigmaRHI::EImageUsage::TRANSFER_SRC,
		.sampler = sampler,
		.extent = {tex.width, tex.height, 1},
		.mipLevels = tex.mipLevels,
	};

	obj->texture = device->CreateImageWithData(commandPool, imageDesc, tex.pixels);

	obj->model = new Model();
	obj->model->LoadModel(modelPath);
	obj->model->Create(device, commandPool);

	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		EnigmaRHI::BindingSetDesc setDesc;

		setDesc.items =
		{
			EnigmaRHI::BindingSetItem::UniformBuffer(0, cam->GetCameraDataBuffers()[i]),
			EnigmaRHI::BindingSetItem::UniformBuffer(1, obj->model->GetModelBuffers()[i]),
			EnigmaRHI::BindingSetItem::Texture(2, obj->texture),
		};

		obj->frameSets[i] = device->CreateBindingSet(bindingLayout, setDesc);
	}

	obj->T = glm::translate(glm::mat4(1.0f), translate);
	obj->R = glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	obj->S = glm::scale(glm::mat4(1.0f), scale);

	return obj;
}
