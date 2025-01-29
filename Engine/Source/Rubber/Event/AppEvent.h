#pragma once
#include "Event.h" 

namespace Rubber
{
	class RB_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {
		}

		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int getHeight() const { return m_Height; }
		
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CATEGORY(toUnderType(EventCategory::WINDOW))
		EVENT_TYPE(EventType::WindowResized)
	private:
		unsigned int m_Width, m_Height;
	};
}
