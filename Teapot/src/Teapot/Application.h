#include <iostream>
#include <GLFW/glfw3.h>
#include <memory>

#include "Core.h"
#include "Window.h"
#include "Renderer/ShaderManager.h"

namespace Teapot 
{
	class TEAPOT_API Application
	{
	public:
		explicit Application(const Teapot::WindowProps& props);
		virtual ~Application();
		virtual void OnUpdate() = 0;
		virtual void OnUpdateAwake() = 0;
		void Run();
		Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		std::shared_ptr<ShaderManager> shaderManager;
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		static Application* s_Instance;
	};
}