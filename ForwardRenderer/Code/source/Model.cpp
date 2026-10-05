#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "../include/Model.h"

void Model::Create(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool)
{
	size_t bufferSize = sizeof(ModelData);

	modelBuffers.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		modelBuffers[i] = rhi->InstantiateBuffer();
		modelBuffers[i]->Create(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		modelBuffers[i]->MapMemory(device, 0, bufferSize, 0);
		modelBuffers[i]->CreateDescriptorBufferInfo();
		modelBuffers[i]->bufferInfo.range = sizeof(ModelData);
	}

	CreateVertexBuffer(rhi, device, commandPool, vertices);
	CreateIndexBuffer(rhi, device, commandPool, indices);
}

void Model::UpdateDataBuffer(uint32_t currentImage, glm::mat4 translate, glm::mat4 rotate, glm::mat4 scale)
{
	ModelData ubo{ scale * rotate * translate };

	modelBuffers[currentImage]->CopyData(&ubo, sizeof(ubo));
}

void Model::Destroy(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device)
{
	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		modelBuffers[i]->Destroy(device);
		rhi->DeleteBuffer(modelBuffers[i]);
	}
	vertexBuffer->Destroy(device);
	rhi->DeleteBuffer(vertexBuffer);
	indexBuffer->Destroy(device);
	rhi->DeleteBuffer(indexBuffer);
}

void Model::Render(EnigmaRHI::ICommandBuffer* cmd)
{
	cmd->BindVertexBuffer(vertexBuffer);
	cmd->BindIndexBuffer(indexBuffer);
	cmd->DrawIndexed(static_cast<uint32_t>(indices.size()));
}

void Model::CreateVertexBuffer(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool, std::vector<Vertex> vertices)
{
	size_t bufferSize = sizeof(vertices[0]) * vertices.size();

	EnigmaRHI::IBuffer* stagingBuffer = rhi->InstantiateBuffer();
	vertexBuffer = rhi->InstantiateBuffer();

	stagingBuffer->Create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer->UploadData(device, 0, bufferSize, vertices.data(), 0);

	vertexBuffer->Create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	stagingBuffer->CopyBuffer(device, commandPool, vertexBuffer, bufferSize);

	stagingBuffer->Destroy(device);
	rhi->DeleteBuffer(stagingBuffer);
}

void Model::CreateIndexBuffer(EnigmaRHI::IRenderInterface* rhi, EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool, std::vector<uint32_t> indices)
{
	size_t bufferSize = sizeof(indices[0]) * indices.size();

	EnigmaRHI::IBuffer* stagingBuffer = rhi->InstantiateBuffer();
	indexBuffer = rhi->InstantiateBuffer();

	stagingBuffer->Create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer->UploadData(device, 0, bufferSize, indices.data(), 0);

	indexBuffer->Create(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	stagingBuffer->CopyBuffer(device, commandPool, indexBuffer, bufferSize);

	stagingBuffer->Destroy(device);
	rhi->DeleteBuffer(stagingBuffer);
}

void Model::LoadModel(const char* filePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err, warn;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath))
		throw std::runtime_error(err);

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertices.push_back(vertex);

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
