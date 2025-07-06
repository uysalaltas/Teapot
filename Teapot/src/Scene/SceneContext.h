#pragma once
#include "Camera/Camera.h"
#include "Scene/ApplicationWindow.h"
#include <memory>

namespace Teapot
{
	class SceneContext
	{
	public:
		SceneContext() = default;
		SceneContext(const SceneContext&) = delete; 
		SceneContext & operator=(const SceneContext&) = delete; 
		SceneContext(SceneContext&&) = delete; 
		SceneContext & operator=(SceneContext&&) = delete;

		static SceneContext& Get();
		static bool Init();
		bool CreateWindow(Teapot::WindowProps& props);
		bool CreateCamera();
		void ActivateSnap(const float x, const float y, const float z);

		inline Window& GetWindow() { return *m_Window; }
		inline Camera& GetCamera() { return *m_Camera; }

		bool IsGizmoActive = false;
		int SelectedGizmo = 7; // ImGuizmo Translate enum number

	private:
		static std::unique_ptr<SceneContext> s_SceneContext;

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Camera> m_Camera;

		glm::vec3 m_cameraPos{ 3.0f, 3.0f, 3.0f };
		glm::vec3 m_cameraCenter{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_cameraUp{ 0.0f, 1.0f, 0.0f };

	};
}

