//#pragma once
//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include "Rubber/Event/EventManager.h"
//#include "Rubber/Renderer/Camera.h"
//
//namespace Rubber {
//	class Event;
//	class MouseScrolledEvent;
//	class WindowResizeEvent;
//	class CameraController {
//	public:
//		CameraController(float fovy, float aspectRatio, float zNear, float zFar, Ref<EventManager>& em);
//		~CameraController();
//
//		inline const Camera& getCamera() const { return this->m_Camera; }
//		inline void setPosition(const glm::vec3& pos) { this->m_Position = pos;}
//		
//		glm::vec3 getFrontDirection () const;
//		glm::vec3 getUpDirection() const;
//		glm::vec3 getRightDirection() const;
//		
//		float getZoom() const {
//			return this->m_Zoom;
//		}
//		
//		void onUpdate();
//		
//	private:
//		void updateViewMatrix();
//		bool onMouseScrolled(const MouseScrolledEvent& e);
//		bool onWindowResize(const WindowResizeEvent& e);
//
//	private:
//		glm::vec3 m_Position;
//
//		// quaternion used to record orientation(rotations)
//		glm::quat m_Orientation;
//
//		// points to -z under euler angle system
//		float m_Yaw   =  0.0f;
//		float m_Roll  =  0.0f;
//		float m_Pitch =  0.0f;
//
//		//speed
//		float m_TranslationSpeed = 6.0f;
//		float mouseSensitivity = 0.06f;
//		float m_Zoom;
//
//		float m_AspectRatio;
//		float m_ZNear;
//		float m_ZFar;
//
//		//mouse last Pos
//		glm::vec2 m_LastMousePos{};
//		bool m_FirstMouse = true;
//
//		//camera(view matrix and perspective matrix)
//		Camera m_Camera;
//
//		// cursor enabled?
//		bool m_IsCursorDisabled = false ;
//		Ref<EventManager> m_Em;
//
//	};
//
//}
