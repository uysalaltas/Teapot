#include "Teapot.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox() 
	{

	}
	~Sandbox()
	{

	}
};

Teapot::Application* Teapot::CreateApplication()
{
	return new Sandbox();
}