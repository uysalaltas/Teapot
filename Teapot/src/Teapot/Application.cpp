#include "Application.h"
#include "EntryPoint.h"

namespace Teapot
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const Teapot::WindowProps& props)
	{
		s_Instance = this;
		std::cout << "Hello From API" << std::endl;

		Teapot::SceneContext::Init();
		Teapot::SceneContext::Get().CreateWindow(props);
		Teapot::SceneContext::Get().CreateCamera();

		shaderManager = Teapot::ShaderManager::GetInstance();
		windowUI = std::make_unique<Teapot::WindowControlUI>();
	}

	Application::~Application()	
	{
		std::cout << "Destructed API" << std::endl;
	}

	void Application::Run()
	{
		Teapot::SceneContext::Get().GetWindow().RenderSceneOnImGuiWindow();

		while (m_Running)
		{
			Teapot::SceneContext::Get().GetWindow().OnFistUpdate();
			OnUpdateAwake();
			shaderManager->RenderShadow();
			Teapot::SceneContext::Get().GetWindow().BindFrameBuffer();
			Teapot::SceneContext::Get().GetWindow().UpdateViewport();
			shaderManager->RunShader();
			OnUpdate();
			Teapot::SceneContext::Get().GetWindow().UnbindFrameBuffer();
			Teapot::SceneContext::Get().GetWindow().OnLastUpdate();
		}
	}
}
