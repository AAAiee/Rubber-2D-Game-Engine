#pragma once
#include <Rubber/Core.h>
#include "Rubber/Window.h"
#include "Rubber/Event/AppEvent.h"
#include "Rubber/LayerStack.h"

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
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
	private:
		// a pointer to the window object
		std::unique_ptr<Window> m_Window;
		// Application's running status
		bool m_Runing = true;
		bool onWindowClose(WindowCloseEvent& event);
		LayerStack m_LayerStack;
	};
	
}
