#include <pch.h>
#include "Application.h"

#include "Rubber/Layer.h"

#include "Rubber/Event/AppEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include "Platform/Windowswindow.h"

#include <glad/glad.h>

// bind event callback  
#define BIND_EVENT_FN(x)  std::bind(&Application::x, this, std::placeholders::_1)

namespace Rubber
{
	Application::Application()
	{
		// create a window when an application instance is created
		m_Window = Window::create();
		m_Window->setEventCallBack(BIND_EVENT_FN(onEvent));

	}
	Application::~Application()
	{

	}

	void Application::onEvent(Event& e)
	{
		// when a window event happens, the eventcallbackfn automatically 
		// passed event happening to here
		RB_INFO("{0}", e.toString());
		// use a dispatcher to store the event and handle it 
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		
		// reversely loop through the layerstack and handle event
		// if a event is handled from the top layer, stop propagation
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->onEvent(e);
			if (e.isHandled())
			{
				break;
			}
		}
	}

	//delegate the task to m_LayerStack
	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.pushLayer(layer);
	}


	// delegate the task to m_LayerStack
	void Application::pushOverlay(Layer* layer)
	{
		m_LayerStack.pushOverlay(layer);
	}

	void Application::run()
	{
		while (m_Runing)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			//update all layers;
			for (Layer* layer : m_LayerStack)
			{
				layer->onUpdate();
			}

			// update the window
			m_Window->onUpdate();
		}
	}

	// this is what we want happen with a window close event
	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
		
	
}
