#pragma once

#include <GLFW/glfw3.h>

#include "Teapot/Window.h"

namespace Teapot
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeigth() const override { return m_Data.Height; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
		};
		WindowData m_Data;
	};
}


