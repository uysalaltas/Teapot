#include "LinuxWindow.h"

namespace Teapot
{
	static bool s_GLFWInitialized = false;

	LinuxWindow::LinuxWindow(const WindowProps& props)
	{
		Init(props);
	}

	LinuxWindow::~LinuxWindow()
	{
		Shutdown();
	}

	void LinuxWindow::Init(const WindowProps& props)
	{
		std::cout << "Linux Window Init" << std::endl;
		m_Data.Title = props.Title;
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
	}

	void LinuxWindow::OnUpdate()
	{
		glClearColor(0.8f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void LinuxWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}