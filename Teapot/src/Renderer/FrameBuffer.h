#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Teapot
{
	class FrameBuffer
	{
	public:
		FrameBuffer(float width, float height);
		~FrameBuffer();
		unsigned int GetFrameTexture() const;
		void RescaleFrameBuffer(int width, int height) const;
		void Bind() const;
		void Unbind() const;
	private:
		unsigned int fbo;
		unsigned int texture;
		unsigned int rbo;
	};
}