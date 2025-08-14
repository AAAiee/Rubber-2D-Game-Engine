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
#include "Rubber/Utility/Timer/GameLoopTimer.h"

//event manager
#include "Rubber/Event/EventManager.h"


#include "Rubber/Utility/Utility.h"
#include "Rubber/Resources/AssetManager.h"


namespace Rubber
{
	Application* Application::s_Instance = nullptr;
	const float Application::FIXED_TIME_STAMP = 1.0f / 120.0f;

	Application::Application(const std::string_view name)
	{
		RB_PROFILE_FUNC();
		  
		RB_CORE_ASSERT(!s_Instance, "Applicaiton instance has already been constructed");
		this->s_Instance = this;

		// Create a window when an application instance is created
		{
			RB_PROFILE_SCOPE("WindowCreate");
			this->m_Window = Window::create(WindowProps(name));
		}

		//TODO:: 128? magic number
		this->m_Em = EventManager::create(128);
		this->m_Window->setEventManager(m_Em);

		//IMGUI is the last layer, so it updates after everything else (e.g. always rendered on top)
		this->m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	
		{ 
			RB_PROFILE_SCOPE("Renderer Init + Random Engine Init");
			Renderer::init();
			RandomEngine::init();
		}

		// Vsync off, use gameLoopTimer to regulate flow
		this->m_Window->setVsync(false);
		this->m_Timer = makeScope<GameLoopTimer>(60);

		m_AssetManager = makeRef<AssetManager>();

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

	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		m_LayerStack.pushOverlay(layer);
	}

	void Application::run()
	{

		//TODO:: This requires all layers to be present at the beginning, which might not be true
		if (m_FirstRun) {
			attachAll();
			m_FirstRun = false;
		}
	
		double lag = 0.0;
		while (m_Runing)
		{
			RB_PROFILE_SCOPE("Application::Running");

			{// start the timer
				RB_PROFILE_SCOPE("startFrame");
				this->m_Timer->startFrame();
			}
			
			{// window update
				RB_PROFILE_SCOPE("FrameWindowUpdate");
				m_Window->onUpdate();
			}// window update

			{//Update all layer in order
				RB_PROFILE_SCOPE("Frame: OnUpdate");
				lag += this->m_Timer->getDeltaTime();
				if (!this->m_IsWindowMinimized) {
					//update all layers;
					while (lag >= FIXED_TIME_STAMP){
						for (Layer* layer : m_LayerStack)
						{
							layer->onUpdate(FIXED_TIME_STAMP);
						}
						lag -= FIXED_TIME_STAMP;
					}
				}
			}

			{// Flush all events held in the queue
				RB_PROFILE_SCOPE("onEvent");
			    this->m_Em->flush();
			}// 

			{// Begin:: On IMGUI Rendering
				RB_PROFILE_SCOPE("OnImGuiRendering");
				m_ImGuiLayer->begin();
				for (Layer* layer : m_LayerStack)
				{
					layer->onImGuiRender();
				}
				m_ImGuiLayer->end();
			}// 

			{// Wait if still has time left
				RB_PROFILE_SCOPE("WaitFrametoEnd");
				m_Timer->waitForFrameEnd();
			}
		}
	}


	// Window close callback
	bool Application::onWindowClose( const WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
	
	//Window resize callback
	bool Application::onWindowResize(const WindowResizeEvent& e)
	{
		RB_PROFILE_FUNC();
		int resizedToWidth = e.getWidth();
		int resizedToHeight = e.getHeight();
		if (resizedToWidth == 0 || resizedToHeight == 0){
			this->m_IsWindowMinimized = true;

			//make sure the resize event will be propagated to other callback as well
			return false;
		}

		//resize the viewport to match the resized window size
		Renderer::onWindowResize(resizedToWidth,resizedToHeight);
		return false;
	}


	//Temp::This is quick hack to make current setting work, requires all layers to be present at the time of running
	// this might not be true
	void Application::attachAll()
	{
		RB_PROFILE_FUNC();
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*(--it))->onAttach();
		}
	}
}
