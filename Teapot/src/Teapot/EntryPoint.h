#pragma once

#include "Teapot/Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/Linux/LinuxWindow.h"

#ifdef TEA_PLATFORM_WINDOWS
namespace Teapot
{
	Window* Window::Create(const WindowProps& props)
	{
		std::cout << "Windows" << std::endl;
		return new WindowsWindow(props);
	}
}
#elif TEA_PLATFORM_LINUX
namespace Teapot
{
	Window* Window::Create(const WindowProps& props)
	{
		std::cout << "Linux" << std::endl;
		return new LinuxWindow(props);
	}
}
#endif