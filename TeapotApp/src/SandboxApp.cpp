#include "Teapot.h"

//class Sandbox : public Teapot::Application
//{
//public:
//	Sandbox() 
//	{
//		std::cout << "Hello From Sandbox" << std::endl;
//	}
//	~Sandbox()
//	{
//		std::cout << "Hello From Sandbox" << std::endl;
//	}
//};
//
//Teapot::Application* Teapot::CreateApplication()
//{
//	return new Sandbox();
//}

int main()
{
	Teapot::Application* app = new Teapot::Application();
	app->Run();

}