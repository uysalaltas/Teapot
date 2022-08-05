#include <iostream>
#include <GLFW/glfw3.h>
#include <memory>

#include "Core.h"
#include "Window.h"

namespace Teapot 
{
	class TEAPOT_API Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void OnUpdate() = 0;
		void Run();
		Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		static Application* s_Instance;
	};
}