#include <pch.h>
#include <glm/glm.hpp>
#include "InputSystem.h"
#include"Rubber/Scene/Scene.h"
#include "Rubber/Window/Window.h"
#include "Rubber/Core/Application.h"
#include "Rubber/Event/KeyEvent.h"
#include "Rubber/Event/MouseEvent.h"
#include "Rubber/Event/EventManager.h"
#include "Rubber/Scene/Utili/Component.h"


namespace Rubber {
	void InputSystem::init(Scene* scene)
	{
		SystemBase::init(scene);

		auto em = Application::getEventManager();
		em->subscribe<KeyPressedEvent>("Game_KeyDown", [this](const KeyPressedEvent& event) {
				m_KeysDown[event.getKeyCode()] = true;
				return true;
			});
		
		em->subscribe<MousePressedEvent>("Game_MouseClicked", [this](const MousePressedEvent& event) {
				m_KeysDown[event.getButton()] = true;
				return true;
			});


		em->subscribe<KeyReleasedEvent>("Game_KeyReleased", [this](const KeyReleasedEvent& event) {
				m_KeysDown[event.getKeyCode()] = false;
				return true;
			});

		em->subscribe<MouseReleasedEvent>("Game_MouseReleased", [this](const MouseReleasedEvent& event) {
				m_KeysDown[event.getButton()] = false;
				return true;
			});

		em->subscribe<MouseMoveEvent>("Game_MouseMoved", [this](const MouseMoveEvent& event) {
				m_MouseX = event.getX();
				m_MouseY = event.getY();
				return true;
			});
	}

	void InputSystem::shutdown()
	{
		auto em = Application::getEventManager();
		em->unsubscribe<KeyPressedEvent>("Game_KeyDown");
		em->unsubscribe <MousePressedEvent>("Game_MouseClicked");
		em->unsubscribe <MouseReleasedEvent>("Game_MouseReleased");
		em->unsubscribe <KeyReleasedEvent>("Game_KeyReleased");
		em->unsubscribe <MouseMoveEvent>("Game_MouseMoved");
	}

	void InputSystem::onUpdate( float ts)
	{
		SystemBase::onUpdate(ts);

		auto view = m_Registry->view<InputComponent>();

		for (auto e : view) {
			auto& ic = view.get<InputComponent>(e);

			// update latest mouse position
			ic.mouseX = m_MouseX;
			ic.mouseY = m_MouseY;

			glm::vec2 mouseWorldPos = mousePosToWorldCoord( m_MouseX, m_MouseY);
			ic.mouseXWorld = mouseWorldPos.x;
			ic.mouseYWorld = mouseWorldPos.y;

			// update latest key/mouse pressed state
			ic.actionState.clear();
			for (auto& it : ic.bindings) {
				ic.actionState[it.name] = m_KeysDown[it.keyCode];
			}
		}
	}


	glm::vec2 InputSystem::mousePosToWorldCoord( float mouseX, float mouseY)
	{
		auto view = m_Registry->view<CameraComponent>();

		entt::entity primaryCameraEntity = entt::null;

		for (auto entity : view) {
			auto& cc = view.get<CameraComponent>(entity);
			if (cc.isPrimary) primaryCameraEntity = entity;
		}

		RB_CORE_ASSERT(primaryCameraEntity != entt::null, "No primary camera exists");

		auto& cC = m_Registry->get<CameraComponent>(primaryCameraEntity);
		auto& cameraTsC = m_Registry->get<TransformComponent>(primaryCameraEntity);

		const glm::mat4& projectionMatrix = cC.camera.getProjection();
		glm::mat4 viewMatrix = glm::inverse(cameraTsC.getTransformMatrix());

		glm::mat4 unprojectMatrix = glm::inverse(viewMatrix * projectionMatrix);

		glm::vec4 ndp;
		Window& window = Application::getWindow();
		ndp.x = (mouseX / window.getWidth()) * 2.0f - 1.0f;
		ndp.y = 1.0f - 2.0f * (mouseY / window.getHeight());
		ndp.z = 0.0f;
		ndp.w = 1.0f;

		// don't really need this for 2d but did it whatsoever
		glm::vec4 mousePosInWorld = unprojectMatrix * ndp;
		return { mousePosInWorld.x, mousePosInWorld.y };
	}

}

