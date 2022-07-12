#include <iostream>

#include "Core.h"
#include <GLFW/glfw3.h>

namespace Teapot 
{
	class TEAPOT_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		GLFWwindow* window;
	};

	Application* CreateApplication();
}

