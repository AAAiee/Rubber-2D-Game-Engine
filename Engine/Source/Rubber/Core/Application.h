#pragma once
#include "Rubber/Layer/LayerStack.h"

namespace Rubber
{
	class Window;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class Timer;
	class EventManager;
	class Camera;
	class ImGuiLayer;
	/* Application
	*  this class controls the application 
	*/
	class  Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		// get the window object
		static Window& getWindow();

		//get the eventManager
		static Ref<EventManager>& getEventManager() {
			return s_Instance->m_Em;
		};

		static Scope<Timer>& getTimer(){
			return s_Instance->m_Timer;
		}

	private:
		Window& getWindowImpl() {
			return *m_Window;
		}

		void attachAll();

	private:
		Scope<Window> m_Window;
		bool m_Runing = true;
		//when minimized, stop layer update, (but keep ImGui update)
		bool m_IsWindowMinimized = false;
		LayerStack m_LayerStack;
		// a static instance of the application
		static Application* s_Instance;
		ImGuiLayer* m_ImGuiLayer;
		Ref<EventManager> m_Em;
		Scope<Timer> m_Timer;
		bool m_FirstRun = true;

	
	private:
		// handling event delegate
		bool onWindowClose(const WindowCloseEvent& e);
		bool onWindowResize(const WindowResizeEvent& e);
	};
	
}
