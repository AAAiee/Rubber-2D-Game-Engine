#pragma once
#include <Rubber/Core.h>
#include <memory>
#include "Rubber/LayerStack.h"

namespace Rubber
{
	class Window;
	class Event;
	class WindowCloseEvent;
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

        static Application& getInstance();

		// get the window object
		Window& getWindow() const;
	private:
		// a pointer to the window object
		std::unique_ptr<Window> m_Window;
		// Application's running status
		bool m_Runing = true;

		// a layerStack to manage all the layers
		LayerStack m_LayerStack;

		// a static instance of the application
		static Application* s_Instance;
	
	private:
		// handling event delegate
		bool onWindowClose(WindowCloseEvent& event);
	};
	
}
