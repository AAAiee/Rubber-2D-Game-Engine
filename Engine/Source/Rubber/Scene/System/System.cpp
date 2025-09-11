#include <pch.h>
#include "System.h"
#include "Rubber/Scene/Scene.h"

namespace Rubber {

	void SystemBase::init(Scene* scene)
	{
		if (!m_HadInit) {
			m_HadInit = true;
		}
		else {
			RB_CORE_ASSERT(false, "Do not init twice!");
		}

		RB_CORE_ASSERT(scene, "Scene ptr is null!");
		m_ScenePtr = scene; 
		m_Registry = &m_ScenePtr->getRegistry();
	}

	void SystemBase::shutdown()
	{
		m_HadInit = false;
	}

	void SystemBase::onUpdate(float ts)
	{

	}


}


