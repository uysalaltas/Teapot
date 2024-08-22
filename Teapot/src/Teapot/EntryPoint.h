#pragma once

#include "Teapot/Window.h"
#include "Platform/ApplicationWindow.h"

namespace Teapot
{
	Window* Window::Create(const WindowProps& props)
	{
		return new ApplicationWindow(props);
	}
}

#ifdef TEA_PLATFORM_WINDOWS
#elif TEA_PLATFORM_LINUX
#endif