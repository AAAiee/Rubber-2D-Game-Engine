#pragma once
#include "Rubber/Core.h"

namespace Rubber {

    template <typename T>
    inline constexpr std::underlying_type_t<T> toUnderType(T enumarator)
    {
        return static_cast<std::underlying_type_t<T>> (enumarator);
    }

    /**
     * @brief A EventType enum class that represents all kinds of events
	 * each event type is a unique value that can be used to identify the type of the event
	 * using enum class instead of enum to avoid polluting the global namespace
     */
    enum class EventType:unsigned char  
    {
        None = 0,
        MouseScrolled, MouseButtonPressed, MouseButtonReleased,MouseMoved,
        KeyboardPressed, KeyboardReleased,
        WindowClosed, WindowResized, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppRender, AppUpdate
    };

    /**
     * @brief EventCategory represents the category of the event
     * using Bit flags so that we can easily check if an event belongs to any or multiple categories;
	 * each type of event can belong to multiple categories
	 * example: MouseButtonPressed event belongs to INPUT, MOUSE and MOUSE_BUTTON categories
     */
    enum class EventCategory:unsigned char
    {
        None = 0,
        INPUT         = BIT(0),        ///< Input event category
        APPLICATION   = BIT(1),        ///< Application event category
        KEYBOARD      = BIT(2),        ///< Keyboard event category
        MOUSE         = BIT(3),        ///< Mouse event category
		MOUSE_BUTTON  = BIT(4)         ///< Mouse button event category
    };

/**
* @brief Macro to define the event category for an event class
* @param category The event category a event type is belong to
* example usage: EVENT_CATEGORY(EventCategory::INPUT | EventCategory::MOUSE)
*/
#define EVENT_CATEGORY(category) std::underlying_type_t<EventCategory> getEventCategory() const override { return category;}

/**
* @brief Macro to define the event type for an event class
* @param eventType a event belongs to 
* Static Version helps to get the event type without creating an instance of the event, make it
* easier to check the event type when dispatching events
* example usage: EVENT_TYPE(EventType::MouseScrolled)
*/
#define EVENT_TYPE(type) static EventType s_GetEventType() {return type;}\
                         EventType getEventType() const override {return s_GetEventType();}\
                         const char* getTypeName() const override {return #type;}

    /**
     * @brief Base class for all events
	 * a interface class that all events should inherit from
     */
    class RB_API Event
    {
    public:
        /**
         * @brief Get the event category
         * @return The event category
         */
        virtual std::underlying_type_t<EventCategory> getEventCategory() const = 0;
        
        /**
         * @brief Get the event type
         * @return The event type
         */
        virtual EventType getEventType() const = 0;

        /**
         * @brief Get the type name of the event
         * @return The type name of the event
         */
        virtual const char* getTypeName() const = 0; 

        /**
         * @brief Convert the event to a string representation
         * @return The string representation of the event
         */
        virtual std::string toString() const { return getTypeName(); }

        /**
         * @brief Check if the event belongs to a specific category
         * @param category The category to check against
         * @return True if the event belongs to the category, false otherwise
         */
        inline bool isInCategory(const EventCategory& category) const 
        {
            return (toUnderType(category) & getEventCategory());
        }

        inline bool isHandled() const 
        {
            return m_isHandled;
        }

    private:
		// make EventDispatcher a friend class so that it can access the m_isHandled flag to stop event propagattion 
		friend class EventDispatcher;
        bool m_isHandled = false;  
    };


	/**
	 * @brief EventDispatcher class that dispatches events to event handlers
	 */

    class RB_API EventDispatcher
    {
    private:
		// Event Handler type that takes a reference to the event and returns a boolean value
        template <typename T>
        using EventFn = std::function<bool(T&)>;

    public:
		// Constructor that takes a reference to the event
        EventDispatcher(Event& event)
            : m_Event(event) {
        }

		/**
		 * @brief Dispatch the event to the given function
		 * @tparam T The type of the event
		 * @param func The function to dispatch the event to
		 * @return True if the event was handled, false otherwise
		 */
        template <typename T>
        bool dispatch(EventFn<T> func) {
            // if the given func is a function that handle the current event type
			// dispatch the event to the function 
            if (m_Event.getEventType() == T::s_GetEventType()) {
                m_Event.m_isHandled =  func(*static_cast<T*> (&m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };
    

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}
