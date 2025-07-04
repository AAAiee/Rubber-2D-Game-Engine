#pragma once
#include <entt.hpp>

namespace Rubber{
	struct ScriptSystem {
		ScriptSystem() = default;
		void init(entt::registry& registry);
		void shutdown();
		void onUpdate(const float ts);

		entt::registry* m_Registry = nullptr; 
	};



}
