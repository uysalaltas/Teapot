#pragma once

#ifdef TEA_PLATFORM_WINDOWS

extern Teapot::Application* Teapot::CreateApplication();

int main(int argc, char** argv) 
{
	auto app = Teapot::CreateApplication();
	app->Run();
	delete app;
}

#endif