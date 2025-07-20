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
		}
		catch (std::runtime_error& e)
		{
			std::cout << "Caught runtime error: " << e.what() << std::endl;
		}

		m_modelHander = std::make_unique<Teapot::ModelHandler>();
		m_windowUI = std::make_unique<Teapot::WindowControlUI>();
		m_modelReader = std::make_unique<Teapot::ModelReader>(m_modelHander);
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
			Teapot::ModelHandlerInterface::RunAwakeModels();
			Teapot::SceneContext::Get().GetWindow().BindFrameBuffer();
			Teapot::SceneContext::Get().GetWindow().UpdateViewport();
			Teapot::ModelHandlerInterface::RunDrawModels();
			OnUpdate();
			Teapot::SceneContext::Get().GetWindow().UnbindFrameBuffer();
			Teapot::SceneContext::Get().GetWindow().OnLastUpdate();
		}
	}
}
