#pragma once

#include "ShadowMapping.h"
#include "Models/Model/Model.h"
#include <glm/glm.hpp>

namespace Teapot 
{
	class Shadow
	{
	public:
		enum class RenderType
		{
			Perspective,
			Ortho
		};

	public:
		Shadow(const Shader& shaderShadow, const Shader& shaderDepth, const glm::vec3& lightPos, RenderType renderType);
		void RenderShadow();

		inline void BindShadow() const { shadowMapping->BindTexture(); };
		inline void UnbindShadow() const { shadowMapping->UnbindFrameBuffer(); };

		inline glm::mat4 GetLightSpaceMatrix() const { return m_lightSpaceMatrix; };
		std::unique_ptr<ShadowMapping> shadowMapping;

	private:
		Shader m_shaderShadow;
		Shader m_shaderDepth;

		const glm::vec3& m_lightPos;
		RenderType m_renderType;

		glm::mat4 m_lightProjection;
		glm::mat4 m_lightView;
		glm::mat4 m_lightSpaceMatrix;
	};
}