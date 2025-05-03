#include "SceneContext.h"

#include <iostream>

namespace Teapot
{
	std::unique_ptr<SceneContext> SceneContext::s_SceneContext = nullptr;

	SceneContext& SceneContext::Get()
	{
		if (!s_SceneContext) 
		{
			std::cerr << "SceneContext not initialized!\n";
			std::terminate();
		}
		return *s_SceneContext;
	}

	void SceneContext::Init() 
	{
		if (!s_SceneContext) {
			s_SceneContext = std::make_unique<SceneContext>();
		}
		else {
			std::cerr << "SceneContext already initialized!\n";
		}
	}

	void SceneContext::CreateWindow(const Teapot::WindowProps& props)
	{
		m_Window = std::unique_ptr<Teapot::Window>(Window::Create(props));
	}

	void SceneContext::CreateCamera()
	{
		m_Camera = std::make_unique<Teapot::Camera>(cameraPos, cameraCenter, cameraUp, m_Window->GetWidthRef(), m_Window->GetHeigthRef());
	}



}