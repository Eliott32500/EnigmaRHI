#include "../include/camera.h"

Camera::Camera(float width, float height)
	: width(width), height(height)
{
    SetCamera(45.f, width / height, 0.1f, 100.f, glm::vec3(-1.f, 2.4f, 1.4f));
}

void Camera::CreateCameraDataBuffer(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* vulkanDevice)
{
	VkDeviceSize bufferSize = sizeof(CameraData);

	cameraDataBuffers.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		cameraDataBuffers[i] = rhi->InstantiateBuffer();
		cameraDataBuffers[i]->Create(vulkanDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		cameraDataBuffers[i]->MapMemory(vulkanDevice, 0, bufferSize, 0);
		cameraDataBuffers[i]->CreateDescriptorBufferInfo();
		cameraDataBuffers[i]->bufferInfo.range = sizeof(CameraData);
	}
}

void Camera::UpdateCameraDataBuffer(uint32_t currentImage, float width, float height, GLFWwindow* mainWindow)
{
	glm::mat4 proj = glm::perspective(45.f, (float)width / (float)height, 0.1f, 100.f);
	glm::mat4 view = glm::lookAt(position, position + orientation, glm::vec3(0.0f, 0.0f, 1.0f));

	proj[1][1] *= -1;

	CameraData ubo{ proj * view };

	cameraDataBuffers[currentImage]->CopyData(&ubo, sizeof(ubo));

    GetInputs(mainWindow);
}

void Camera::SetCamera(float fov, float aspectRatio, float zNear, float zFar, glm::vec3 _position)
{
    projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    position = _position;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.z = sin(glm::radians(pitch));
    orientation = glm::normalize(front);

    deltaTime = 0.0f;
    lastFrame = 0.0f;
}

void Camera::GetInputs(GLFWwindow* window)
{
    float currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    glm::vec3 right = glm::normalize(glm::cross(-glm::vec3(0.f, 0.f, 1.f), orientation));
    glm::vec3 cameraUp = glm::normalize(glm::cross(right, orientation));

    MouseCallback(window);

    if (useCamera)
    {
        if (InputManager::GetKeyDown(window, KEY_Z))
            position += orientation * speed * deltaTime;

        if (InputManager::GetKeyDown(window, KEY_S))
            position -= orientation * speed * deltaTime;

        if (InputManager::GetKeyDown(window, KEY_D))
            position += right * speed * deltaTime;

        if (InputManager::GetKeyDown(window, KEY_Q))
            position -= right * speed * deltaTime;

        if (InputManager::GetKeyDown(window, KEY_E))
            position += cameraUp * speed * deltaTime;

        if (InputManager::GetKeyDown(window, KEY_A))
            position -= cameraUp * speed * deltaTime;
    }
}


void Camera::MouseCallback(GLFWwindow* window)
{
    glm::vec2 mousePos = InputManager::GetMousePos();
    float xpos = mousePos.x;
    float ypos = mousePos.y;

    float xoffset = lastX - xpos;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (InputManager::GetMouseButton(window, MOUSE_BUTTON_RIGHT))
    {
        useCamera = true;

        xoffset *= mouseSensitivity * deltaTime;
        yoffset *= mouseSensitivity * deltaTime;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.z = sin(glm::radians(pitch));
        orientation = glm::normalize(front);
    }
    else if (InputManager::GetMouseButton(window, MOUSE_BUTTON_MIDDLE))
    {
        useCamera = true;
        isMiddleMouseButtonPressed = true;

        xoffset *= -mouseWheelSensitivity * deltaTime;
        yoffset *= -mouseWheelSensitivity * deltaTime;

        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.f, 0.f, 1.f), orientation));
        glm::vec3 upMove = glm::normalize(glm::vec3(0.f, 0.f, 1.f));

        position += right * xoffset;
        position += upMove * yoffset;
    }
    else
    {
        useCamera = false;
        isMiddleMouseButtonPressed = false;
    }
}

void Camera::Destroy(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device)
{
    for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
    {
        cameraDataBuffers[i]->UnMapMemory(device);
        cameraDataBuffers[i]->Destroy(device);
        rhi->DeleteBuffer(cameraDataBuffers[i]);
	}
}

