#pragma once
#pragma once
#include "ModelHandlerInterface.h"

namespace Teapot
{
	class DebugModelHandler : public ModelHandlerInterface
	{
	public:
		DebugModelHandler();

		std::shared_ptr<Teapot::ModelInterface> CreateModel(const Shapes::Shape& shapes, const std::string& nameObject) override;
		std::shared_ptr<Teapot::ModelInterface> CreateModel(const std::string& pathObject, const std::string& nameObject) override;

		void RunAwake() override;
		void DrawModels() override;

	private:
		Teapot::Shader m_shader{ SHADERPATH + "BasicDebug.shader" };
	};
}