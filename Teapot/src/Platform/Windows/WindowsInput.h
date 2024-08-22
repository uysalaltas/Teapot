#pragma once

#include "Teapot/Application.h"
#include "Teapot/Input.h"
#include <GLFW/glfw3.h>

namespace Teapot
{
	class WindowsInput : public Input
	{
	protected:
		bool IsKeyPressedImpl(int keycode) override;
		bool IsMouseButtonPressedImpl(int button) override;
		std::pair<float, float> GetMousePositionImpl() override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
	};
}