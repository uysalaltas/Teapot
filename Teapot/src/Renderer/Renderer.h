#pragma once
#include <glad/glad.h>
#include <iostream>
#include <memory>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader/Shader.h"
#include "Texture.h"

namespace Teapot
{
	class Renderer
	{
	public:
		Renderer(
			std::vector<Teapot::Vertex>&& vertices,
			std::vector<GLuint>&& indices, 
			std::vector<Teapot::Texture>&& textures
		);

		void Clear() const;
		void DrawShape(uint8_t renderType) const;
		void DrawTriangle(Teapot::Shader& shader) const;
		void PushTexture(const Texture& texture);

	private:
		std::vector<Teapot::Vertex> m_vertices;
		std::vector<GLuint> m_indices;
		std::vector<Teapot::Texture> m_textures;

		VertexArray m_va;
		std::unique_ptr<Teapot::VertexBuffer> m_vb;
		std::unique_ptr<Teapot::IndexBuffer> m_ib;
	};
}