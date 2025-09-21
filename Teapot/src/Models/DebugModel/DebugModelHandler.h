#pragma once

#include "Models/ModelHandlerInterface.h"

namespace Teapot
{
	class DebugModelHandler : public ModelHandlerInterface
	{
	public:
		DebugModelHandler();

		void RunAwake() override;
		void DrawModels() override;

	private:
		Teapot::Shader m_shader{ SHADERPATH + "BasicDebug.shader" };
	};
}