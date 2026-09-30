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

	EnigmaRHI::IRenderInterface* rhi;

	EnigmaRHI::IInstance* instance;
	EnigmaRHI::IDevice* device;
	EnigmaRHI::ISwapChain* swapChain;
	EnigmaRHI::ICommandPool* commandPool;
	EnigmaRHI::IImage* roomTexture;
	EnigmaRHI::IDescriptor* descriptor;
	EnigmaRHI::IPipeline* pipeline;
	EnigmaRHI::ISurface* surface;
	EnigmaRHI::IRenderPass* renderPass;
	EnigmaRHI::ISync* syncronizer;
	EnigmaRHI::IShaderModule* vertShader;
	EnigmaRHI::IShaderModule* fragShader;

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