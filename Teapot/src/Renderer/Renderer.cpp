#include "Renderer.h"
#include <glm/gtx/string_cast.hpp>

namespace Teapot
{
	Renderer::Renderer(
		std::vector<Teapot::Vertex>&& vertices, 
		std::vector<GLuint>&& indices, 
		std::vector<Teapot::Texture>&& textures
	)
		: m_vertices(std::move(vertices))
		, m_indices(std::move(indices))
		, m_textures(std::move(textures))
	{
		m_va.Bind();

		m_vb = std::make_unique<Teapot::VertexBuffer>(m_vertices);
		m_ib = std::make_unique<Teapot::IndexBuffer>(m_indices);

		m_va.AddBuffer(*m_vb, 0, 3, sizeof(Teapot::Vertex), nullptr);
		m_va.AddBuffer(*m_vb, 1, 3, sizeof(Teapot::Vertex), (void*)offsetof(Teapot::Vertex, color));
		m_va.AddBuffer(*m_vb, 2, 3, sizeof(Teapot::Vertex), (void*)offsetof(Teapot::Vertex, normal));
		m_va.AddBuffer(*m_vb, 3, 2, sizeof(Teapot::Vertex), (void*)offsetof(Teapot::Vertex, texUV));
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
			glDrawElements(GL_LINES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);
		}
		else if (renderType == 1)
		{
			glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void Renderer::DrawTriangle(Teapot::Shader& shader) const
	{
		//shader.Bind();
		m_va.Bind();

		for (const auto& texture : m_textures)
		{
			texture.Bind();
			texture.TexUnit(shader);
		}

		//vb->BufferDataModification(vertices);
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);

		for (const auto& texture : m_textures)
		{
			texture.Unbind();
		}
	}

	void Renderer::PushTexture(const Texture& texture)
	{
		m_textures.push_back(texture);
	}

}