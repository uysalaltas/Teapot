#pragma once

#include "ShadowMapping/ShadowMapping.h"
#include "Renderer/Model.h"
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
		Shadow(const Shader& shaderShadow, const Shader& shaderDepth);
		~Shadow();

		void RenderShadow(const glm::vec3& lightPos, std::vector<Model*>& models, RenderType renderType);

		inline void BindShadow() const { shadowMapping->BindTexture(); };
		inline glm::mat4 GetLightSpaceMatrix() const { return lightSpaceMatrix; };
	private:
		ShadowMapping* shadowMapping;
		Shader m_shaderShadow;
		Shader m_shaderDepth;

		glm::mat4 lightProjection;
		glm::mat4 lightView;
		glm::mat4 lightSpaceMatrix;
	};
}