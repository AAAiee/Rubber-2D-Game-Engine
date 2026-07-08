#include "pch.h"
#include <glad/glad.h>

#include "WindowsWindow.h"

#include "RB/Event/AppEvent.h"
#include "RB/Event/KeyEvent.h"
#include "RB/Event/MouseEvent.h"
#include "RB/Renderer/GraphicsContext.h"
#include "Platform/OpenGL/GLContext.h"


namespace RB {
	// false by default, GLFW not initialized yet
	// only want to initialize GLFW once
	bool WindowsWindow::s_GLFWInitialized = false;

	static void glfwErrorCallBack(int error, const char* description)
	{
		RB_ERROR("GLFW ERROR ({0}): {1}", error, description);
	}

	//provide a definition for Window createWindow
	Scope<Window> Window::create(const WindowProps& props )
	{
		return makeScope<WindowsWindow>(props);
	}

	//constructor delegate initialization to init
	RB::WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	// destructor delegate to shutDown
	RB::WindowsWindow::~WindowsWindow()
	{
		shutDown();
	}

	// update include polling events and swap buffers
	void RB::WindowsWindow::onUpdate()
	{
		RB_PROFILE_FUNC();

		glfwPollEvents();
		this->m_ContextManager->swapBuffer();
	}


	// set the vsync
	void RB::WindowsWindow::setVsync(bool enabled)
	{
		RB_PROFILE_FUNC();

		if (enabled)
			// Swap interval is the number of screen updates to wait from the time glfwSwapBuffers was called before 
			// swapping the buffers and returning.
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.Vsync = enabled;
	}

	// check if vsync is enabled
	bool RB::WindowsWindow::isVsync() const
	{
		return m_Data.Vsync;
	}

	// Init all fields, initializing glfw, creating window, setting user pointer
	void RB::WindowsWindow::init(const WindowProps& props)
	{
		RB_PROFILE_FUNC();

		// initialize info fields 
		m_Data.title = props.title;
		m_Data.height = props.height;
		m_Data.width = props.width;

		// creating window
		RB_INFO("Creating Window: {0} ({1} {2})", m_Data.title, m_Data.width, m_Data.height);
		if (!s_GLFWInitialized)
			// if glfw is not initialized, initialize it
		{
			bool success = glfwInit();
			// using assert to check if glfw is initialized, if false, debugbreak
			RB_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(glfwErrorCallBack);
			// set glfw initialized to true
			s_GLFWInitialized = true;
		}

		// create window
		this->m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);

		// set the context to the current window
		this->m_ContextManager = new GLContext(m_Window);
		this->m_ContextManager->init();

		// set the user pointer to the data, so later we can access windows info and EventCallBack
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// set vsync to true on default
		setVsync(true);

		RB_INFO("Window Created!: {0} ({1} {2})", m_Data.title, m_Data.width, m_Data.height);

		{//window event publish
			RB_PROFILE_SCOPE("WindowEventPublish");
			// set window resize callback
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
				{
					WindowInfo& data = *(WindowInfo*)(glfwGetWindowUserPointer(window));
					data.height = height;
					data.width = width;

					WindowResizeEvent event(width, height);
					data.em.value()->enqueue(event);
				});

			// set window close callback
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
				{
					WindowInfo& data = *(WindowInfo*)(glfwGetWindowUserPointer(window));
					WindowCloseEvent event;
					// passing the event to onEvent function in application and let it 
					// handle it
					data.em.value()->enqueue(event);
				});
			// set mouse button callback: pressed, released
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mode)
				{
					WindowInfo& data = *(WindowInfo*)(glfwGetWindowUserPointer(window));
					switch (action)
					{
					case GLFW_PRESS:
					{
						MousePressedEvent event(button);
						data.em.value()->enqueue(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseReleasedEvent event(button);
						data.em.value()->enqueue(event);
						break;
					}
					}
				});

			// set keyboard key callback: pressed, released, repeat for press
			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
				{
					WindowInfo& data = *(WindowInfo*)(glfwGetWindowUserPointer(window));

					switch (action)
					{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.em.value()->enqueue(event);
						break;
					}

					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.em.value()->enqueue(event);
						break;
					}

					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.em.value()->enqueue(event);
						break;
					}
					}
				});

			// set callback for scroll event
			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x_offset, double y_offset)
				{
					WindowInfo& data = *(WindowInfo*)(glfwGetWindowUserPointer(window));
					MouseScrolledEvent event((float)x_offset, (float)y_offset);
					data.em.value()->enqueue(event);
				});

			//set mouse move callback
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
				{
					WindowInfo& data = *(WindowInfo*)(glfwGetWindowUserPointer(window));
					MouseMoveEvent event((float)xPos, (float)yPos);
					data.em.value()->enqueue(event);
				});
		}//window event publish
	}


	void RB::WindowsWindow::shutDown()
	{
		// shut down the window only, keep glfw active for other usages
		glfwDestroyWindow(m_Window);
		delete m_ContextManager;
	}
}
