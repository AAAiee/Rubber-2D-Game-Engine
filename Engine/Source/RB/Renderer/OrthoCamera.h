#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



//TODO::DELETE THIS WHOLE FILE, this is just for current testing hack ,will be removed eventually
namespace RB {

	class OrthoCamera {
	public:
		OrthoCamera() = default;
		~OrthoCamera() = default;
		OrthoCamera(const glm::mat4& projectionMatrix) {
			m_ProjectionMatrix = projectionMatrix;
		}

		inline void setProjection(const glm::mat4& projectionMatrix) {
			this->m_ProjectionMatrix = projectionMatrix;
		};

		inline void setViewMatrix(const glm::mat4& viewMatrix) {
			this->m_ViewMatrix = viewMatrix;
		}

		inline glm::mat4& getProjection() {
			return m_ProjectionMatrix;
		}

		inline const glm::mat4& getProjection() const {
			return m_ProjectionMatrix;
		}

		inline glm::mat4 getVpMatrix() const {
			return m_ViewMatrix * m_ProjectionMatrix;
		}

	protected:
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};
}