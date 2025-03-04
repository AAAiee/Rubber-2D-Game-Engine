#include <pch.h>
#include "Application.h"
#include "Event/KeyEvent.h"
#include "Event/AppEvent.h"
#include "Event/MouseEvent.h"
#include "Rubber/Core.h"
#include "Platform/Windowswindow.h"
#include "GLFW/glfw3.h"

// bind event callback  
#define BIND_EVENT_FN(x)  std::bind(&Application::x, this, std::placeholders::_1)

namespace Rubber
{
	Application::Application()
	{
		// create a window when an application instance is created
		m_Window = Window::create();
		m_Window->setEventCallBack(BIND_EVENT_FN(onEvent));
	}
	Application::~Application()
	{

	}

	void Application::onEvent(Event& e)
	{
		// when a window event happens, the eventcallbackfn automatically 
		// passed event happening to here
		RB_INFO("{0}", e.toString());
		// use a dispatcher to store the event and handle it 
		EventDispatcher dispacher(e);
		dispacher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
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

	// this is what we want happen with a window close event
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
		
	
}
