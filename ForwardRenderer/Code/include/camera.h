#pragma once

#include "InputManager.h"
#include "IBuffer.h"
#include "IRenderInterface.h"

struct CameraData
{
	glm::mat4 vp;
};

class Camera
{
public:

	Camera(float width, float height);
	void CreateCameraDataBuffer(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* vulkanDevice);
	void SetCamera(float fov, float aspectRatio, float zNear, float zFar, glm::vec3 _position);
	void GetInputs(GLFWwindow* window);
	void MouseCallback(GLFWwindow* window);
	void UpdateCameraDataBuffer(uint32_t currentImage, float width, float height, GLFWwindow* mainWindow);
	void Destroy(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device);

	std::vector<EnigmaRHI::IBuffer*> GetCameraDataBuffers() { return cameraDataBuffers; }

private:

	std::vector<EnigmaRHI::IBuffer*> cameraDataBuffers;
	float width,height;

	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 position;
	glm::vec3 orientation;

	float speed = 2.f;
	float mouseSensitivity = 70.f;
	float mouseWheelSensitivity = 5.f;

	float deltaTime;
	float lastFrame;

	float yaw = -65.f;
	float pitch = -25.f;
	float lastX = 0;
	float lastY = 0;

	bool useCamera = false;

	bool isMiddleMouseButtonPressed = false;
};