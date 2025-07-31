#include "DebugModelHandler.h"
#include "DebugModel.h"
#include "Scene/SceneContext.h"

namespace Teapot
{
	DebugModelHandler::DebugModelHandler()
	{
		modelHandlerName = "Debug Model";
	}

	std::shared_ptr<Teapot::ModelInterface> DebugModelHandler::CreateModel(Shapes::Shape& shapes, const std::string& nameObject)
	{
		auto model = std::make_shared<Teapot::DebugModel>(shapes, nameObject);
		s_Models->push_back(model);
		return model;
	}

	std::shared_ptr<Teapot::ModelInterface> DebugModelHandler::CreateModel(const std::string& pathObject, const std::string& nameObject)
	{
		return std::shared_ptr<Teapot::ModelInterface>();
	}

	void DebugModelHandler::RunAwake()
	{
		// No action to run
	}

	void DebugModelHandler::DrawModels()
	{
		m_shader.Bind();
		m_shader.SetUniformMat4f("view", Teapot::SceneContext::Get().GetCamera().GetViewMatrix());
		m_shader.SetUniformMat4f("projection", Teapot::SceneContext::Get().GetCamera().GetProjMatrix());
		for (const auto& model : *s_Models)
		{
			m_shader.SetUniformMat4f("model", model->objModel);
			model->Draw(m_shader);
		}
		m_shader.Unbind();
	}
}

