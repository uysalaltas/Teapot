#include "ModelHandler.h"

namespace Teapot
{
	ModelHandler::ModelHandler()
		: light(m_shader, m_shaderDepthBasic)
	{
	}

	std::shared_ptr<Teapot::ModelInterface> ModelHandler::CreateModel(const Shapes::Shape& shapes, const std::string& nameObject)
	{
		auto model = std::make_shared<Teapot::Model>(shapes, nameObject);
		s_Models->push_back(model);
		return model;
	}

	std::shared_ptr<Teapot::ModelInterface> ModelHandler::CreateModel(const std::string& pathObject, const std::string& nameObject)
	{
		auto model = std::make_shared<Teapot::Model>(pathObject, nameObject);
		s_Models->push_back(model);
		return model;
	}

	void ModelHandler::RunAwake()
	{
		if (light.IsShadowActive())
		{
			for (const auto& shadow : light.GetShadows())
			{
				shadow->RenderShadow();
				for (const auto& model : *s_Models)
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
		for (const auto& model : *s_Models)
		{
			m_shader.SetUniformMat4f("model", model->objModel);
			m_shader.SetUniform1i("hasTexture", model->hasTexture);
			model->Draw(m_shader);
		}
	}
}
