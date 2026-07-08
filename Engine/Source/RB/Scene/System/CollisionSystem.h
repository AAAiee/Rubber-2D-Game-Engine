#pragma once
#include <entt.hpp>
#include "RB/Scene/System/System.h"

namespace RB {

	enum class CollisionMethod: uint8_t {
		AABB=0
	};

	class CollisionSystem : public SystemBase {
		
	public:
		void onUpdate(float ts) override;

		bool detectCollision(::entt::entity a, ::entt::entity b, CollisionMethod method = CollisionMethod::AABB);
	};



}

