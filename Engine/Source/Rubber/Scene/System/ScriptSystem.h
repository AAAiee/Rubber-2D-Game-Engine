#pragma once
#include <entt.hpp>
#include <Rubber/Scene/System/System.h>


namespace Rubber{
	class ScriptSystem : public SystemBase {
	public:
		ScriptSystem() = default;

		void shutdown() override;
		void onUpdate(const float ts) override;
	};



}
