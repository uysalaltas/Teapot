#pragma once
#include "Teapot/Window.h"
#include "Camera/Camera.h"

#include <memory>

namespace Teapot
{
	class SceneContext
	{
	public:
		SceneContext() = default;
		static SceneContext& Get();
		static bool Init();
		bool CreateWindow(const Teapot::WindowProps& props);
		bool CreateCamera();

		inline Window& GetWindow() { return *m_Window; }
		inline Camera& GetCamera() { return *m_Camera; }

		bool IsGizmoActive = false;
		int SelectedGizmo = 7; // ImGuizmo Translate enum number

	private:
		static std::unique_ptr<SceneContext> s_SceneContext;
		
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Camera> m_Camera;

		glm::vec3 cameraPos{ 3.0f, 3.0f, 3.0f };
		glm::vec3 cameraCenter{ 0.0f, 0.0f, 0.0f };
		glm::vec3 cameraUp{ 0.0f, 0.0f, 1.0f };

	};
}

