#include <pch.h>
#include <Rubber/Scene/Utili/Entity.h>
#include "Rubber/Scene/System/RenderSystem.h"
#include "Rubber/Renderer/Renderer2D.h"


namespace Rubber{

	void RendererSystem::init(entt::registry& registry)
	{
		 m_Registry = &registry;
	     m_Group = registry.group<TransformComponent, SpriteComponent>();
	}

	void RendererSystem::shutdown()
	{

	}

	void RendererSystem::onUpdate(const float ts)
	{
		const glm::mat4* projectionMatrix;
		glm::mat4 viewMatrix;
		{// find the main camera and calculate VP matrix
			auto view = (*m_Registry).view<CameraComponent, TransformComponent, PrimaryCameraTag>();
			entt::entity primaryCameraEntityHandle = view.front();
			if (primaryCameraEntityHandle != entt::null) {
				const CameraComponent& cameraComponent = view.get<const CameraComponent>(primaryCameraEntityHandle);
				const TransformComponent cameraTransform = view.get<const TransformComponent>(primaryCameraEntityHandle);
				projectionMatrix = &(cameraComponent.camera.getProjection());
				viewMatrix = glm::inverse(cameraTransform.transform);
			}
		}// find the main camera;

		glm::mat4 vpMatrix = viewMatrix * *projectionMatrix;

		// rendering
		Renderer2D::beginScene(vpMatrix);
		{
			for (auto entity : m_Group)
			{
				auto& transform = m_Group.get<TransformComponent>(entity);
				auto& sprite = m_Group.get<SpriteComponent>(entity);
				Renderer2D::drawColorQuad(transform.transform, sprite.color);
			}
		}
		Renderer2D::endScene();
	}
}