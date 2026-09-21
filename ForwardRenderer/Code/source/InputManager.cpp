#include "../include/InputManager.h"

glm::vec2 InputManager::mousePos = { 0.f, 0.f };

bool InputManager::GetKeyUp(GLFWwindow* window, int key)
{
	return glfwGetKey(window, key) == GLFW_RELEASE;
}

bool InputManager::GetKeyDown(GLFWwindow* window, int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool InputManager::GetMouseButton(GLFWwindow* window, int key)
{
	return glfwGetMouseButton(window, key) == GLFW_PRESS;
}

void InputManager::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	mousePos.x = static_cast<float>(xPos);
	mousePos.y = static_cast<float>(yPos);
}

glm::vec2 InputManager::GetMousePos()
{
	return mousePos;
}
