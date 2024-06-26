#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Teapot/Window.h"
#include "Renderer/FrameBuffer.h"

namespace Teapot
{
	class LinuxWindow : public Window
	{
	public:
		LinuxWindow(const WindowProps& props);
		virtual ~LinuxWindow();

		void OnFistUpdate() override;
		void OnLastUpdate() override;
		void UpdateViewport() override;
		void RenderSceneOnImGuiWindow() override;
		void ActivateGizmo(std::shared_ptr<Camera> camera) override {};

		inline virtual void* GetNativeWindow() const { return m_Window; }

		inline unsigned int GetWidth() const override { return m_WindowData.Width; }
		inline unsigned int GetHeigth() const override { return m_WindowData.Height; }
		inline unsigned int& GetWidthRef() override { return m_WindowData.Width; }
		inline unsigned int& GetHeigthRef() override { return m_WindowData.Height; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		static void error_callback(int error, const char* description)
		{
			fprintf(stderr, "Error: %s\n", description);
		}

	private:
		//FrameBuffer* sceneBuffer;
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
		};
		WindowData m_WindowData;
	};
}