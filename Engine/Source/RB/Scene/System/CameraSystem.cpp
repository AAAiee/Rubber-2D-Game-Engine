#include <pch.h>
#include "CameraSystem.h"

namespace RB{


	void CameraSystem::onUpdate(const float ts)
	{
		RB_CORE_ASSERT(m_Registry != nullptr, "Call init before doing anything to the system!");
	}

}
