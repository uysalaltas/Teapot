#include "Application.h"
#include "EntryPoint.h"

namespace Teapot
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		std::cout << "Hello From API" << std::endl;

		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
		std::cout << "Destructed API" << std::endl;
	}

	void Application::Run()
	{
		m_Window->RenderSceneOnImGuiWindow();

		while (m_Running)
		{
			m_Window->OnFistUpdate();
			OnUpdate();
			m_Window->OnLastUpdate();
		}
	}
}
