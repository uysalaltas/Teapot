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

	bool SceneContext::Init() 
	{
		if (!s_SceneContext) {
			s_SceneContext = std::make_unique<SceneContext>();
			return true;
		}
		else {
			std::cerr << "SceneContext already initialized!\n";
			return false;
		}
	}

	bool SceneContext::CreateWindow(const Teapot::WindowProps& props)
	{
		m_Window = std::unique_ptr<Teapot::Window>(Window::Create(props));
		if(m_Window) return true;
		return false;
	}

	bool SceneContext::CreateCamera()
	{
		m_Camera = std::make_unique<Teapot::Camera>(cameraPos, cameraCenter, cameraUp, m_Window->GetWidthRef(), m_Window->GetHeightRef());
		if (m_Camera) return true;
		return false;
	}



}