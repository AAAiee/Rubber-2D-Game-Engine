#include <pch.h>
#include "Rubber/Window/Window.h"

#include <Rubber/Scene/Utili/Entity.h>
#include "Rubber/Scene/System/RenderSystem.h"

#include "Rubber/Renderer/Renderer2D.h"
#include "Rubber/Renderer/RendererCommand.h"
#include "Rubber/Renderer/Subtexture2D.h"
#include "Rubber/Scene/Utili/Component.h"

#include "Rubber/Core/Application.h"
#include "Rubber/Resources/Asset.h"
#include "Rubber/Resources/AssetManager.h"
#include "Rubber/Resources/AssetMetaData.h"
#include "Rubber/Resources/AnimationResources/AnimationAtlas.h"
#include "Rubber/Resources/AnimationResources/AnimationSpriteSheet.h"

namespace Rubber{

	void RendererSystem::onUpdate(float ts)
	{
		glm::mat4* projectionMatrix = nullptr;
		glm::mat4 viewMatrix;

		{// find the main camera and calculate the VP matrix
			auto view = (*m_Registry).view<CameraComponent, TransformComponent>();
			for (auto entity :view) {
				auto& cc = view.get<CameraComponent>(entity);

				if (!cc.isPrimary) continue;

				auto& tsC = view.get<TransformComponent>(entity);
				projectionMatrix = &(cc.camera.getProjection());

				viewMatrix = glm::inverse(tsC.getTransformMatrix());
			}
		}// find the main camera;

		if (projectionMatrix == nullptr) return; 

		glm::mat4 vpMatrix = (*projectionMatrix) * viewMatrix;
		//Do a sorting based on z-value to ensure correctly drawings on screen based on depth
		m_Registry->sort<SpriteComponent>(
			[&](entt::entity lhs, entt::entity rhs) {
				const auto& tL = m_Registry->get<TransformComponent>(lhs);
				const auto& tR = m_Registry->get<TransformComponent>(rhs);
				return tL.position.z < tR.position.z;
			}
		);

		RendererCommand::clear();
		RendererCommand::setPolygonMode(RendererPolygonMode::FILL);
		// drawing all entities
		Renderer2D::beginScene(vpMatrix);
		{
			auto view = m_Registry->view<SpriteComponent, TransformComponent, VisibilityControlComponent>();
			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);
				auto& sprite = view.get<SpriteComponent>(entity);
				auto& visibilityCC = view.get<VisibilityControlComponent>(entity);

				if (!visibilityCC.isVisible) continue; // if no texture / visibility level is false, renderer nothing
				glm::vec3 posAfterOffset{};
				posAfterOffset.x = transform.position.x + sprite.localOffset.x;
				posAfterOffset.y = transform.position.y + sprite.localOffset.y;
				posAfterOffset.z = transform.position.z;

				glm::mat4 tsMatrix = glm::scale(
					glm::rotate(glm::translate(glm::mat4(1.0f), posAfterOffset), transform.rotation, glm::vec3(0.0f, 0.0f, 1.0f)),
					glm::vec3(transform.scale.x, transform.scale.y, 1.0f));

				if (sprite.texture == nullptr) {
					Renderer2D::drawColorQuad(tsMatrix, sprite.color);
				}
				else {
					Renderer2D::drawTextureQuad(tsMatrix, sprite.texture->getSpriteSheetTexture(), sprite.texture->getTexCoord(), 1.0f, sprite.color);
				}
			}
		}
		Renderer2D::endScene();

		RendererCommand::setPolygonMode(RendererPolygonMode::LINE);
		// draw all collision boxes
		Renderer2D::beginScene(vpMatrix);
		{
			auto view = m_Registry->view<SpriteComponent, TransformComponent, CollisionComponent, CollisionBoxOnRenderFlag>();
			for (auto e : view) {

				CollisionBoxOnRenderFlag& flag = view.get<CollisionBoxOnRenderFlag>(e);
				CollisionComponent& collisionC = view.get<CollisionComponent>(e);
				if (!flag.onDebugRender)
					continue;

				auto& spriteC = view.get<SpriteComponent>(e);

				glm::vec3 posAfterOffset{};
				posAfterOffset.x = collisionC.position.x;
				posAfterOffset.y = collisionC.position.y;
				posAfterOffset.z = 0.0f;

				glm::mat4 tsMatrix = glm::scale(
					glm::rotate(glm::translate(glm::mat4(1.0f), posAfterOffset), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f)),
					glm::vec3(collisionC.size.x, collisionC.size.y, 1.0f));

				const glm::vec4 green = { 0.0f, 1.0f, 0.0f ,1.0f };
				const glm::vec4 red = { 1.0f, 0.0f ,0.0f, 1.0f };
				const glm::vec4 blue = { 0.0f, 0.0f, 1.0f, 1.0f };

				if (collisionC.enabled && !flag.renderCollision) {
					Renderer2D::drawColorQuad(tsMatrix, green);
				}
				else if (flag.renderCollision) {
					Renderer2D::drawColorQuad(tsMatrix, red);
					// consume
					flag.renderCollision = false;
				}
				else {
					Renderer2D::drawColorQuad(tsMatrix, blue);
				}
			}
		}
		Renderer2D::endScene();
	}

	void RendererSystem::init(Ref<Scene> scene)
	{
		SystemBase::init(scene);

		FrameBufferSpecification spec;

		spec.m_Width = Application::getWindow().getWidth();
		spec.m_Height = Application::getWindow().getWidth();

		m_WorldFBO->create(spec); 
	}

}