#pragma once
#include "Models/ModelHandlerInterface.h"
#include "Light/Light.h"

namespace Teapot
{
	class ModelHandler : public ModelHandlerInterface
	{
	public:
		ModelHandler();

		std::shared_ptr<Teapot::ModelInterface> CreateModel(Shapes::Shape& shapes, const std::string& nameObject) override;
		std::shared_ptr<Teapot::ModelInterface> CreateModel(const std::string& pathObject, const std::string& nameObject) override;
		void RunAwake() override;
		void DrawModels() override;

		Teapot::Light light;
	
	private:
		Teapot::Shader m_shaderDepthBasic{ SHADERPATH + "BasicDepth.shader" };
		Teapot::Shader m_shader{ SHADERPATH + "MaterialShader.shader" };
	};
}