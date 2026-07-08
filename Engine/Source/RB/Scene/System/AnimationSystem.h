#pragma once
#include <entt.hpp>
#include "RB/Scene/System/System.h"


namespace RB {
	class AnimaitonSystem :public SystemBase {
	public:
		void init(Ref<Scene> scene) override;

		void onUpdate(float ts) override;
	};
}

