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

		glfwSetErrorCallback(error_callback);
		
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		//sceneBuffer = new FrameBuffer(props.Width, props.Height);
	}

	void LinuxWindow::OnFistUpdate()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void LinuxWindow::OnLastUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void LinuxWindow::UpdateViewport()
	{
		glViewport(0, 0, m_Data.Width, m_Data.Height);
		//sceneBuffer->RescaleFrameBuffer(m_Data.Width, m_Data.Height);
	}

	void LinuxWindow::RenderSceneOnImGuiWindow()
	{
	}

	void LinuxWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}