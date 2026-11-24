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

struct RenderObject
{
	std::array<EnigmaRHI::IBindingSet*, EnigmaRHI::MAX_FRAMES_IN_FLIGHT> frameSets = {};
	Model* model = nullptr;
	EnigmaRHI::IImage* texture = nullptr;

	glm::mat4 T;
	glm::mat4 R;
	glm::mat4 S;
};

class Application
{
public:

	void Run();

private:

	EnigmaRHI::IRenderInterface* rhi = nullptr;

	EnigmaRHI::IInstance* instance = nullptr;
	EnigmaRHI::IDevice* device = nullptr;
	EnigmaRHI::ISwapChain* swapChain = nullptr;
	EnigmaRHI::ICommandPool* commandPool = nullptr;
	EnigmaRHI::IBindingLayout* bindingLayout = nullptr;
	EnigmaRHI::IDescriptor* descriptor = nullptr;
	EnigmaRHI::IPipeline* pipeline = nullptr;
	EnigmaRHI::ISurface* surface = nullptr;
	EnigmaRHI::ISync* syncronizer = nullptr;
	EnigmaRHI::IShaderModule* vertShader = nullptr;
	EnigmaRHI::IShaderModule* fragShader = nullptr;

	EnigmaRHI::IImage* depthImage = nullptr;

	Camera* cam = nullptr;

	void InitAPI();
	void MainLoop();
	void CleanUp();
	void CreateDepthImage();

	GLFWwindow* mainWindow = nullptr;

	void InitWindow();

	const uint32_t WIDTH = 1920;
	const uint32_t HEIGHT = 1080;

	uint32_t frame = 0;

	const char* TEXTURE_PATH = "Assets/Textures/viking_room.png";
	const char* MODEL_PATH = "Assets/Meshes/viking_room.obj";
	const char* TEXTURE_PATH2 = "Assets/Textures/container.png";
	const char* MODEL_PATH2 = "Assets/Meshes/cube.obj";

	void DrawFrame();
	void ResizeSwapChain();

	std::array<EnigmaRHI::IFence*, EnigmaRHI::MAX_FRAMES_IN_FLIGHT> frameFences;

	std::vector<RenderObject*> renderObjets;
	RenderObject* CreateObject(const char* modelPath, const char* texturePath, glm::vec3 translate, float rotate, glm::vec3 scale);
};