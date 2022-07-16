#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

#include "Teapot/Window.h"

namespace Teapot
{
	class LinuxWindow : public Window
	{
	public:
		LinuxWindow(const WindowProps& props);
		virtual ~LinuxWindow();

		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeigth() const override { return m_Data.Height; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		static void error_callback(int error, const char* description)
		{
			fprintf(stderr, "Error: %s\n", description);
		}

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