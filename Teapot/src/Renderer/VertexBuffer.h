#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

constexpr size_t MAX_BONE_INFLUENCE = 4;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texUV;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

namespace Teapot
{
	class VertexBuffer
	{
	public:
		explicit VertexBuffer(const std::vector<Vertex>& vertices);
		~VertexBuffer();

		void BufferDataModification(std::vector<Vertex>& vertices) const;
		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_RendererID;

	};
}