#pragma once
#ifdef WINDOW_64_API
namespace Rubber
{
	/*
	* A function that is defined from the application 
	* @return the application object
	*/
     extern Application* CreateApp();
}

int main()
{
	Rubber::Logger::Init();
	auto app = Rubber::CreateApp();
	app->run();
	delete app;
}
#endif

