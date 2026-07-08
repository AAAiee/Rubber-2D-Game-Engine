#include <pch.h>
#include "ScriptSystem.h"
#include "RB/Scene/Utili/Component.h"

namespace RB{

	void ScriptSystem::shutdown()
	{
		SystemBase::shutdown();
		m_Registry->view<NativeScriptComponent>().each([](auto entity, NativeScriptComponent& nsc) {
			if (nsc.instance != nullptr){
				nsc.destroyScript(&nsc);
				RB_CORE_ASSERT(nsc.instance == nullptr, "fail to clean up the scripts instance!");
			}
		});
	}

	void ScriptSystem::onUpdate(const float ts)
	{
		SystemBase::onUpdate(ts);
		RB_CORE_ASSERT(m_Registry, " set registry first");

		m_Registry->view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
			if (nsc.instance == nullptr){
				nsc.instance = nsc.initScript();

				nsc.instance->m_Entity = { entity,m_ScenePtr };

				RB_CORE_ASSERT(nsc.instance != nullptr, "failed to init the script!");
				nsc.instance->onCreate();
			}
			nsc.instance->onUpdate(ts);
		});
	}
}


