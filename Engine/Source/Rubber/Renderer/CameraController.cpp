#include <pch.h>
#include "Rubber/Renderer/CameraController.h"
#include "Rubber/Input/Input.h"
#include "Rubber/Input/KeyCodes.h"
#include "Rubber/Input/MouseCodes.h"
#include "Rubber/Event/Event.h"
#include "Rubber/Event/MouseEvent.h"
#include "Rubber/Event/AppEvent.h"
#include "imgui.h"


namespace Rubber{

	CameraController::CameraController(float fovy, float aspectRatio, float zNear, float zFar, Ref<EventManager>& em)
		: m_Position(), m_Zoom(fovy), m_AspectRatio(aspectRatio), m_ZNear(zNear), m_ZFar(zFar), m_Camera(this->m_Zoom, this->m_AspectRatio, this->m_ZNear, this->m_ZFar), m_Em(em)
	{
		m_Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		// subscribe event
		em->subscribe<MouseScrolledEvent>("Camera MouseScroll", [this](const MouseScrolledEvent& e)->bool {
			return onMouseScrolled(e);
			});

		em->subscribe<WindowResizeEvent>("Camera WindowResize",[this](const WindowResizeEvent& e)-> bool {
			return onWindowResize(e);
			});
	}

	CameraController::~CameraController()
	{
		m_Em->unsubscribe<MouseScrolledEvent>("Camera MouseScroll");
		m_Em->unsubscribe<WindowResizeEvent>("Camera WindowResize");
	}

	glm::vec3 CameraController::getFrontDirection() const
	{
		return glm::normalize(this->m_Orientation * glm::vec3(0, 0, -1));
	}

	glm::vec3 CameraController::getUpDirection() const
	{
		return glm::normalize(this->m_Orientation * glm::vec3(0, 1, 0));
	}

	glm::vec3 CameraController::getRightDirection() const
	{
		return glm::normalize(this->m_Orientation * glm ::vec3(1, 0, 0));
	}

	void CameraController::onUpdate()
	{
		// first make sure ImGui does not want the e
		ImGuiIO& io = ImGui::GetIO();
		bool imguiWantsMouse = io.WantCaptureMouse;
		bool cursorGrabDesired = (!imguiWantsMouse && Input::isMouseButtonPressed(RB_MOUSE_BUTTON_RIGHT));

		// if we want to move using mouse but the cursor is not disabled
		if (cursorGrabDesired && !m_IsCursorDisabled){
			Input::disableCursor();
			this->m_FirstMouse = true; // reset first mouse (smooth movement)
			this->m_IsCursorDisabled = true;
		}else if (!cursorGrabDesired && m_IsCursorDisabled){
			Input::enableCurosr();
			m_IsCursorDisabled = false;
		}

		if (m_IsCursorDisabled){

			Input::disableCursor();
			float deltaTime = 1.0f / 240.f;
			float transSpeed = this->m_TranslationSpeed * deltaTime;
			float rotationSpeed = 360.0f * deltaTime;

			if (Input::isKeyPressed(RB_KEY_W)) {
				m_Position += transSpeed * getFrontDirection();
			}
			else if (Input::isKeyPressed(RB_KEY_S)) {
				m_Position -= transSpeed * getFrontDirection();
			}
			if (Input::isKeyPressed(RB_KEY_A)) {
				m_Position -= transSpeed * getRightDirection();
			}
			else if (Input::isKeyPressed(RB_KEY_D)) {
				m_Position += transSpeed * getRightDirection();
			}

			//handle mouse movement rotation/ yaw, pitch
			auto [mousePosX, mousePosY] = Input::getMousePosition();
			if (m_FirstMouse) {
				this->m_LastMousePos = { mousePosX, mousePosY };
				m_FirstMouse = false;
			}
			float yOffset = this->m_LastMousePos.y - mousePosY;
			float xOffset = mousePosX - this->m_LastMousePos.x;

			xOffset *= mouseSensitivity;
			yOffset *= mouseSensitivity;

			m_Yaw += xOffset;
			m_Pitch += yOffset;;

			glm::quat yawDelta = glm::angleAxis(glm::radians(-this->m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat pitchDelta = glm::angleAxis(glm::radians(this->m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
			this->m_Orientation = glm::normalize(yawDelta * pitchDelta);

			this->m_LastMousePos = { mousePosX, mousePosY };
		}else{
			Input::enableCurosr();
		}

		updateViewMatrix();
	}



	void CameraController::updateViewMatrix()
	{
		glm::quat rotation = glm::conjugate(this->m_Orientation);
		glm::mat4 rot = glm::mat4_cast(rotation);
		glm::mat4 viewMatrix = glm::translate(rot, glm::vec3(-m_Position));
		this->m_Camera.setViewMatrix(viewMatrix);
	}

	bool CameraController::onMouseScrolled(const MouseScrolledEvent& e)
	{
		float yOffset = e.getYOffset();
		m_Zoom -= yOffset;
		m_Zoom = std::clamp(m_Zoom, 1.0f, 60.0f);
		this->m_Camera.setProjection(
			glm::perspective(glm::radians(this->m_Zoom), this->m_AspectRatio, this->m_ZNear, this->m_ZFar));
		return false;
	}

	bool CameraController::onWindowResize(const WindowResizeEvent& e)
	{
		int newWidth, newHeight;
		newWidth = e.getWidth(), newHeight = e.getHeight();
		if (newWidth != 0 && newHeight != 0) {

			this->m_AspectRatio = (float)newWidth / (float)newHeight;
		}
		else{
			this->m_AspectRatio = 1.0f;
		}

		this->m_Camera.setProjection(
			glm::perspective(glm::radians(this->m_Zoom), this->m_AspectRatio, this->m_ZNear, this->m_ZFar));

		return false;
	}
}

