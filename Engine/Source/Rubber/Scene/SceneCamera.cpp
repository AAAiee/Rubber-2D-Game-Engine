#include <pch.h>
#include "SceneCamera.h"

namespace Rubber{

	SceneCamera::SceneCamera()
	{
		recalculateMatrix();
	}

	void SceneCamera::onResize(const float width, const float height)
	{
		m_AspectRatio = width / height;
		recalculateMatrix();
	}

	void SceneCamera::recalculateMatrix()
	{
		RB_CORE_ASSERT(m_ActiveProjectionType != ProjectionType::Persp, "Perspective projection is not supported!");

		if (m_ActiveProjectionType == ProjectionType::Ortho){
 
			float orthoLeft   =  -m_OrthoSize * m_AspectRatio * 0.5f;
			float orthoRight  =  -orthoLeft;
			float orthoTop    =   m_OrthoSize * 0.5f;
			float orthoBottom =  -orthoTop;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthoNear, m_OrthoFar); 
		}
	}

}


