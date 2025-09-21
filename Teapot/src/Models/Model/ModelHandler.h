#pragma once
#include "Models/ModelHandlerInterface.h"
#include "Light/Light.h"

namespace Teapot
{
	class ModelHandler : public ModelHandlerInterface
	{
	public:
		ModelHandler();

		void RunAwake() override;
		void DrawModels() override;

		Teapot::Light light;
	
	private:
		Teapot::Shader m_shaderDepthBasic{ SHADERPATH + "BasicDepth.shader" };
		Teapot::Shader m_shader{ SHADERPATH + "MaterialShader.shader" };
	};
}