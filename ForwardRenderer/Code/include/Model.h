#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#include "../../../EclipseRHI/include/Vertex.h"
#include "../../../EclipseRHI/include/IBuffer.h"
#include "../../../EclipseRHI/include/IRenderInterface.h"

struct ModelData
{
	glm::mat4 trs;
};

class Model
{
public:

	Model() = default;

	void LoadModel(const char* filePath);
	void Create(EclipseRHI::IRenderInterface* rhi, EclipseRHI::IDevice* device, EclipseRHI::ICommandPool* commandPool);
	void UpdateDataBuffer(uint32_t currentImage, glm::mat4 translate, glm::mat4 rotate, glm::mat4 scale);
	void Destroy(EclipseRHI::IRenderInterface* rhi, EclipseRHI::IDevice* device);
	void Render(EclipseRHI::ICommandBuffer* cmd);

	std::vector<EclipseRHI::IBuffer*> GetModelBuffers() {return modelBuffers; }
	EclipseRHI::IBuffer* GetVertexBuffer() const { return vertexBuffer; };
	EclipseRHI::IBuffer* GetIndexBuffer() const { return indexBuffer; };

private:

	void CreateVertexBuffer(EclipseRHI::IRenderInterface* rhi, EclipseRHI::IDevice* device, EclipseRHI::ICommandPool* commandPool, std::vector<Vertex> vertices);
	void CreateIndexBuffer(EclipseRHI::IRenderInterface* rhi, EclipseRHI::IDevice* device, EclipseRHI::ICommandPool* commandPool, std::vector<uint32_t> indices);

	EclipseRHI::IBuffer* vertexBuffer = nullptr;
	EclipseRHI::IBuffer* indexBuffer = nullptr;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::vector<EclipseRHI::IBuffer*> modelBuffers;
};