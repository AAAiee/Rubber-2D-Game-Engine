#include <pch.h>
#include "MoveSystem.h"

#include "RB/Scene/Utili/Component.h"


namespace RB {


	void MoveSystem::onUpdate(float ts)
	{
		auto view = m_Registry->view<TransformComponent, MoveComponent >();
		for (auto e : view) {

			auto& tsC = view.get<TransformComponent>(e);
			auto& moveC = view.get<MoveComponent>(e);

			if (!moveC.moveEnabled) continue;

			if (moveC.enableGravity) {
				moveC.velocity.y += moveC.gravity * ts;
			}

			tsC.position.x += moveC.velocity.x * ts;
			tsC.position.y += moveC.velocity.y * ts; 

			
		}
	}

}

