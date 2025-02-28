#include "pch.h"
#include "WindowsWindow.h"
#include "Rubber/Log.h"
#include "GLFW/glfw3.h"

namespace Rubber {
	// false by default, glfw not initialized yet
	// only want to initialize glfw once
	bool WindowsWindow::s_GLFWInitialized = false;

	//provide a definition for Window createWindow
	std::unique_ptr<Window> Window::create(const WindowProps& props )
	{
		return std::make_unique<WindowsWindow>(props);
	}

	//constructor delegate initialization to init
	Rubber::WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	Rubber::WindowsWindow::~WindowsWindow()
	{
		shutDown();
	}

	void Rubber::WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}


	void Rubber::WindowsWindow::setVsync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.Vsync = enabled;
	}

	bool Rubber::WindowsWindow::isVsync() const
	{
		return m_Data.Vsync;
	}

	void Rubber::WindowsWindow::init(const WindowProps& props)
	{
		m_Data.title = props.title;
		m_Data.height = props.height;	
		m_Data.width = props.width;

		RB_INFO("Creating Window: {0} ({1} {2})", m_Data.title, m_Data.width, m_Data.height);
		if (!s_GLFWInitialized)
		{
			bool success = glfwInit();
			RB_CORE_ASSERT(success, "Could not initialize GLFW!");

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVsync(true);
		RB_INFO("Window Created!: {0} ({1} {2})", m_Data.title, m_Data.width, m_Data.height);
	}

	void Rubber::WindowsWindow::shutDown()
	{
		glfwDestroyWindow(m_Window);
	}
}
