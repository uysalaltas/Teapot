#pragma once

#include "Teapot/Window.h"
#include "Scene/ApplicationWindow.h"

namespace Teapot
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
		return std::unique_ptr<Window>(new ApplicationWindow(props));
	}
}

#ifdef TEA_PLATFORM_WINDOWS
#elif TEA_PLATFORM_LINUX
#endif