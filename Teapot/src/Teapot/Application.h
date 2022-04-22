#pragma once

#include "Core.h"

namespace Teapot 
{
	class TEAPOT_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
}

