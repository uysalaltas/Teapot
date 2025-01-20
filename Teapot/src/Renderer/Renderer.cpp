#include "Renderer.h"
#include <glm/gtx/string_cast.hpp>

namespace Teapot
{
	Renderer::Renderer(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
		: vertices(vertices)
		, indices(indices)
		, textures(textures)
	{
		m_va.Bind();

		m_vb = std::make_unique<VertexBuffer>(vertices);
		m_ib = std::make_unique<IndexBuffer>(indices);

		m_va.AddBuffer(*m_vb, 0, 3, sizeof(Vertex), nullptr);
		m_va.AddBuffer(*m_vb, 1, 3, sizeof(Vertex), (void*)offsetof(Vertex, color));
		m_va.AddBuffer(*m_vb, 2, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		m_va.AddBuffer(*m_vb, 3, 2, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
		//va.AddBuffer(*vb, 4, 3, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		//va.AddBuffer(*vb, 5, 3, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
		//va.AddBuffer(*vb, 6, 4, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
		//va.AddBuffer(*vb, 7, 4, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

		m_va.Unbind();
		m_vb->Unbind();
		m_ib->Unbind();
	}

	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::DrawShape(uint8_t renderType) const
	{
		m_va.Bind();
		//shader.Bind();

		if (renderType == 0)
		{
			glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
		}
		else if (renderType == 1)
		{
			glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void Renderer::DrawTriangle(Shader& shader) const
	{
		//shader.Bind();
		m_va.Bind();

		for (const auto& texture : textures)
		{
			texture.Bind();
			texture.TexUnit(shader);
		}

		//vb->BufferDataModification(vertices);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
	}

}