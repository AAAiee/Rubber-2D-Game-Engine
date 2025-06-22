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
	RB_PROFILE_BEGIN_SESSION("Rubber-StartUp", "Rubber-StartUp.json");
	Rubber::Logger::Init();
	auto app = Rubber::CreateApp();
	RB_PROFILE_END_SESSION();


	RB_PROFILE_BEGIN_SESSION("Rubber-Runtime", "Rubber-Runtime.json");
	app->run();
	RB_PROFILE_END_SESSION();


	RB_PROFILE_BEGIN_SESSION("Rubber-Cleanup", "Rubber-Cleanup.json");
	delete app;
	RB_PROFILE_END_SESSION();
}
#endif

