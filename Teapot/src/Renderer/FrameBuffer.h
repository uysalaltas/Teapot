#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Teapot
{
	class FrameBuffer
	{
	public:
		FrameBuffer(int width, int height);
		~FrameBuffer();
		unsigned int GetFrameTexture() const;
		void RescaleFrameBuffer(int width, int height) const;
		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_fbo;
		unsigned int m_texture;
		unsigned int m_rbo;
	};
}