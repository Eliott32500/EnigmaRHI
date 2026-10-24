#include "../include/application.h"
#include <thread>
#include "VulkanImageTypes.h"

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
	renderPass = rhi->InstantiateRenderPass();
	descriptor = rhi->InstantiateDescriptor();
	pipeline = rhi->InstantiatePipeline();
	syncronizer = rhi->InstantiateSync();
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
	swapChain->Create(device, surface, commandPool, EnigmaRHI::Extent2D{WIDTH, HEIGHT});
	renderPass->Create(device, swapChain, device->FindDepthFormat());

	descriptor->AddBufferBinding( 0, EnigmaRHI::EShaderType::VERTEX );
	descriptor->AddBufferBinding( 1, EnigmaRHI::EShaderType::VERTEX );
	descriptor->AddImageBinding(2);
	descriptor->Create(device);

	vertShader->Create(device, "Assets/Shaders/vert.spv");
	fragShader->Create(device, "Assets/Shaders/frag.spv");

	pipeline->Create(vertShader, fragShader, device, swapChain, renderPass, descriptor);

	TextureLoadOptions opts;
	opts.colorSpace = ETextureColorSpace::SRGB;   // Linear pour normal/roughness/metallic...

	TextureData tex = LoadTexture(TEXTURE_PATH, opts);
	if (!tex.IsValid())
		return;

	EnigmaRHI::ImageSampler sampler
	{
		.minFilter = EnigmaRHI::EFilter::LINEAR,
		.magFilter = EnigmaRHI::EFilter::LINEAR,
		.mipMapMode = EnigmaRHI::EMipmapMode::LINEAR,
		.wrapU = EnigmaRHI::EWrappingMode::REPEAT,
		.wrapV = EnigmaRHI::EWrappingMode::REPEAT,
		.wrapW = EnigmaRHI::EWrappingMode::REPEAT,
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
		.usage = EnigmaRHI::EImageUsage::SAMPLED | EnigmaRHI::EImageUsage::TRANSFER_DST,
		.sampler = sampler,
		.extent = {tex.width, tex.height, 1},
		.data = tex.pixels,
	};

	roomTexture = device->CreateImageTexture(commandPool, imageDesc);

	model = new Model();
	model->LoadModel(MODEL_PATH);
	model->Create(device, commandPool);

	cam = new Camera(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height);
	cam->CreateCameraDataBuffer(device);

	descriptor->AddImageInfo(roomTexture);

	std::vector<EnigmaRHI::IDescriptor::FrameDescriptorInfo> perFrameDescriptors;
	perFrameDescriptors.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		perFrameDescriptors[i].buffers.push_back(cam->GetCameraDataBuffers()[i]->bufferInfo);
		perFrameDescriptors[i].buffers.push_back(model->GetModelBuffers()[i]->bufferInfo);
	}
	 
	descriptor->CreateDescriptorSets(device, perFrameDescriptors);

	syncronizer->Create(device);
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
	swapChain->Destroy(device);
	pipeline->Destroy(device);
	roomTexture->Destroy(device);
	renderPass->Destroy(device);
	descriptor->Destroy(device);
	model->Destroy(device);
	cam->Destroy(device);
	syncronizer->Destroy(device);
	commandPool->Destroy(device);
	device->Destroy();
	surface->Destroy(instance);
	instance->Destroy();

	rhi->DeleteSwapChain(swapChain);
	rhi->DeleteShaderModule(fragShader);
	rhi->DeleteShaderModule(vertShader);
	rhi->DeletePipeline(pipeline);
	rhi->DeleteRenderPass(renderPass);
	device->DeleteImage(roomTexture);
	rhi->DeleteDescriptor(descriptor);
	delete model;
	delete cam;
	rhi->DeleteSync(syncronizer);
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
	uint32_t imageIndex = 0;
	
	bool aquireSucceded = syncronizer->AquireNextImage(device, swapChain, commandPool, surface, renderPass, mainWindow, &imageIndex);

	if (!aquireSucceded)
		return;
	
	cam->UpdateCameraDataBuffer(syncronizer->GetCurrentFrame(), swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, mainWindow);
	
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 1.f));
	
	model->UpdateDataBuffer(syncronizer->GetCurrentFrame(), translate, rotate, scale);
	
	EnigmaRHI::ICommandBuffer* cmd = commandPool->GetCommandBuffer(syncronizer->GetCurrentFrame());
	
	cmd->BeginDraw(renderPass, swapChain, pipeline, imageIndex);
	cmd->BindDescriptorSet(pipeline, descriptor, syncronizer);

		model->Render(cmd);
	
	cmd->EndDraw();
	
	syncronizer->PresentFrame(device, swapChain, commandPool, surface, renderPass, mainWindow, &imageIndex);
	syncronizer->MoveToNextFrame();
}