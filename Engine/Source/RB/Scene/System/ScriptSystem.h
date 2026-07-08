#pragma once
#include <entt.hpp>
#include <RB/Scene/System/System.h>


namespace RB{
	class ScriptSystem : public SystemBase {
	public:
		ScriptSystem() = default;

		void shutdown() override;
		void onUpdate(const float ts) override;
	};



}
