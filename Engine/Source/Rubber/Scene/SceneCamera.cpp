#include <pch.h>
#include "SceneCamera.h"

namespace Rubber{

	SceneCamera::SceneCamera()
	{
		recalculateMatrix();
	}


	void SceneCamera::setProjectionType(ProjectionType type)
	{
		m_ActiveProjectionType = type;
		recalculateMatrix();
	}

	Rubber::SceneCamera::ProjectionType SceneCamera::getProjectionType() const
	{
		return m_ActiveProjectionType;
	}

	void SceneCamera::setOrtho(float size, float aspectRatio, float clipNear, float clipFar)
	{
		m_OrthoSize = size;
		m_AspectRatio = aspectRatio;
		m_OrthoNear = clipNear;
		m_OrthoFar = clipFar;
		recalculateMatrix();
	}

	void SceneCamera::onResize(float width, float height)
	{
		m_AspectRatio = width / height;
		recalculateMatrix();
	}


	void SceneCamera::recalculateMatrix()
	{

		if (m_ActiveProjectionType == ProjectionType::Ortho) {

			float orthoLeft = -m_OrthoSize * m_AspectRatio * 0.5f;
			float orthoRight = -orthoLeft;
			float orthoTop = m_OrthoSize * 0.5f;
			float orthoBottom = -orthoTop;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthoNear, m_OrthoFar);
		}
		else {
			m_ProjectionMatrix = glm::perspective(m_PersFovY, m_AspectRatio, m_PersNear, m_PersFar);
		}
	}

}


