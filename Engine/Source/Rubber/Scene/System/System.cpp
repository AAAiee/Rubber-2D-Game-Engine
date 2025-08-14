#include <pch.h>
#include "System.h"
#include "Rubber/Scene/Scene.h"

namespace Rubber {

	void SystemBase::init(Ref<Scene> scene)
	{
		if (!m_HadInit) {
			m_HadInit = true;
		}
		else {
			RB_CORE_ASSERT(false, "Do not init twice!");
		}

		m_ScenePtr = scene; 
		m_Registry = &m_ScenePtr->getRegistry();
	}

	void SystemBase::shutdown()
	{
		RB_CORE_ASSERT(m_HadInit, "The system is not initialized yet!");
	}

	void SystemBase::onUpdate(float ts)
	{
		RB_CORE_ASSERT(m_HadInit, "The system is not initialized yet!");
	}

}


