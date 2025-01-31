#pragma once
#include <Rubber/Core.h>

namespace Rubber {
    /**
     * @brief A EventType enum class that represents all kinds of events
     */
    enum class EventType  
    {
        None = 0,
        MouseScrolled, MouseButtonPressed, MouseButtonReleased,MouseMoved,
        KeyboardPressed, KeyboardReleased,
        WindowClosed, WindowResized, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppRender, AppUpdate
    };

    /**
     * @brief EventCategory represents the category of the event
     * using Bit flags so that we can easily check if an event belongs to a, or multiple categories;
     */
    enum class EventCategory
    {
        None = 0,
        INPUT       = BIT(0),        ///< Input event category
        APPLICATION = BIT(1),        ///< Application event category
        KEYBOARD    = BIT(2),        ///< Keyboard event category
        MOUSE       = BIT(3),        ///< Mouse event category
        WINDOW      = BIT(4)         ///< Window event category
    };

    /**
     * @brief Macro to define the event category for an event class
     * @param category The event category
     */
#define EVENT_CATEGORY(category) std::underlying_type_t<EventCategory> getEventCategory() const override { return category;}

    /**
     * @brief Macro to define the event type for an event class
     * @param type The event type
     */
#define EVENT_TYPE(type) static EventType s_getEventType() {return type;}\
                         EventType getEventType() const override {return s_getEventType();}\
                         const char* getTypeName() const override {return #type;}

    /**
     * @brief Base class for all events
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

    private:
        bool m_isHandled = false;   ///< Flag indicating if the event is handled, used to stop event propagation
    };
}
