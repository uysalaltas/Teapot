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
#include "Renderer/Model.h"

namespace Teapot
{
	class ApplicationWindow : public Window
	{
	public:
		ApplicationWindow(const WindowProps& props);
		~ApplicationWindow() override;
		ApplicationWindow(ApplicationWindow const&) = delete;
		ApplicationWindow& operator=(ApplicationWindow const&) = delete;

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
		void Init(const WindowProps& props);
		void Shutdown();

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width{};
			unsigned int Height{};
			glm::vec4 BackgroundColor{};
		};
		WindowData m_WindowData;

		void DecomposeMtx(const glm::mat4& m, glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)
		{
			pos = m[3];
			for (int i = 0; i < 3; i++)
				scale[i] = glm::length(glm::vec3(m[i]));
			const glm::mat3 rotMtx(
				glm::vec3(m[0]) / scale[0],
				glm::vec3(m[1]) / scale[1],
				glm::vec3(m[2]) / scale[2]);

			auto rotQuat = glm::quat_cast(rotMtx);
			//rot  = glm::degrees(glm::eulerAngles(rotQuat));
		}
	};
}
