#include <pch.h>
#include "Rubber/Core/Application.h"

//layer
#include "Rubber/Layer/Layer.h"
#include "Rubber/imGui/ImGuiLayer.h"

//event
#include "Rubber/Event/AppEvent.h"
#include "Rubber/Event/KeyEvent.h"
#include "Rubber/Event/MouseEvent.h"

// window
#include "Platform/Windows/Windowswindow.h"

// input
#include "Rubber/Input/Input.h"
#include "Rubber/Input/KeyCodes.h"

//renderer
#include "Rubber/Renderer/Renderer.h"

//timer
#include "Rubber/Timer/Timer.h"

//event manager
#include "Rubber/Event/EventManager.h"


#include "Rubber/Utility/Utility.h"



// bind event callback  
namespace Rubber
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		RB_PROFILE_FUNC();
		  
		RB_CORE_ASSERT(!s_Instance, "Applicaiton instance has already been constructed");
		this->s_Instance = this;

		// Create a window when an application instance is created
		{
			RB_PROFILE_SCOPE("WindowCreate");
			this->m_Window = Window::create();
		}
		this->m_Em = makeRef<EventManager>();
		this->m_Window->setEventManager(m_Em);

		// push ImGui to be the last layer (rendered last )
		this->m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	
		{
			RB_PROFILE_SCOPE("Renderer Init + Random Engine Init");
			Renderer::init();
			RandomEngine::init();
		}

		this->m_Window->setVsync(false);
		this->m_Timer = makeScope<Timer>(60);

		//event subscription
		{
			RB_PROFILE_SCOPE("Application Event Subs");
			this->m_Em->subscribe<WindowCloseEvent>("Application WindowClose", [this](const WindowCloseEvent& e) { return this->onWindowClose(e); });
			this->m_Em->subscribe<WindowResizeEvent>("Application WindowResize", [this](const WindowResizeEvent& e) { return this->onWindowResize(e); });
			this->m_Em->subscribe<KeyPressedEvent>("Application KeyPressed", [this](const KeyPressedEvent& e) {
				if (e.getKeyCode() == RB_KEY_ESCAPE) {
					WindowCloseEvent windowClose = WindowCloseEvent();
					return this->onWindowClose(windowClose);
				}
				return false;
				});
		}
	}

	Application::~Application()
	{
		//TODO:: shut down all subsystems

	}

	Window& Application::getWindow()
	{
		return s_Instance->getWindowImpl();
	}

	//delegate the task to m_LayerStack, also immediately attach the layer
	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.pushLayer(layer);
	}


	// delegate the task to m_LayerStack, also immediately attach the layer
	void Application::pushOverlay(Layer* layer)
	{
		m_LayerStack.pushOverlay(layer);
	}

	void Application::run()
	{
		if (m_FirstRun) {
			attachAll();
			m_FirstRun = false;
		}
	
		double lag = 0.0;
		while (m_Runing)
		{
			
			RB_PROFILE_SCOPE("Application::Running");

			{// startFrame
				RB_PROFILE_SCOPE("startFrame");
				this->m_Timer->startFrame();
			}// startFrame
			
			{// window update
				RB_PROFILE_SCOPE("FrameWindowUpdate");
				m_Window->onUpdate();
			}// window update


			{// logic update ++ rendering
				RB_PROFILE_SCOPE("Frame: OnUpdate");
				lag += this->m_Timer->getDeltaTime();

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
			}// logic update + rendering

			{// onEvent
				RB_PROFILE_SCOPE("onEvent");
			    this->m_Em->flush();
			}// onEvent

			{// on IMGUI RENDERING
				RB_PROFILE_SCOPE("OnImGuiRendering");
				m_ImGuiLayer->begin();
				for (Layer* layer : m_LayerStack)
				{
					layer->onImGuiRender();
				}
				m_ImGuiLayer->end();
			}// on IMGUI RENDERING

			{// wait 
				RB_PROFILE_SCOPE("WaitFrametoEnd");
				m_Timer->waitForFrameEnd();
			}// wait
		}
	}

	// this is what we want happen with a window close event
	bool Application::onWindowClose( const WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
	
	// this is what we want happen with a window resize event
	bool Application::onWindowResize(const WindowResizeEvent& e)
	{
		RB_PROFILE_FUNC();
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

	void Application::attachAll()
	{
		RB_PROFILE_FUNC();
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*(--it))->onAttach(m_Em);
		}
	}
}
