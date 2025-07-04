#include <pch.h>
#include "ScriptSystem.h"
#include "Rubber/Scene/Utili/Component.h"

namespace Rubber{
	void ScriptSystem::init(entt::registry& registry)
	{
		m_Registry = &registry;
	}

	void ScriptSystem::shutdown()
	{
		m_Registry->view<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc) {
			if (nsc.instance != nullptr){
				nsc.destroyScript(&nsc);
				RB_CORE_ASSERT(nsc.instance == nullptr, "fail to clean up the scripts instance!");
			}
		});
	}

	void ScriptSystem::onUpdate(const float ts)
	{
		m_Registry->view<NativeScriptComponent>().each([ts](auto entity, NativeScriptComponent& nsc) {
			if (nsc.instance == nullptr){
				nsc.instance = nsc.initScript();
				RB_CORE_ASSERT(nsc.instance == nullptr, "failed to init the script!");
				nsc.instance->onCreate();
			}
			nsc.instance->onUpdate(ts);
		});
	}
}


