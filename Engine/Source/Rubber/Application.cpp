#include <pch.h>
#include "Application.h"

#include "Rubber/Layer.h"

#include "Rubber/Event/AppEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include "Platform/Windows/Windowswindow.h"

#include <glad/glad.h>
#include "Rubber/Input/Input.h"

// bind event callback  
namespace Rubber
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		RB_CORE_ASSERT(!s_Instance, "Applicaiton instance has already been constructed");
		s_Instance = this;

		// Create a window when an application instance is created
		m_Window = Window::create();
		
		// When a window event happens, the eventcallbackfn automatically
		// passed event happening to onEvent
		m_Window->setEventCallBack(
			[this](Event& e) {
				this->onEvent(e);
			});
	}
	Application::~Application()
	{

	}

	Window& Application::getWindow()
	{
		return s_Instance->getWindowImpl();
	}


	void Application::onEvent(Event& e)
	{
		RB_INFO("{0}", e.toString());

		// use a dispatcher to store the event and handle it 
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->onWindowClose(e);});
		
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

	//delegate the task to m_LayerStack, also immediately attach the layer
	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}


	// delegate the task to m_LayerStack, also immediately attach the layer
	void Application::pushOverlay(Layer* layer)
	{
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
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

			// simple example to show input now working.
			auto [x, y ] = Input::getMousePosition();
			RB_WARN("Mouse Position: {0} {1}", x, y);
			auto pressed = Input::isKeyPressed(GLFW_KEY_W);
			auto mousePressed = Input::isMouseButtonPressed(2);
			RB_WARN("Mouse Position: {0} {1}", x, y);
			RB_WARN("Key O is pressed? : {0}", pressed);
			RB_WARN("Mouse Button 1 is pressed? : {0}", mousePressed);
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
