#pragma once
#include <glad/glad.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Renderer
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Renderer(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures);
	~Renderer()
	{
		std::cout << "Renderer Deconstructor" << std::endl;
		delete vb;
		delete ib;
	}

	void Clear();
	void DrawShape(uint8_t renderType);
	void DrawTriangle(Shader& shader);

private:
	VertexArray va;
	VertexBuffer* vb;
	IndexBuffer* ib;
};