#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

namespace Teapot
{
	class IndexBuffer
	{
	public:
		explicit IndexBuffer(const std::vector<GLuint>& indices);
		~IndexBuffer();
		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_RendererID;
	};
}
