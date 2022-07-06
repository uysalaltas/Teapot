#include "Application.h"

namespace Teapot
{
	Application::Application()
	{
		std::cout << "Hello From API" << std::endl;
	}

	Application::~Application()
	{
		std::cout << "Destructed API" << std::endl;
	}

	void Application::Run()
	{
		while (true)
		{
			//std::cout << "Hello From API Loop" << std::endl;
		}
	}	
}
