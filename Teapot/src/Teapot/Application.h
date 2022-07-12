#include <iostream>
#include <GLFW/glfw3.h>

#include "Core.h"
#include "Window.h"

namespace Teapot 
{
	class TEAPOT_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		Window& GetWindow() { return *m_Window; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
}

