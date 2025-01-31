#include <pch.h>
#include "Application.h"
#include "Event/KeyEvent.h"
#include "Rubber/Log.h"
#include "Event/AppEvent.h"
#include "Event/MouseEvent.h"
#include "Rubber/Core.h"

namespace Rubber
{
	Application::Application()
	{

	}
	Application::~Application()
	{

	}
	void Application::run()
	{
		RB_TRACE("Welcome to the engine!");
		AppTickEvent tick;
		AppRenderEvent render;
		WindowCloseEvent close;
		WindowResizeEvent resize(1280, 720);
		RB_TRACE(tick.toString());
		RB_INFO(render.toString());
		RB_CRITICAL(close.toString());
		RB_ERROR(resize.toString());
		while (true) {};
	}
	
}
