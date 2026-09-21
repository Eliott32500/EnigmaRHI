#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "input.h"

class InputManager
{
public:

	static bool GetKeyUp(GLFWwindow* window, int key);
	static bool GetKeyDown(GLFWwindow* window, int key);
	static bool GetMouseButton(GLFWwindow* window, int key);
	static void MouseCallback(GLFWwindow* window, double xPos, double yPos);

	static glm::vec2 GetMousePos();

private:
	static glm::vec2 mousePos;
};