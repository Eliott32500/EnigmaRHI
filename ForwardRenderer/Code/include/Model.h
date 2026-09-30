#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#include "Vertex.h"
#include "IBuffer.h"
#include "IRenderInterface.h"

struct ModelData
{
	glm::mat4 trs;
};

class Model
{
public:

	Model() = default;

	void LoadModel(const char* filePath);
	void Create(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool);
	void UpdateDataBuffer(uint32_t currentImage, glm::mat4 translate, glm::mat4 rotate, glm::mat4 scale);
	void Destroy(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device);
	void Render(EnigmaRHI::ICommandBuffer* cmd);

	std::vector<EnigmaRHI::IBuffer*> GetModelBuffers() {return modelBuffers; }
	EnigmaRHI::IBuffer* GetVertexBuffer() const { return vertexBuffer; };
	EnigmaRHI::IBuffer* GetIndexBuffer() const { return indexBuffer; };

private:

	void CreateVertexBuffer(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool, std::vector<Vertex> vertices);
	void CreateIndexBuffer(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool, std::vector<uint32_t> indices);

	EnigmaRHI::IBuffer* vertexBuffer = nullptr;
	EnigmaRHI::IBuffer* indexBuffer = nullptr;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::vector<EnigmaRHI::IBuffer*> modelBuffers;
};