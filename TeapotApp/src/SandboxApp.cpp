#include "Teapot.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox() 
	{
		std::cout << "Sandbox Initialized" << std::endl;
	}
	~Sandbox()
	{
		std::cout << "Sandbox deinitialized" << std::endl;
	}

	void OnUpdate() override
	{
		std::cout << "Loop" << std::endl;
	}
};

int main()
{
	Sandbox* s = new Sandbox();
	s->Run();
	s->OnUpdate();
	delete(s);
}