#pragma once
#include "Rubber/Window.h"

#include <GLFW/glfw3.h>
namespace Rubber
{
	class WindowsWindow :public Window
	{
	public:
		// constructors and destructors
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		//override methods from Window
		void onUpdate() override;
		inline unsigned int getWidth() const override { return m_Data.width; }
		inline unsigned int getHeight() const override { return m_Data.height; }
		inline GLFWwindow* getWindow() const {
			return m_Window;
		}
		
		// Window Attributes
		inline void setEventCallBack (const EventCallBackFn& callback) override {
			m_Data.eventCallBack = callback;
		}
		void setVsync(bool enabled) override;
		bool isVsync() const override;
	private:
		// initialization && ShutDown
		virtual void init(const WindowProps& props);
		virtual void shutDown();

	private:
		// make sure glfw is initialized only once
		static bool s_GLFWInitialized;
	    // our window object
		GLFWwindow* m_Window;
		// group our data into a sturct
		struct WindowInfo
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool Vsync;
			EventCallBackFn eventCallBack;
		};
		WindowInfo m_Data;
	};
}

