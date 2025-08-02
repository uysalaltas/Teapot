#include "Shadow.h"

namespace Teapot
{
	static int shadowMapIndex = 0;

	Shadow::Shadow(const Shader& shaderShadow, const Shader& shaderDepth, const glm::vec3& lightPos, RenderType renderType)
		: m_shaderShadow(shaderShadow)
		, m_shaderDepth(shaderDepth)
		, m_lightPos(lightPos)
		, m_renderType(renderType)
	{
		shadowMapping = std::make_unique<ShadowMapping>();
		m_shaderShadow.Bind();
		auto shadowMapArr = std::format("shadowMapArr[{}]", std::to_string(shadowMapIndex));
		m_shaderShadow.SetUniform1i(shadowMapArr, shadowMapping->GetShadowMapTexture());
		shadowMapIndex++;
	}

	void Shadow::RenderShadow()
	{
		m_lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		if (Shadow::RenderType::Perspective == m_renderType)
		{
			m_lightProjection = glm::perspective<float>(glm::radians(45.0f), 1.0f, 2.0f, 50.0f);
		}
		else // Shadow::RenderType::Ortho:
		{
			m_lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
		}

		m_lightSpaceMatrix = m_lightProjection * m_lightView;

		shadowMapping->RenderShadow(m_shaderDepth, m_lightSpaceMatrix);
	}
}