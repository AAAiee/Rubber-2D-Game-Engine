#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Rubber{

	class Camera {
	public:
		Camera() = default;
		virtual ~Camera() = default;
		Camera(const glm::mat4& projection); 

		inline void setProjection(const glm::mat4& projectionMatrix) {
			this->m_ProjectionMatrix = projectionMatrix;
		};

		inline glm::mat4& getProjection(){
			return m_ProjectionMatrix;
		}

		inline const glm::mat4& getProjection() const {
			return m_ProjectionMatrix;
		}

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};
}
