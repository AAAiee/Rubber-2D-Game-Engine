#pragma once
#include <Rubber/Application.h>
#include "Rubber/Log.h"
#include <iostream>

#ifdef WINDOW_64_API
namespace Rubber
{
     extern Application* CreateApp();
}

int main()
{
	Rubber::Logger::Init();
	RB_CRITICAL("this is a fatal error");
	CRITICAL("THIS IS A FATAL ERROR FROM CLINET");
	auto app = Rubber::CreateApp();
	app->run();
	delete app;
}
#endif

