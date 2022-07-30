#include "Teapot.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox() 
	{
		//Contructor
	}
	~Sandbox()
	{
		//Destructor
	}

	void OnUpdate() override
	{
		//Loop
	}
};

int main()
{
	Sandbox* s = new Sandbox();
	s->Run();
	s->OnUpdate();
	delete(s);
}