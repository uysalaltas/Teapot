#include "Application.h"
#include "EntryPoint.h"
#include "Shader/ShaderManager.h"

namespace Teapot
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const Teapot::WindowProps& props)
	{
		s_Instance = this;
		std::cout << "Hello From API" << std::endl;

		m_Window = std::unique_ptr<Window>(Window::Create(props));
		camera = std::make_shared<Teapot::Camera>(cameraPos, cameraCenter, cameraUp, GetWindow().GetWidthRef(), GetWindow().GetHeigthRef());

		m_Window->ActivateGizmo(camera);
		shaderManager = Teapot::ShaderManager::GetInstance();
		shaderManager->SetShaderValues(*camera);

		windowUI = std::make_unique<Teapot::WindowControlUI>(*m_Window);
	}

	Application::~Application()	
	{
		std::cout << "Destructed API" << std::endl;
	}

	void Application::Run()
	{
		m_Window->RenderSceneOnImGuiWindow();

		while (m_Running)
		{
			m_Window->OnFistUpdate();
			OnUpdateAwake();
			shaderManager->RenderShadow();
			GetWindow().BindFrameBuffer();
			m_Window->UpdateViewport();
			shaderManager->RunShader();
			OnUpdate();
			GetWindow().UnbindFrameBuffer();
			m_Window->OnLastUpdate();
		}
	}
}
