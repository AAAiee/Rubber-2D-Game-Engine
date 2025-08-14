#include "GroundLandingSystem.h"
#include "entt.hpp"
#include <ECS\GameComponents\GameComponents.h>

void GroundLandingSystem::onUpdate(float ts)
{
	auto view = m_Registry->view<GroundComponent, RB::TransformComponent, RB::MoveComponent>();
	for (auto e : view) {

		auto& gC = view.get<GroundComponent>(e);
		auto& mvC = view.get<RB::MoveComponent>(e);
		auto& tsC = view.get<RB::TransformComponent>(e);

		if (tsC.position.y <= gC.groundY) {
			tsC.position.y = gC.groundY;
			mvC.velocity.y = 0.0f;
			gC.onGround = true;
		}
		else {
			gC.onGround = false;
		}

		if (tsC.position.x <= gC.leftBound || tsC.position.x >= gC.rightBound) {
			tsC.position.x -= (tsC.position.x >= 0.0f) ? 0.01f : -0.01f;
			mvC.velocity.x = 0.0f;
		}
	}
}



