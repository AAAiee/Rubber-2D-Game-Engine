#pragma  once 
#include <entt.hpp>
#include<Rubber/Scene/Utili/Component.h>

namespace Rubber{

	struct CameraSystem{
		CameraSystem() = default;
		using CameraGroup =
			decltype(std::declval<entt::registry&>().group<NativeScriptComponent, CameraComponent>());

		void init(entt::registry& registry);
		void shutdown();
		void onUpdate(const float ts); 

		CameraGroup m_Group;
		entt::registry* m_Registry = nullptr;
	};
}