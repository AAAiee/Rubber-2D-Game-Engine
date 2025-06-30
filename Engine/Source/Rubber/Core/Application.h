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


	class  Application
	{
	public:
		Application(const std::string_view name="");
		virtual ~Application();

	public:
		// get the window object
		static Window& getWindow();
		//get the eventManager
		static Ref<EventManager>& getEventManager() {
			return s_Instance->m_Em;
		};
		static Scope<Timer>& getTimer(){
			return s_Instance->m_Timer;
		}

	public:
		void run();
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

	private:
		Window& getWindowImpl() {
			return *m_Window;
		}
		void attachAll();

	private:
		bool m_Runing = true;
		//when minimized, stop layer update, (but keep ImGui update)
		bool m_IsWindowMinimized = false;
		// a static instance of the application
		static Application* s_Instance;
		bool m_FirstRun = true;

	private:
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		Ref<EventManager> m_Em;
		Scope<Timer> m_Timer;
	
	private:
		// handling event delegate
		bool onWindowClose(const WindowCloseEvent& e);
		bool onWindowResize(const WindowResizeEvent& e);
	};
	
}
