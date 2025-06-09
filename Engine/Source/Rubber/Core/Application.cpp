#include <pch.h>
#include "Rubber/Core/Application.h"

#include "Rubber/Layer/Layer.h"

#include "Rubber/Event/AppEvent.h"
#include "Rubber/Event/KeyEvent.h"
#include "Rubber/Event/MouseEvent.h"

#include "Platform/Windows/Windowswindow.h"

#include "Rubber/Input/Input.h"
#include "Rubber/Renderer/BufferLayout.h"
#include "Rubber/Renderer/VertexArray.h"
#include "Rubber/Renderer/Renderer.h"

#include "Rubber/Renderer/Camera.h"
#include "Rubber/Input/KeyCodes.h"

#include "glad/glad.h"

// bind event callback  
namespace Rubber
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
		:m_Timer(144)
	{
		RB_CORE_ASSERT(!s_Instance, "Applicaiton instance has already been constructed");
		this->s_Instance = this;

		// Create a window when an application instance is created
		this->m_Window = Window::create();

		// When a window event happens, the callbacks automatically
		// passed event happening to onEvent
		this->m_Window->setEventCallBack(
			[this](Event& e) {
				this->onEvent(e);
			});

		// push ImGui to be the last layer (rendered last )
		this->m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer:     " << renderer << std::endl;

		this->m_Window->setVsync(false);

		Renderer::init();
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
		//RB_INFO("{0} ,from rubber", e.toString());

		// use a dispatcher to store the event and handle it 
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->onWindowClose(e);});

		dispatcher.dispatch<KeyPressedEvent>([this](const KeyPressedEvent& e) {
			if (e.getKeyCode() == RB_KEY_ESCAPE) {
				WindowCloseEvent windowClose = WindowCloseEvent();
				return this->onWindowClose(windowClose);
			}
			return false;
			});

		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return this->onWindowResize(e); });

		// reversely loop through the layer stack and handle event
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
		double lag = 0.0;
		while (m_Runing)
		{
			this->m_Timer.startFrame();

			m_Window->onUpdate();

			lag += this->m_Timer.getDeltaTime();
			//RB_ERROR("DELTA TIME =  {}", m_Timer.getDeltaTime());
			//RB_ERROR("LAG NOW IS : {}", lag);

			if (!this->m_IsWindowMinimized) {
				//update all layers;
				while (lag >= 1 / 120.0) {
					for (Layer* layer : m_LayerStack)
					{
						layer->onUpdate();
					}
					lag -= (1.0 / 120.0);
				}
			}

			m_ImGuiLayer->begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->onImGuiRender();
			}
			m_ImGuiLayer->end();

			double avgFrameTime = this->m_Timer.getAverageFrameTime();
			//RB_TRACE("Current frame rate: {} FPS", this->m_Timer.getFps());
			//RB_TRACE("Average frame time: {:.5f} ms", avgFrameTime);
			//RB_TRACE("Approx. avg FPS: {:.2f}", 1.0 / avgFrameTime);
			//
			m_Timer.waitForFrameEnd();
		}
	}

	// this is what we want happen with a window close event
	bool Application::onWindowClose( WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
	
	// this is what we want happen with a window resize event
	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		int resizedToWidth = e.getWidth();
		int resizedToHeight = e.getHeight();
		if (resizedToWidth == 0 || resizedToHeight == 0){
			this->m_IsWindowMinimized = true;

			//make sure this will be propagated to other layers
			return false;
		}

		//resize the viewport to match the resized window size
		Renderer::onWindowResize(resizedToWidth,resizedToHeight);
		return false;
		
	}
}
