#pragma once 
#include "Rubber/Renderer/Camera.h"


namespace Rubber{
	class EventManager;
	class WindowResizeEvent;
	class KeyPressedEvent;
	class MouseMoveEvent;
	class MouseScrolledEvent;

	struct OrthoCameraBounds{
		float left; 
		float right;
		float bottom; 
		float top;

		float getWidth() const{
			return right - left;
		}

		float getHeight() const{
			return top - bottom;
		}
	};

	class OrthoCameraController {
	public:
		OrthoCameraController(float aspectRatio, Ref<EventManager>& em, bool rotation = false);
		~OrthoCameraController();

		const Camera& getCamera() const;
		inline void setPosition(const glm::vec3& newPos){
			m_Position = newPos;
		}

		inline const glm::vec3& getPosition() const{
			return m_Position;
		}

		void ProcessInputs();
		void setViewMatrix(const glm::vec3& position, const float rotation);
		inline void setZoomLevel(float zoomLevel){
			m_ZoomLevel = zoomLevel;
		}
		inline const OrthoCameraBounds& getBounds() const{
			return m_Bounds;
		}

	private:
		void setProjection();
		bool onWindowResize(const WindowResizeEvent& e);
		bool onMouseScrolled(const MouseScrolledEvent& e);
		void updateCurrentSpeed();

	private:
		float m_AspectRatio;
		float m_ZoomLevel{ 1.0f };

		OrthoCameraBounds m_Bounds;
		Camera m_Camera;

		bool m_IsRotationEnabled{ false };
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		float m_RotationAngles = 0.0f;
		float m_TranslationSpeed = 5.0f;
		float m_RotationSpeed = 180.0f;

		//event handling
		Ref<EventManager> m_EventManager;
	};


}

