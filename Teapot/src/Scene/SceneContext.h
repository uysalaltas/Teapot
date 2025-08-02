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

		inline Window& GetWindow() { return *mp_window; }
		inline Camera& GetCamera() { return *mp_camera; }
		inline int& GetSelectedGizmo() { return m_selectedGizmo; }
		inline bool& IsGizmoActive() { return m_isGizmoActive; }


	private:
		static std::unique_ptr<SceneContext> s_SceneContext;

		std::unique_ptr<Window> mp_window;
		std::unique_ptr<Camera> mp_camera;

		glm::vec3 m_cameraPos{ 3.0f, 3.0f, 3.0f };
		glm::vec3 m_cameraCenter{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_cameraUp{ 0.0f, 1.0f, 0.0f };

		int m_selectedGizmo = 7; // ImGuizmo Translate enum number
		bool m_isGizmoActive = false;

	};
}

