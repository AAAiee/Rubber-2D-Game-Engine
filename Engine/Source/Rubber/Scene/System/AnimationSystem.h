#pragma once
#include <entt.hpp>
#include "Rubber/Scene/System/System.h"


namespace Rubber {
	class AnimaitonSystem :public SystemBase {
	public:
		void init(Scene* scene) override;

		void onUpdate(float ts) override;
	};
}

