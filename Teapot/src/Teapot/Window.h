#pragma once

#include <string>

#include "Renderer/FrameBuffer.h"
#include "Core.h"
#include "Camera/Camera.h"
#include <memory>

namespace Teapot
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps
		(
			const std::string& title = "Teapot Engine",
			unsigned int width = 1280,
			unsigned int height = 720
		) 
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class TEAPOT_API Window
	{
	public:
		virtual ~Window() = default;
		virtual void OnFistUpdate() = 0;
		virtual void OnLastUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeigth() const = 0;
		virtual void UpdateViewport() = 0;

		virtual void RenderSceneOnImGuiWindow() = 0;
		inline void BindFrameBuffer() const { sceneBuffer->Bind(); }
		inline void UnbindFrameBuffer() const { sceneBuffer->Unbind(); }

		virtual void* GetNativeWindow() const = 0;
		virtual unsigned int& GetWidthRef() = 0;
		virtual unsigned int& GetHeigthRef() = 0;

		virtual void ActivateGizmo(std::shared_ptr<Camera> camera) = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	
	public:
		std::unique_ptr <FrameBuffer> sceneBuffer;
		bool IsGizmoActive{ false };
		int SelectedGizmo{ 7 }; // ImGuizmo Translate enum number
	};
}