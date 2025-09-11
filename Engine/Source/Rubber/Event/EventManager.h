#pragma once
#include "Rubber/Event/EventBus.h"


namespace Rubber{

	class EventManager{
	private:
		template<typename Event>
		using EventHandlerFunc = std::function<bool(const Event& event)>;

	public: 
		EventManager(uint32_t capacity);

		template<typename Event>
		void subscribe(std::string_view name, EventBus<Event>::EventHandler&& handler);

		template<typename Event>
		void unsubscribe(std::string_view name);

		template<typename Event>
		void enqueue(const Event& event);

		void dispatchAllEvents(); // in queued order

		template<typename Event> 
		bool dispatchEvent(Event& event, const EventHandlerFunc<Event>& func); // immediately

		static Ref<EventManager> create(uint32_t capacity=128);


	private:
		Vector<std::function<void()>> m_EventQueue;
	};

	template<typename Event>
	bool Rubber::EventManager::dispatchEvent(Event& event, const EventHandlerFunc<Event>& func)
	{
		if (Event::getStaticType() == event.getEventType()) {
			event.getHandledRef() = func(event);
			return true;
		}
		return false;
	}

	template<typename Event>
	void Rubber::EventManager::subscribe(std::string_view name, EventBus<Event>::EventHandler&& handler)
	{
		EventBus<Event>::registerEventHandler(name, std::move(handler));
	}

	template<typename Event>
	void Rubber::EventManager::unsubscribe(std::string_view name)
	{
		EventBus<Event>::unregisterEventHandler(name);
	}

	template<typename Event>
	void Rubber::EventManager::enqueue(const Event& event)
	{
		RB_PROFILE_FUNC();
		auto fn = [ev = event]() mutable
			{
				EventBus<Event>::processAllHandlers(ev);
			};

		this->m_EventQueue.emplace_back(fn);
	}
}

