#include <pch.h>
#include "OrthoCameraController.h"
#include "Rubber/Input/Input.h"
#include "Rubber/Input/KeyCodes.h"


#include "Rubber/Event/EventManager.h"
#include "Rubber/Event/AppEvent.h"
#include "Rubber/Event/MouseEvent.h"


namespace Rubber{


	OrthoCameraController::OrthoCameraController(float aspectRatio, Ref<EventManager>& em, bool rotation /*= false*/)
		:m_AspectRatio(aspectRatio), m_Bounds({ -aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_IsRotationEnabled(rotation),
		m_Camera(glm::ortho(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top)), m_EventManager(em)
	{ 
		m_EventManager->subscribe<WindowResizeEvent>("Camera Resize", [this](const WindowResizeEvent& e) { return onWindowResize(e); });
		m_EventManager->subscribe<MouseScrolledEvent>("Camera Zoom", [this](const MouseScrolledEvent& e) { return onMouseScrolled(e); });
	}

	OrthoCameraController::~OrthoCameraController()
	{
		m_EventManager->unsubscribe<WindowResizeEvent>("Camera Resize");
		m_EventManager->unsubscribe<MouseScrolledEvent>("Camera Zoom");
	}

	void OrthoCameraController::setProjection()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.setProjection(glm::ortho(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top));
	}

	bool OrthoCameraController::onWindowResize(const WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();
		setProjection();

		return false;
	}

	void OrthoCameraController::ProcessInputs()
	{
		float ts = 1.0f / 120.0f;
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
		setProjection();

		return false;
	}

	

	void OrthoCameraController::updateCurrentSpeed()
	{
		m_TranslationSpeed = m_ZoomLevel * 5.0f;
	}

	void OrthoCameraController::setViewMatrix(const glm::vec3& position, const float rotationAngles)
	{
		glm::mat4 viewMatrix = glm::rotate(glm::translate(glm::mat4(1.0f), -position), glm::radians(-rotationAngles), glm::vec3(0.0f, 0.0f, 1.0f));

		m_Camera.setViewMatrix(viewMatrix);
	}

	const Rubber::Camera& OrthoCameraController::getCamera() const
	{
		return m_Camera;
	}

}