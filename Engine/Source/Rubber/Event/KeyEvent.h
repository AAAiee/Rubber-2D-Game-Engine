#pragma once
#include "Event.h"
#include <sstream>
#include "Rubber/Core.h"

namespace Rubber
{
	/**
	* @brief A KeyEvent class that represents all kinds of key events.
	* Key Pressed Event and Key Released Event both have the same keycode attribute.
	*/
	class RB_API KeyEvent : public Event
	{
	protected:
	/**
    * @brief Constructs a KeyEvent with the given keycode.
    * @param Keycode The keycode of the key event.
    */
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode) { }

	public:
		/**
		* @brief Gets the category flags for the key event.
		*/
		EVENT_CATEGORY(toUnderType(EventCategory::KEYBOARD) | toUnderType(EventCategory::INPUT))

	   /**
	   * @brief Gets the keycode of the key event.
	   * @return The keycode of the key event.
	   */
		inline int getKeyCode() const { return m_KeyCode; }

	private:
		int m_KeyCode; ///< The keycode of the key event.
	};

	/**
    * @brief A KeyPressedEvent class that represents a key pressed event.
    */
	class RB_API KeyPressedEvent : public KeyEvent
	{
	public:
	   /**
	   * @brief Constructs a KeyPressedEvent with the given keycode and repeat count.
	   * @param keyCode The keycode of the key pressed event.
	   * @param repeatCount The repeat count of the key pressed event.
	   */
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) { }

	   /**
	   * @brief Converts the key pressed event to a string representation.
	   * @return A string representation of the key pressed event.
	   */
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "The current key being pressed is " << getKeyCode()
				<< " with repeating count of " << getRepeatCount();
			return ss.str();
		}

	   /**
	   * @brief Gets the repeat count of the key pressed event.
	   * @return The repeat count of the key pressed event.
	   */
		inline int getRepeatCount() const { return m_RepeatCount; }

		EVENT_TYPE(EventType::KeyboardPressed)

	private:
		int m_RepeatCount; ///< The repeat count of the key pressed event.
	};

	/**
	* @brief A KeyReleasedEvent class that represents a key released event.
	*/
	class RB_API KeyReleasedEvent : public KeyEvent
	{
	public:
	   /**
	   * @brief Constructs a KeyReleasedEvent with the given keycode.
	   * @param keyCode The keycode of the key released event.
	   */
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) { }
		
	   /**
	   * @brief Converts the key released event to a string representation.
	   * @return A string representation of the key released event.
	   */
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "The current key being released is " << getKeyCode();
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyboardReleased)
	};
}
