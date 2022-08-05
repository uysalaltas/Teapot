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

		//Key Press Example
		if (Teapot::Input::IsKeyPressed(TEA_KEY_A))
		{
			std::cout << "A Pressed" << std::endl;
		}
	}
};

int main()
{
	Sandbox* s = new Sandbox();
	s->Run();
	s->OnUpdate();
	delete(s);
}