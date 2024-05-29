#include "Renderer.h"
#include <glm/gtx/string_cast.hpp>

namespace Teapot
{
	Renderer::Renderer(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
		: vertices(vertices)
		, indices(indices)
		, textures(textures)
	{
		va.Bind();

		vb = std::make_unique<VertexBuffer>(vertices);
		ib = std::make_unique<IndexBuffer>(indices);

		va.AddBuffer(*vb, 0, 3, sizeof(Vertex), (void*)0);
		va.AddBuffer(*vb, 1, 3, sizeof(Vertex), (void*)offsetof(Vertex, color));
		va.AddBuffer(*vb, 2, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		//va.AddBuffer(*vb, 3, 2, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
		//va.AddBuffer(*vb, 4, 3, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		//va.AddBuffer(*vb, 5, 3, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
		//va.AddBuffer(*vb, 6, 4, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
		//va.AddBuffer(*vb, 7, 4, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

		va.Unbind();
		vb->Unbind();
		ib->Unbind();
	}

	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::DrawShape(uint8_t renderType) const
	{
		va.Bind();
		//shader.Bind();

		if (renderType == 0)
		{
			glDrawElements(GL_LINES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		}
		else if (renderType == 1)
		{
			glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		}
	}

	void Renderer::DrawTriangle(Shader& shader)
	{
		//shader.Bind();
		va.Bind();

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			std::string type = textures[i].type;
			textures[i].Bind();
			textures[i].TexUnit(shader, type.c_str(), i);
		}

		//vb->BufferDataModification(vertices);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	}

}