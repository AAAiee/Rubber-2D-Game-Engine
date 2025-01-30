#pragma once
#include <sstream>
#include "Event.h"

namespace Rubber
{
	/**
  * @class MouseMoveEvent
  * @brief Event class for mouse movement.
  */
	class RB_API MouseMoveEvent : public Event
	{
	public:
		/**
   * @brief Constructor for MouseMoveEvent.
   * @param x The x-coordinate of the mouse.
   * @param y The y-coordinate of the mouse.
   */
     MouseMoveEvent(float x, float y)
			:m_MouseX(x), m_MouseY(y) {
		}

		/**
   * @brief Gets the x-coordinate of the mouse.
   * @return The x-coordinate.
   */
		inline float getX() const { return m_MouseX; }

		/**
   * @brief Gets the y-coordinate of the mouse.
   * @return The y-coordinate.
   */
		inline float getY() const { return m_MouseY; }

		/**
   * @brief Converts the event data to a string.
   * @return A string representation of the event.
   */
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Mouse moved to x: " << getX() << " y: " << getY();
			return ss.str();
		}

		EVENT_CATEGORY(toUnderType(EventCategory::MOUSE) | toUnderType(EventCategory::INPUT))
		EVENT_TYPE(EventType::MouseMoved)

	private:
		float m_MouseX, m_MouseY;
	};

	/**
  * @class MouseScrolledEvent
  * @brief Event class for mouse scrolling.
  */
	class RB_API MouseScrolledEvent : public Event
	{
	public:
		/**
   * @brief Constructor for MouseScrolledEvent.
   * @param xOffset The horizontal scroll offset.
   * @param yOffset The vertical scroll offset.
   */
		MouseScrolledEvent(float xOffset, float yOffset)
			:m_xOffset(xOffset), m_yOffset(yOffset) {
		}

		/**
   * @brief Gets the horizontal scroll offset.
   * @return The horizontal scroll offset.
   */
		inline float getXOffset() const { return m_xOffset; }

		/**
   * @brief Gets the vertical scroll offset.
   * @return The vertical scroll offset.
   */
		inline float getYOffset() const { return m_yOffset; }

		/**
   * @brief Converts the event data to a string.
   * @return A string representation of the event.
   */
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Mouse Scrolled with x: " << getXOffset() << " y: " << getYOffset();
			return ss.str();
		}

		EVENT_CATEGORY(toUnderType(EventCategory::MOUSE) | toUnderType(EventCategory::INPUT))
		EVENT_TYPE(EventType::MouseScrolled)

	private:
		float m_xOffset, m_yOffset;
	};

	/**
  * @class MouseEvent
  * @brief Base class for mouse button events.
  */
	class RB_API MouseEvent : public Event
	{
	public:
		EVENT_CATEGORY(toUnderType(EventCategory::MOUSE) | toUnderType(EventCategory::INPUT))

		/**
   * @brief Gets the mouse button associated with the event.
   * @return The mouse button.
   */
		inline int getButton() const { return m_button; }

	protected:
		/**
   * @brief Constructor for MouseEvent.
   * @param button The mouse button.
   */
		MouseEvent(int button)
			:m_button(button) { }

	private:
		int m_button;
	};

	/**
  * @class MousePressedEvent
  * @brief Event class for mouse button press.
  */
	class RB_API MousePressedEvent : public MouseEvent
	{
	public:
		/**
   * @brief Constructor for MousePressedEvent.
   * @param button The mouse button that was pressed.
   */
		MousePressedEvent(int button)
			:MouseEvent(button){}
		
		EVENT_TYPE(EventType::MouseButtonPressed)
	
		/**
   * @brief Converts the event data to a string.
   * @return A string representation of the event.
   */
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Mouse key (Pressed): " << getButton();
			return ss.str();
		}
	};

	/**
  * @class MouseReleasedEvent
  * @brief Event class for mouse button release.
  */
	class RB_API MouseReleasedEvent : public MouseEvent
	{
	public:
		/**
   * @brief Constructor for MouseReleasedEvent.
   * @param button The mouse button that was released.
   */
		MouseReleasedEvent(int button)
			:MouseEvent(button){}
		
		EVENT_TYPE(EventType::MouseButtonPressed)
	
		/**
   * @brief Converts the event data to a string.
   * @return A string representation of the event.
   */
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "Mouse key (Released): " << getButton();
			return ss.str();
		}
	};
}
