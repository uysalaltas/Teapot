#include "ModelHandler.h"
#include "Model.h"

namespace Teapot
{
	ModelHandler::ModelHandler()
		: light(m_shader, m_shaderDepthBasic)
	{
		modelHandlerName = "Model";
		modelType = ModelType::model;
	}

	void ModelHandler::RunAwake()
	{
		if (light.IsShadowActive())
		{
			for (const auto& shadow : light.GetShadows())
			{
				shadow->RenderShadow();
				for (const auto& model : *models)
				{
					m_shaderDepthBasic.SetUniformMat4f("model", model->objModel);
					model->Draw(m_shaderDepthBasic);
				}
				shadow->UnbindShadow();
			}
		}
	}

	void ModelHandler::DrawModels()
	{
		light.RenderShadow();
		light.RunShader();
		for (const auto& model : *models)
		{
			m_shader.SetUniformMat4f("model", model->objModel);
			m_shader.SetUniform1i("hasTexture", model->hasTexture);
			model->Draw(m_shader);
		}
	}
}
