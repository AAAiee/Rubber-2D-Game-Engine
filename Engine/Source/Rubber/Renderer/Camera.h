#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Rubber{

	class Camera {
	public:
		Camera() = default;
		Camera(float fovY, float apsectRatio, float zNear, float zFar);
		Camera(const glm::mat4& projection); // currently for orthogonal camera initialization 

		inline void setProjection(const glm::mat4& projectionMatrix) {
			this->m_ProjectionMatrix = projectionMatrix;
		};
		
		inline void setViewMatrix(const glm::mat4& viewMatrix) {
			this->m_ViewMatrix = viewMatrix;
		}

		inline glm::mat4 getViewMatrix() const {
			return this->m_ViewMatrix;
		}

		inline glm::mat4 getViewProjectionMatrix() const {
			return this->m_ProjectionMatrix * this->m_ViewMatrix;
		}

	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	};
}
