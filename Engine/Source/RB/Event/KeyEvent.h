#pragma once
#include "RB/Event/Event.h"

namespace RB
{
	/**
	* @brief A KeyEvent class that represents all kinds of key events.
	* Key Pressed Event and Key Released Event both have the same key code attribute.
	*/
	class  KeyEvent : public Event
	{
	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode) { }

	public:
		EVENT_CATEGORY(toUnderType(EventCategory::KEYBOARD) | toUnderType(EventCategory::INPUT))

		inline int getKeyCode() const { return m_KeyCode; }

	private:
		int m_KeyCode; ///< The key code of the key event.
	};

	/**
    * @brief A KeyPressedEvent class that represents a key pressed event.
    */
	class  KeyPressedEvent : public KeyEvent
	{
	public:
	   /**
	   * @brief Constructs a KeyPressedEvent with the given key code and repeat count.
	   * @param keyCode The key code of the key pressed event.
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
	class  KeyReleasedEvent : public KeyEvent
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
