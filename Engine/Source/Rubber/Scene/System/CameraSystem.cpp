#include <pch.h>
#include "CameraSystem.h"

namespace Rubber{
	void CameraSystem::init(entt::registry& registry)
	{
		m_Group = registry.group<NativeScriptComponent, CameraComponent>();
		m_Registry = &registry;
	}

	void CameraSystem::shutdown()
	{

	}


	void CameraSystem::onUpdate(const float ts)
	{
		RB_CORE_ASSERT(m_Registry != nullptr, "Call init before doing anything to the system!");
		
	}

}
