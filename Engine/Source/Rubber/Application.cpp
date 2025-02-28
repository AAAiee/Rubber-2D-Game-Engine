#include <pch.h>
#include "Application.h"
#include "Event/KeyEvent.h"
#include "Rubber/Log.h"
#include "Event/AppEvent.h"
#include "Event/MouseEvent.h"
#include "Rubber/Core.h"
#include "Platform/Windowswindow.h"
#include "GLFW/glfw3.h"


namespace Rubber
{
	Application::Application()
	{
		// create a window when an application instance is created
		m_Window = Window::create();
	}
	Application::~Application()
	{

	}
	void Application::run()
	{
		while (m_Runing)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->onUpdate();
		}
	}
	
}
