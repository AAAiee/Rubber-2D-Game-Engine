#pragma once
#include "Event.h" 

namespace Rubber
{
	/**
	* @brief WindowResizeEvent class that represents the window resize event
	* fields : width The width of the window
	*		   height The height of the window
	* 
	**/
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

		EVENT_CATEGORY(toUnderType(EventCategory::APPLICATION))
		EVENT_TYPE(EventType::WindowResized)
	private:
		unsigned int m_Width, m_Height;
	};


	/**
	* @brief WindowCloseEvent class that represents the window close event
	* 
	**/
	class RB_API WindowCloseEvent : public Event
	{ 
	public:
		WindowCloseEvent() = default;
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}
		EVENT_CATEGORY(toUnderType(EventCategory::APPLICATION))
		EVENT_TYPE(EventType::WindowClosed)
	};


	class RB_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "AppTickEvent";
			return ss.str();
		}
		EVENT_CATEGORY(toUnderType(EventCategory::APPLICATION))
		EVENT_TYPE(EventType::AppTick)
	};


	class RB_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "AppUpdateEvent";
			return ss.str();
		}
		EVENT_CATEGORY(toUnderType(EventCategory::APPLICATION))
		EVENT_TYPE(EventType::AppUpdate)
	};


	class RB_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "AppRenderEvent";
			return ss.str();
		}
		EVENT_CATEGORY(toUnderType(EventCategory::APPLICATION))
		EVENT_TYPE(EventType::AppRender)
	};

}
