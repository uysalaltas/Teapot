#include "Shadow.h"

namespace Teapot
{
	Shadow::Shadow(Shader& shaderShadow, Shader& shaderDepth)
		: m_shaderShadow(shaderShadow)
		, m_shaderDepth(shaderDepth)
	{
		shadowMapping = new ShadowMapping();
		// shader configuration
		// --------------------
		m_shaderShadow.Bind();
		m_shaderShadow.SetUniform1i("shadowMap", 1);
	}

	Shadow::~Shadow()
	{
		delete shadowMapping;
	}

	void Shadow::RenderShadow(glm::vec3& lightPos, std::vector<Model*>& models)
	{
		glCullFace(GL_FRONT);
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		lightSpaceMatrix = lightProjection * lightView;

		shadowMapping->RenderShadow(m_shaderDepth, lightSpaceMatrix);
		for (int i = 0; i < models.size(); i++)
		{
			models[i]->Draw(m_shaderDepth);
		}

		shadowMapping->UnbindFrameBuffer();
		glCullFace(GL_BACK);
	}
}