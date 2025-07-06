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
		glm::vec4 BackgroundColor;

		WindowProps
		(
			const std::string& title = "Teapot Engine",
			unsigned int width = 1280,
			unsigned int height = 720,
			glm::vec4 backgroundColor = glm::vec4{ 0.17f, 0.21f, 0.27f, 1.0f }
		) 
			: Title(title), Width(width), Height(height), BackgroundColor(backgroundColor)
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
		inline void ActivateSnap(const float x, const float y, const float z)
		{
			snap[0] = x;
			snap[1] = y;
			snap[2] = z;
		}

		virtual void* GetNativeWindow() const = 0;
		virtual unsigned int& GetWidthRef() = 0;
		virtual unsigned int& GetHeightRef() = 0;

	public:
		std::unique_ptr <FrameBuffer> sceneBuffer;
		float snap[3] { 0.0f, 0.0f, 0.0f };
	};
}