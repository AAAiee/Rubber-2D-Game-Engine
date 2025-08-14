#pragma once
#include "Rubber/Layer/LayerStack.h"

namespace Rubber
{
	//fwd
	class Window;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class GameLoopTimer;
	class EventManager;
	class Camera;
	class ImGuiLayer;
	class AssetManager;


	class  Application
	{
	public:
		Application(const std::string_view name="");
		virtual ~Application();

	public:

		static Window& getWindow();

		static Ref<EventManager> getEventManager() {
			return s_Instance->m_Em;
		};

		static const Scope<GameLoopTimer>& getTimer(){
			return s_Instance->m_Timer;
		}

		static Ref<AssetManager> getAssetManager() {
			return s_Instance->m_AssetManager;
		}

	public:
		void run();
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

	private:
		inline Window& getWindowImpl() {
			return *m_Window;
		}
		void attachAll();

	private:
		bool m_Runing = true;

		//when minimized, stop layer update, (but keep ImGui update)
		bool m_IsWindowMinimized = false;

		//one application-> one instance
		static Application* s_Instance;

		bool m_FirstRun = true;

	private:
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		Ref<EventManager> m_Em;
		Scope<GameLoopTimer> m_Timer;
		Ref<AssetManager> m_AssetManager;
	
	private:
		bool onWindowClose(const WindowCloseEvent& e);
		bool onWindowResize(const WindowResizeEvent& e);

	private:
		static const float FIXED_TIME_STAMP;
	};
	
}
