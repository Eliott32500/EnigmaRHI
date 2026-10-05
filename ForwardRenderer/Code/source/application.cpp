#include "../include/application.h"

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
	commandPool->Create(device, surface);
	swapChain->Create(device, surface, commandPool, mainWindow);
	renderPass->Create(device, swapChain, device->FindDepthFormat());

	descriptor->AddBufferBinding( 0, EnigmaRHI::ShaderStage::Vertex );
	descriptor->AddBufferBinding( 1, EnigmaRHI::ShaderStage::Vertex );
	descriptor->AddImageBinding(2);
	descriptor->Create(device);

	vertShader->Create(device, "Assets/Shaders/vert.spv");
	fragShader->Create(device, "Assets/Shaders/frag.spv");

	pipeline->Create(vertShader, fragShader, device, swapChain, renderPass, descriptor);

	roomTexture = rhi->InstantiateImage();
	
	Texture texture{};
	texture.LoadTexture(TEXTURE_PATH);
	roomTexture->CreateTextureImage(texture.GetData(), device, commandPool, texture.GetWidth(), texture.GetHeight(), texture.GetImageFormat());
	texture.FreeTextureData();

	model = new Model();
	model->LoadModel(MODEL_PATH);
	model->Create(rhi, device, commandPool);

	cam = new Camera(swapChain->GetWidth(), swapChain->GetHeight());
	cam->CreateCameraDataBuffer(rhi, device);

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
	device->DeviceWaitIdle();
}

void Application::CleanUp()
{
	swapChain->Destroy(device);
	pipeline->Destroy(device);
	roomTexture->Destroy(device);
	renderPass->Destroy(device);
	descriptor->Destroy(device);
	model->Destroy(rhi, device);
	cam->Destroy(rhi, device);
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
	rhi->DeleteImage(roomTexture);
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
	
	syncronizer->AquireNextImage(device, swapChain, commandPool, surface, renderPass, mainWindow, &imageIndex);
	
	cam->UpdateCameraDataBuffer(syncronizer->GetCurrentFrame(), swapChain->GetWidth(), swapChain->GetHeight(), mainWindow);
	
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