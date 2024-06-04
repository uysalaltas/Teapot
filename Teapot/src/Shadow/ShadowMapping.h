#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

#include "Shader/Shader.h"

namespace Teapot
{
	class ShadowMapping
	{
	public:
		ShadowMapping();
		~ShadowMapping() = default;

		inline unsigned int GetShadowMapTexture() const { return depthMapTexture; }
		void RenderShadow(Shader& shader, const glm::mat4& lightSpaceMatrix) const;
		void BindTexture() const;
		void UnbindFrameBuffer() const;
		void DebugShadow(const Shader& shader);

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