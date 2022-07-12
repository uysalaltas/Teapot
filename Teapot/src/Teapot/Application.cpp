#include "Application.h"

namespace Teapot
{
	Application::Application()
	{
		std::cout << "Hello From API" << std::endl;

		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
		std::cout << "Destructed API" << std::endl;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}	
}
