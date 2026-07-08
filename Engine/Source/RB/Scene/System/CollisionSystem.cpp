#include <pch.h>

#include "CollisionSystem.h"
#include "RB/Renderer/Renderer2D.h"
#include "RB/Renderer/RendererCommand.h"

#include "RB/Scene/Utili/Component.h"


namespace RB {
	static void collisionBoxUpdate(::entt::registry& reg, ::entt::entity ent);

	void CollisionSystem::onUpdate(float ts)
	{

		auto view = m_Registry->view<CollisionComponent,SpriteComponent, TransformComponent>();

		for (auto colliderSrc : view) {
			auto& coliderSrcCC = view.get<CollisionComponent>(colliderSrc);
			collisionBoxUpdate(*m_Registry, colliderSrc);
			if (!coliderSrcCC.enabled || coliderSrcCC.targetLayer == CollisionLayer::None) 
				continue;

			for (auto colliderDst : view) {

				auto& colliderDstCC = view.get<CollisionComponent>(colliderDst);
				if (colliderSrc == colliderDst || !colliderDstCC.enabled
					|| (colliderDstCC.srcLayer != coliderSrcCC.targetLayer))
					continue;

				bool collided = detectCollision(colliderSrc, colliderDst);

				if (collided && colliderDstCC.onCollision) {
					colliderDstCC.onCollision({colliderDst, m_ScenePtr});
					auto& flagSrc = m_Registry->get<CollisionBoxOnRenderFlag>(colliderSrc); 
					auto& flagDst = m_Registry->get<CollisionBoxOnRenderFlag>(colliderDst); 

					flagSrc.renderCollision = true;
					flagDst.renderCollision = true;
				}
			}
		}
	}

	bool CollisionSystem::detectCollision(entt::entity a, entt::entity b, CollisionMethod method /*= CollisionMethod::AABB*/)
	{

		RB_CORE_ASSERT(method == CollisionMethod::AABB, "Not supported method!");

		auto& aCC = m_Registry->get<CollisionComponent>(a);
		auto& bCC = m_Registry->get<CollisionComponent>(b);

		auto& aTsc = m_Registry->get<TransformComponent>(a);
		auto& bTsc = m_Registry->get<TransformComponent>(b);

		glm::vec3 aPos = aTsc.position;
		glm::vec3 bPos = bTsc.position;

		float aMaxX = aTsc.position.x + (aCC.size.x * 0.5f);
		float aMinX = aTsc.position.x - (aCC.size.x * 0.5f);
		float aMaxY = aTsc.position.y + (aCC.size.y * 0.5f);
		float aMinY = aTsc.position.y - (aCC.size.y * 0.5f);

		float bMaxX = bTsc.position.x + (bCC.size.x * 0.5f);
		float bMinX = bTsc.position.x - (bCC.size.x * 0.5f);
		float bMaxY = bTsc.position.y + (bCC.size.y * 0.5f);
		float bMinY = bTsc.position.y - (bCC.size.y * 0.5f);

		bool notOverlap = aMaxX < bMinX || aMinX > bMaxX || aMaxY < bMinY || aMinY > bMaxY;

		return (notOverlap) ? false : true;
	}

	static void collisionBoxUpdate(::entt::registry& reg, ::entt::entity ent)
	{
		auto& CC = reg.get<CollisionComponent>(ent);
		auto& tsC = reg.get<TransformComponent>(ent);
		auto& spC = reg.get<SpriteComponent>(ent);

		// get the center of the collision box ( center of the sprite)
		CC.position.x = tsC.position.x + spC.localOffset.x;
		CC.position.y = tsC.position.y + spC.localOffset.y;
	}

}

