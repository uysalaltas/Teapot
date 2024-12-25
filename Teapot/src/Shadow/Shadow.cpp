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
		m_shaderShadow.SetUniform1i("shadowMapArr[" + std::to_string(shadowMapIndex) + "]", shadowMapping->GetShadowMapTexture());
		shadowMapIndex++;
	}

	void Shadow::RenderShadow()
	{
		lightView = glm::lookAt(m_lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		switch (m_renderType)
		{
		case Teapot::Shadow::RenderType::Perspective:
			lightProjection = glm::perspective<float>(glm::radians(45.0f), 1.0f, 2.0f, 50.0f);
			break;
		case Teapot::Shadow::RenderType::Ortho:
		default:
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
			break;
		}
		lightSpaceMatrix = lightProjection * lightView;

		shadowMapping->RenderShadow(m_shaderDepth, lightSpaceMatrix);
		ModelManager::DrawModelShadows();

		shadowMapping->UnbindFrameBuffer();
	}
}