#include "Application.h"

namespace Teapot
{
	Application* Application::s_Instance = nullptr;

	Application::Application(Teapot::WindowProps& props)
	{
		s_Instance = this;
		std::cout << "Hello From API" << std::endl;

		try
		{
			if (!Teapot::SceneContext::Init()) { throw std::runtime_error("Scene not initalized"); }
			if (!Teapot::SceneContext::Get().CreateWindow(props)) { throw std::runtime_error("Window not created"); }
			if (!Teapot::SceneContext::Get().CreateCamera()) { throw std::runtime_error("Camera not created"); }
			if (!Teapot::ShaderManager::Init()) { throw std::runtime_error("ShaderManager not initalized"); }
		}
		catch (std::runtime_error& e)
		{
			std::cout << "Caught runtime error: " << e.what() << std::endl;
		}

		windowUI = std::make_unique<Teapot::WindowControlUI>();
	}

	Application::~Application()	
	{
		std::cout << "Destructed API" << std::endl;
	}

	void Application::Run()
	{
		Teapot::SceneContext::Get().GetWindow().RenderSceneOnImGuiWindow();

		while (m_Running)
		{
			Teapot::SceneContext::Get().GetWindow().OnFistUpdate();
			OnUpdateAwake();
			Teapot::ShaderManager::GetInstance().RenderShadow();
			Teapot::SceneContext::Get().GetWindow().BindFrameBuffer();
			Teapot::SceneContext::Get().GetWindow().UpdateViewport();
			Teapot::ShaderManager::GetInstance().RunShader();
			OnUpdate();
			Teapot::SceneContext::Get().GetWindow().UnbindFrameBuffer();
			Teapot::SceneContext::Get().GetWindow().OnLastUpdate();
		}
	}
}
