#include <pch.h>
#include "OrthoCameraController.h"
#include "RB/Input/Input.h"
#include "RB/Input/KeyCodes.h"


#include "RB/Event/EventManager.h"
#include "RB/Event/AppEvent.h"
#include "RB/Event/MouseEvent.h"


namespace RB{


	OrthoCameraController::OrthoCameraController(float aspectRatio, const Ref<EventManager>& em, bool rotation /*= false*/)
		:m_AspectRatio(aspectRatio), m_Bounds({ -aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_IsRotationEnabled(rotation),
		m_Camera(glm::ortho(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top)), m_EventManager(em)
	{ 

	}

	OrthoCameraController::~OrthoCameraController()
	{
		unsubscribeAllEvent();
	}

	void OrthoCameraController::updateProjectionMatrix()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.setProjection(glm::ortho(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top));
	}

	bool OrthoCameraController::onWindowResize(const WindowResizeEvent& e)
	{
		updateAspectRatio((float)e.getWidth(), (float)e.getHeight());
		return false;
	}

	void OrthoCameraController::ProcessInputs(const float ts)
	{
		float movementVelocity = m_TranslationSpeed * ts;
		float angularVelocity = m_RotationSpeed * ts;

		//not the camera's right
		glm::vec3 localRight = { glm::cos(glm::radians(-m_RotationAngles)), glm::sin(glm::radians(-m_RotationAngles)), 0.0f };
		glm::vec3 localUp = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), localRight));

		glm::vec3 delta = glm::vec3(0.0f);
		if (Input::isKeyPressed(RB_KEY_A))
		{
			delta -= localRight;
		}

		if (Input::isKeyPressed(RB_KEY_D))
		{
			delta += localRight;
		}

		if (Input::isKeyPressed(RB_KEY_W))
		{
			delta += localUp;
		}

		if (Input::isKeyPressed(RB_KEY_S))
		{
			delta -= localUp;
		}

		if (delta != glm::vec3{ 0.0f, 0.0f, 0.0f }) {
			glm::vec3 direction = glm::normalize(delta);
			m_Position += direction * movementVelocity;
		}

		if (m_IsRotationEnabled) {
			if (Input::isKeyPressed(RB_KEY_Q)) {
				m_RotationAngles -= angularVelocity;
			}

			if (Input::isKeyPressed(RB_KEY_E)) {
				m_RotationAngles += angularVelocity;
			}

			if (m_RotationAngles > 180.0f) {
				m_RotationAngles -= 360.0f;
			}
			else if (m_RotationAngles < -180.0f) {
				m_RotationAngles += 360.0f;
			}
		}

		setViewMatrix(m_Position, m_RotationAngles);
	}

	bool OrthoCameraController::onMouseScrolled(const MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.getYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		updateCurrentSpeed();
		updateProjectionMatrix();
		return false;
	}

	

	void OrthoCameraController::updateCurrentSpeed()
	{
		m_TranslationSpeed = m_ZoomLevel * 5.0f;
	}

	void OrthoCameraController::updateAspectRatio(const float width, const float height)
	{
		m_AspectRatio = width / height;
		updateProjectionMatrix();
	}

	void OrthoCameraController::subscribeAllEvent()
	{
		RB_INFO("subscribe all events");
		m_EventManager->subscribe<WindowResizeEvent>("Camera Resize", [this](const WindowResizeEvent& e) { return onWindowResize(e); });
		m_EventManager->subscribe<MouseScrolledEvent>("Camera Zoom", [this](const MouseScrolledEvent& e) { return onMouseScrolled(e); });
	}

	void OrthoCameraController::unsubscribeAllEvent()
	{
		RB_INFO("unsubscribe all events");
		m_EventManager->unsubscribe<WindowResizeEvent>("Camera Resize");
		m_EventManager->unsubscribe<MouseScrolledEvent>("Camera Zoom");
	}

	void OrthoCameraController::setViewMatrix(const glm::vec3& position, const float rotationAngles)
	{
		glm::mat4 viewMatrix = glm::rotate(glm::translate(glm::mat4(1.0f), -position), glm::radians(-rotationAngles), glm::vec3(0.0f, 0.0f, 1.0f));

		m_Camera.setViewMatrix(viewMatrix);
	}

	const RB::OrthoCamera& OrthoCameraController::getCamera() const
	{
		return m_Camera;
	}

}