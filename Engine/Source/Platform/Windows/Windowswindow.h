#pragma once
#include "RB/Window/Window.h"
#include "RB/Event/EventManager.h"
#include <optional>

#include <GLFW/glfw3.h>
namespace RB
{   
	class GraphicsContext;
	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;
		inline unsigned int getWidth() const override { return m_Data.width; }
		inline unsigned int getHeight() const override { return m_Data.height; }
		inline void* getNativeWindow() const override { return m_Window; }
		
		
		// Window Attributes
		inline void setEventManager(Ref<EventManager>& em) override {
			m_Data.em = em;
		}
		void setVsync(bool enabled) override;
		bool isVsync() const override;
	private:
		// initialization && ShutDown
		virtual void init(const WindowProps& props) ;
		virtual void shutDown();

	private:
		// make sure GLFW is initialized only once
		static bool s_GLFWInitialized;
	    // our window object
		GLFWwindow* m_Window;
		// group our data into a struct
		struct WindowInfo
		{
			std::string title;
			unsigned int width = 1280;
			unsigned int height = 720;
			bool Vsync = false;
			std::optional<Ref<EventManager>> em = std::nullopt;
		};
		WindowInfo m_Data;

		GraphicsContext* m_ContextManager;
	};
}

