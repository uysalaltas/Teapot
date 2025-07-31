#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Teapot/Window.h"
#include "Renderer/FrameBuffer.h"
#include "Models/ModelManager.h"

namespace Teapot
{
	class ApplicationWindow : public Window
	{
	public:
		explicit ApplicationWindow(WindowProps& props);
		~ApplicationWindow() override;

		void OnFistUpdate() override;
		void OnLastUpdate() override;
		void UpdateViewport() override;
		void RenderSceneOnImGuiWindow() override;
		void RenderGizmo();

		inline void* GetNativeWindow() const override { return m_Window; }

		inline unsigned int GetWidth() const override  { return m_WindowData.Width; }
		inline unsigned int GetHeigth() const override { return m_WindowData.Height; }
		inline unsigned int& GetWidthRef() override    { return m_WindowData.Width; }
		inline unsigned int& GetHeightRef() override   { return m_WindowData.Height; }

	private:
		void Init();
		void Shutdown();

		GLFWwindow* m_Window;
		WindowProps& m_WindowData;

		void DecomposeMtx(const glm::mat4& m, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) const;
	};
}
