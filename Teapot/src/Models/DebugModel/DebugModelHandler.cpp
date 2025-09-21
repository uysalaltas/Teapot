#include "DebugModelHandler.h"
#include "Scene/SceneContext.h"

namespace Teapot
{
	DebugModelHandler::DebugModelHandler()
	{
		modelHandlerName = "Debug Model";
		modelType = ModelType::debugModel;
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
		for (const auto& model : *models)
		{
			m_shader.SetUniformMat4f("model", model->objModel);
			model->Draw(m_shader);
		}
		m_shader.Unbind();
	}
}

