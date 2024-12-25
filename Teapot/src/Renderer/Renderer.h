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
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		Renderer(const std::vector<Vertex>& vertices,const  std::vector<GLuint>& indices, const std::vector<Texture>& textures);

		void Clear() const;
		void DrawShape(uint8_t renderType) const;
		void DrawTriangle(Shader& shader);

	private:
		VertexArray va;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
	};
}