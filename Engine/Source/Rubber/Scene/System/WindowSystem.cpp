#include <pch.h>
#include "Rubber/Scene/System/WindowSystem.h"

#include "Rubber/Event/AppEvent.h"
#include "Rubber/Event/EventManager.h"
#include "Rubber/Event/Event.h"

#include "Rubber/Scene/Utili/Component.h"
#include "Rubber/Core/Application.h"

namespace Rubber {
	void WindowSystem::init(Scene* scene)
	{
		SystemBase::init(scene);

		auto em = Application::getEventManager();

		em->subscribe<ViewPortResizeEvent>("Viewport Resize", [this](const ViewPortResizeEvent& e) {
			return onViewPortResize(e);
			});

		RB_INFO("Window System Init, subscribe to viewport resize");
	}

	void WindowSystem::shutdown()
	{
		SystemBase::shutdown();
		Application::getEventManager()->unsubscribe<ViewPortResizeEvent>("Viewport Resize");
		RB_INFO("Window System shutdown, unsubscribe to viewport resize");
	}


	bool WindowSystem::onViewPortResize(const ViewPortResizeEvent& e)
	{
		// First check if the system is still valid
		if (m_HadInit == false || !m_Registry) {
			RB_WARN("Window System not properly initialized or already shut down");
			return false;
		}
		
		auto view = m_Registry->view<CameraComponent>();
		for (auto& entity : view) {
			CameraComponent& cc = view.get<CameraComponent>(entity);
			if (!cc.isFixedAspectRatio) {

				// making the scene also store the viewport's width and height?
				float newWidth = static_cast<float> (e.getWidth());
				float newHeight = static_cast<float> (e.getHeight());
				cc.camera.onResize(newWidth, newHeight);
				RB_INFO("Camera ViewPort Changed To :{}, {}", newWidth, newHeight);
			}
		}

		return false;
	}

}