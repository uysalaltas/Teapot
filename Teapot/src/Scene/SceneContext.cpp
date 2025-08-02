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
		if (!s_SceneContext) 
		{
			s_SceneContext = std::make_unique<SceneContext>();
			return true;
		}
		else 
		{
			std::cerr << "SceneContext already initialized!\n";
			return false;
		}
	}

	bool SceneContext::CreateWindow(Teapot::WindowProps& props)
	{
		mp_window = std::make_unique<Teapot::ApplicationWindow>(props);
		if(mp_window) return true;
		return false;
	}

	bool SceneContext::CreateCamera()
	{
		mp_camera = std::make_unique<Teapot::Camera>(m_cameraPos, m_cameraCenter, m_cameraUp, mp_window->GetWidthRef(), mp_window->GetHeightRef());
		if (mp_camera) return true;
		return false;
	}
}