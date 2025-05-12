#pragma once
#include <Rubber/Core/Core.h>
#include <memory>
#include "Rubber/Layer/LayerStack.h"
#include "Rubber/imGui/ImGuiLayer.h"

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
		void onEvent(Event& event);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		// get the window object
		static Window& getWindow();
	private:
		Window& getWindowImpl() {
			return *m_Window;
		}

	private:
		// a pointer to the window object
		std::unique_ptr<Window> m_Window;
		// Application's running status
		bool m_Runing = true;
		// a layerStack to manage all the layers
		LayerStack m_LayerStack;
		// a static instance of the application
		static Application* s_Instance;
		ImGuiLayer* m_ImGuiLayer;
	
	private:
		// handling event delegate
		bool onWindowClose(WindowCloseEvent& event);
	};
	
}
