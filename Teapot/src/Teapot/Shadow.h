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
		Shadow(Shader& shaderShadow, Shader& shaderDepth);
		~Shadow();

		void RenderShadow(glm::vec3& lightPos, std::vector<Model*>& models, RenderType renderType);

		inline void BindShadow() 
		{
			shadowMapping->BindTexture(); 
		};

		inline glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; };
	private:
		ShadowMapping* shadowMapping;
		Shader m_shaderDepth;
		Shader m_shaderShadow;

		glm::mat4 lightProjection;
		glm::mat4 lightView;
		glm::mat4 lightSpaceMatrix;
	};
}