#pragma once

#include <iostream>
#include <memory>

#include "Core.h"
#include "Window.h"
#include "Ui/WindowControlUI.h"
#include "Shader/ShaderManager.h"
#include "Scene/SceneContext.h"
#include "ModelReader.h"

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
		inline static Application& Get() { return *s_Instance; }

		inline Window& GetWindow() { return Teapot::SceneContext::Get().GetWindow(); }
		inline Camera& GetCamera() { return Teapot::SceneContext::Get().GetCamera(); }
		inline WindowControlUI& GetUI() { return *windowUI; }
		inline ShaderManager& GetShaderManager() { return *shaderManager; }

	private:
		std::shared_ptr<Teapot::ShaderManager> shaderManager;
		std::unique_ptr<Teapot::WindowControlUI> windowUI;

		bool m_Running = true;
		static Application* s_Instance;
	};
}