#pragma once

#include "InputManager.h"
#include "../../../EclipseRHI/include/IBuffer.h"
#include "../../../EclipseRHI/include/IRenderInterface.h"

struct CameraData
{
	glm::mat4 vp;
};

class Camera
{
public:

	Camera(float width, float height);
	void CreateCameraDataBuffer(EclipseRHI::IRenderInterface* rhi, EclipseRHI::IDevice* vulkanDevice);
	void SetCamera(float fov, float aspectRatio, float zNear, float zFar, glm::vec3 _position);
	void GetInputs(GLFWwindow* window);
	void MouseCallback(GLFWwindow* window);
	void UpdateCameraDataBuffer(uint32_t currentImage, float width, float height, GLFWwindow* mainWindow);
	void Destroy(EclipseRHI::IRenderInterface* rhi, EclipseRHI::IDevice* device);

	std::vector<EclipseRHI::IBuffer*> GetCameraDataBuffers() { return cameraDataBuffers; }

private:

	std::vector<EclipseRHI::IBuffer*> cameraDataBuffers;
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