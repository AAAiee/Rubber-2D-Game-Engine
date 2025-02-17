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
		m_Window = Window::create();
	}
	Application::~Application()
	{

	}
	void Application::run()
	{
		while (m_Runing)
		{
			m_Window->onUpdate();
		}
	}
	
}
