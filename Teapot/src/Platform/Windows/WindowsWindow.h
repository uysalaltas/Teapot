#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Teapot/Window.h"
#include "Renderer/FrameBuffer.h"

namespace Teapot
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnFistUpdate() override;
		void OnLastUpdate() override;
		void UpdateViewport() override;
		void RenderSceneOnImGuiWindow() override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeigth() const override { return m_Data.Height; }
		inline unsigned int* GetWidthRef() override { return &m_Data.Width; }
		inline unsigned int* GetHeigthRef() override { return &m_Data.Height; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
		};
		WindowData m_Data;
	};
}
