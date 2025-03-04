#pragma once
#include <Rubber/Core.h>
#include <Rubber/Window.h>
#include "Rubber/Event/AppEvent.h"

namespace Rubber
{
	/* Application
	*  this class controls the application 
	*/
	class RB_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
		// Called when windows event callback are triggered
		void onEvent(Event& event);
	private:
		// a pointer to the window object
		std::unique_ptr<Window> m_Window;
		// Application's running status
		bool m_Runing = true;
		bool onWindowClose(WindowCloseEvent& event);
	};
	
}
