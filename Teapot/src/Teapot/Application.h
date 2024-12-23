#pragma once

#include <iostream>
#include <memory>

#include "Core.h"
#include "Window.h"
#include "Ui/WindowControlUI.h"
#include "Shader/ShaderManager.h"
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
		Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

		glm::vec3 cameraPos{ 3.0f, 3.0f, 3.0f };
		glm::vec3 cameraCenter{ 0.0f, 0.0f, 0.0f };
		glm::vec3 cameraUp{ 0.0f, 0.0f, 1.0f };

		std::shared_ptr<ShaderManager> shaderManager;
		std::shared_ptr<Teapot::Camera> camera;
		std::unique_ptr<Teapot::WindowControlUI> windowUI;

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		static Application* s_Instance;
	};
}