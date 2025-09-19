#pragma once
#include "Rubber/Event/EventBus.h"


namespace Rubber{

	class EventManager{  
	private:
		template<typename EventType>
		using EventHandlerFunc = std::function<bool(const EventType& e)>;

	public: 
		template<typename EventType>
		void subscribe(std::string_view name, EventBus<EventType>::EventHandler&& handler);

		template<typename EventType>
		void unsubscribe(std::string_view name);


		template<typename EventType>
		void enqueue(const EventType& e);

		void dispatchAllEvents(); // in queued order

		template<typename EventType>
		bool dispatchEvent(EventType& e, const EventHandlerFunc<EventType>& func); // immediately

		static Ref<EventManager> create();

		~EventManager() = default;

	private:
		EventManager() = default ;
		Vector<std::function<void()>> m_CurrentQueue;
		Vector<std::function<void()>> m_NextQueue;
	};

	template<typename EventType>
	bool Rubber::EventManager::dispatchEvent(EventType& e, const EventHandlerFunc<EventType>& func)
	{
		if (EventType::getStaticType() == e.getEventType()) {
			e.setHandleStatus(func(e));
			return true;
		}
		return false;
	}

	template<typename EventType>
	void Rubber::EventManager::subscribe(std::string_view name, EventBus<EventType>::EventHandler&& handler)
	{
		EventBus<EventType>::registerEventHandler(name, std::move(handler));
	}

	template<typename EventType>
	void Rubber::EventManager::unsubscribe(std::string_view name)
	{
		EventBus<EventType>::unregisterEventHandler(name);
	}

	template<typename EventType>
	void Rubber::EventManager::enqueue(const EventType& e)
	{
		RB_PROFILE_FUNC();
		auto fn = [ev = e]() mutable {  // pay a copy, for now all events ar small so no smart pointer involved
			EventBus<EventType>::processAllHandlers(ev);
		};
		m_NextQueue.emplace_back(std::move(fn));
	}
}

