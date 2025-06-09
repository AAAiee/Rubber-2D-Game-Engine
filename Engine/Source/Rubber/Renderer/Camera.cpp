#include "pch.h"
#include "Camera.h"

namespace Rubber {
    Camera::Camera(float fovY, float aspectRatio, float zNear, float zFar)
		:m_ProjectionMatrix(glm::perspective(glm::radians(fovY), aspectRatio, zNear, zFar)), m_ViewMatrix(1.0f)
	{
		
	}

}
