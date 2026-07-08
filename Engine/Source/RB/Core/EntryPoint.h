#pragma once
#ifdef WINDOW_64_API
namespace RB
{
	/*
	* A function that is defined from the application 
	* @return the application object
	*/
     extern Application* CreateApp();
}

int main()
{
	RB_PROFILE_BEGIN_SESSION("RB-StartUp", "RB-StartUp.json");
	RB::Logger::Init();
	auto app = RB::CreateApp();
	RB_PROFILE_END_SESSION();


	RB_PROFILE_BEGIN_SESSION("RB-Runtime", "RB-Runtime.json");
	app->run();
	RB_PROFILE_END_SESSION();


	RB_PROFILE_BEGIN_SESSION("RB-Cleanup", "RB-Cleanup.json");
	delete app;
	RB_PROFILE_END_SESSION();
}
#endif

