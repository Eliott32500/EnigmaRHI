#pragma once

#include "../include/Model.h"
#include "../include/texture.h"
#include "../include/camera.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

class Application
{
public:

	void Run();

private:

	EclipseRHI::IRenderInterface* rhi;

	EclipseRHI::IInstance* instance;
	EclipseRHI::IDevice* device;
	EclipseRHI::ISwapChain* swapChain;
	EclipseRHI::ICommandPool* commandPool;
	EclipseRHI::IImage* roomTexture;
	EclipseRHI::IDescriptor* descriptor;
	EclipseRHI::IPipeline* pipeline;
	EclipseRHI::ISurface* surface;
	EclipseRHI::IRenderPass* renderPass;
	EclipseRHI::ISync* syncronizer;
	EclipseRHI::IShaderModule* vertShader;
	EclipseRHI::IShaderModule* fragShader;

	Camera* cam;
	Model* model;

	void InitAPI();
	void MainLoop();
	void CleanUp();


	GLFWwindow* mainWindow = nullptr;

	void InitWindow();

	const uint32_t WIDTH = 1920;
	const uint32_t HEIGHT = 1080;

	const char* TEXTURE_PATH = "Assets/Textures/viking_room.png";
	const char* MODEL_PATH = "Assets/Meshes/viking_room.obj";

	void DrawFrame();
};