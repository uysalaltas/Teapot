#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include "Teapot/Window.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Model.h"

namespace Teapot
{
	class WindowsWindow : public Window
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		WindowsWindow(WindowsWindow const&) = delete;
		WindowsWindow& operator=(WindowsWindow const&) = delete;

		void OnFistUpdate() override;
		void OnLastUpdate() override;
		void UpdateViewport() override;
		void RenderSceneOnImGuiWindow() override;
		void RenderGizmo() const;
		void ActivateGizmo(std::shared_ptr<Camera> camera) override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

		inline unsigned int GetWidth() const override  { return m_WindowData.Width; }
		inline unsigned int GetHeigth() const override { return m_WindowData.Height; }
		inline unsigned int& GetWidthRef() override    { return m_WindowData.Width; }
		inline unsigned int& GetHeigthRef() override   { return m_WindowData.Height; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width{};
			unsigned int Height{};
		};
		WindowData m_WindowData;

		std::shared_ptr<Camera> m_camera;
	};
}
