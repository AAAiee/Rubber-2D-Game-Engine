#pragma once
#include <Rubber/Core/Core.h>
#include <memory>
#include "Rubber/Layer/LayerStack.h"
#include "Rubber/imGui/ImGuiLayer.h"
#include "Rubber/Renderer/Buffers.h"
#include "Rubber/Renderer/Shaders.h"
#include "Rubber/Renderer/VertexArray.h"
#include "Rubber/Renderer/Camera.h"
#include "Rubber/Timer/Timer.h"

namespace Rubber
{
	class Window;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
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
		Scope<Window> m_Window;
		// Application's running status
		bool m_Runing = true;

		//when minimized, stop layer update, (but keep ImGui update)
		bool m_IsWindowMinimized = false;
		
		// a layerStack to manage all the layers
		LayerStack m_LayerStack;
		// a static instance of the application
		static Application* s_Instance;
		ImGuiLayer* m_ImGuiLayer;

		Timer m_Timer;

	
	private:
		// handling event delegate
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	};
	
}
