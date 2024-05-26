#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

#include "Renderer/Shader.h"

namespace Teapot
{
	class ShadowMapping
	{
	public:
		ShadowMapping();
		~ShadowMapping();

		inline unsigned int GetShadowMapTexture() { return depthMapTexture; }
		void RenderShadow(Shader& shader, glm::mat4& lightSpaceMatrix);
		void BindTexture() const;
		void UnbindFrameBuffer() const;
		void DebugShadow(Shader& shader);

	private:
		void RenderQuad();

	private:
		GLuint depthMapFBO;
		GLuint depthMapTexture;
		GLuint quadVAO = 0;
		GLuint quadVBO = 0;
		int shadowWidth = 4096;
		int shadowHeigth = 4096;
	};
}