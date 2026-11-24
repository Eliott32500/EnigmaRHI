#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "../include/Model.h"
#include "RHIConfig.h"

void Model::Create(EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool)
{
	size_t bufferSize = sizeof(ModelData);

	EnigmaRHI::BufferDesc desc
	{
		.size = sizeof(ModelData),
		.usage = EnigmaRHI::EBufferUsage::UNIFORM,
		.access = EnigmaRHI::EMemoryAccess::CPU_TO_GPU,
		.persistentMap = true,
	};

	modelBuffers.resize(EnigmaRHI::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < EnigmaRHI::MAX_FRAMES_IN_FLIGHT; i++)
	{
		modelBuffers[i] = device->CreateBuffer(desc);
	}

	CreateVertexBuffer(device, commandPool, vertices);
	CreateIndexBuffer(device, commandPool, indices);
}

void Model::UpdateDataBuffer(EnigmaRHI::IDevice* device, uint32_t currentImage, glm::mat4 translate, glm::mat4 rotate, glm::mat4 scale)
{
	ModelData ubo{ scale * rotate * translate };

	modelBuffers[currentImage]->Write(device, &ubo, sizeof(ubo));
}

void Model::Destroy(EnigmaRHI::IDevice* device)
{
	for (auto* buffer : modelBuffers)
		device->DeleteBuffer(buffer);

	modelBuffers.clear();

	device->DeleteBuffer(vertexBuffer);
	device->DeleteBuffer(indexBuffer);
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

void Model::Render(EnigmaRHI::ICommandBuffer* cmd)
{
	cmd->BindVertexBuffer(vertexBuffer);
	cmd->BindIndexBuffer(indexBuffer);
	cmd->DrawIndexed(static_cast<uint32_t>(indices.size()));
}

void Model::CreateVertexBuffer(EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool, std::vector<Vertex> vertices)
{
	EnigmaRHI::BufferDesc desc
	{
		.size = sizeof(Vertex) * vertices.size(),
		.usage = EnigmaRHI::EBufferUsage::VERTEX,
		.access = EnigmaRHI::EMemoryAccess::GPU_ONLY,
	};

	vertexBuffer = device->CreateBufferWithData(commandPool, desc, vertices.data());
}

void Model::CreateIndexBuffer(EnigmaRHI::IDevice* device, EnigmaRHI::ICommandPool* commandPool, std::vector<uint32_t> indices)
{
	EnigmaRHI::BufferDesc desc
	{
		.size = sizeof(uint32_t) * indices.size(),
		.usage = EnigmaRHI::EBufferUsage::INDEX,
		.access = EnigmaRHI::EMemoryAccess::GPU_ONLY,
	};

	indexBuffer = device->CreateBufferWithData(commandPool, desc, indices.data());
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

	vertices.clear();
	indices.clear();

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

			if (index.texcoord_index >= 0)
			{
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0)
			{
				uint32_t newIndex =
					static_cast<uint32_t>(vertices.size());

				uniqueVertices[vertex] = newIndex;
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

