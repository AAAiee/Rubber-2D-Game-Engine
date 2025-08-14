#include <pch.h>
#include "Rubber/Scene/System/WindowSystem.h"

#include "Rubber/Event/AppEvent.h"
#include "Rubber/Event/EventManager.h"
#include "Rubber/Event/Event.h"

#include "Rubber/Scene/Utili/Component.h"
#include "Rubber/Core/Application.h"

namespace Rubber {
	void WindowSystem::init(Ref<Scene> scene)
	{
		SystemBase::init(scene);

		auto em = Application::getEventManager();

		em->subscribe<ViewPortResizeEvent>("Viewport Resize", [this](const ViewPortResizeEvent& e) {
			return onViewPortResize(e);
			});
	}

	void WindowSystem::shutdown()
	{
		SystemBase::shutdown();
		Application::getEventManager()->unsubscribe<ViewPortResizeEvent>("Viewport Resize");
	}


	bool WindowSystem::onViewPortResize(const ViewPortResizeEvent& e)
	{
		// passing down to camera (maybe use a flag? ) ;
		auto view = m_Registry->view<CameraComponent>();
		for (auto& entity : view) {
			CameraComponent& cc = view.get<CameraComponent>(entity);
			if (!cc.isFixedAspectRatio) {
				float newWidth = static_cast<float> (e.getWidth());
				float newHeight = static_cast<float> (e.getHeight());
				cc.camera.onResize(newWidth, newHeight);
				RB_INFO("Camera ViewPort Changed To :{}, {}", newWidth, newHeight);
			}
		}

		return false;
	}

}