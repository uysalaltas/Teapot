#include "Shadow.h"

namespace Teapot
{
	Shadow::Shadow(const Shader& shaderShadow, const Shader& shaderDepth)
		: m_shaderShadow(shaderShadow)
		, m_shaderDepth(shaderDepth)
	{
		shadowMapping = std::make_unique<ShadowMapping>();
		m_shaderShadow.Bind();
		m_shaderShadow.SetUniform1i("shadowMapArr[" + std::to_string(shadowMapping->GetShadowMapTexture() - 1) + "]", shadowMapping->GetShadowMapTexture());
	}

	void Shadow::RenderShadow(const glm::vec3& lightPos, const std::vector<Model*>& models, RenderType renderType)
	{
		glCullFace(GL_FRONT);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		switch (renderType)
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
		for (const auto& model : models)
		{
			model->DrawShadow();
		}

		shadowMapping->UnbindFrameBuffer();
		glCullFace(GL_BACK);
	}
}