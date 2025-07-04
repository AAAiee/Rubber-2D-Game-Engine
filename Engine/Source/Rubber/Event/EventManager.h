#pragma once
#include "Rubber/Event/EventBus.h"


namespace Rubber{

	class EventManager{
	private:
		public: 
		EventManager(uint32_t capacity);

		template<typename Event>
		void subscribe(std::string_view name, EventBus<Event>::EventHandler&& callBack){
			EventBus<Event>::subscribe(name, std::move(callBack));
		}

		template<typename Event>
		void unsubscribe(std::string_view name){
			 EventBus<Event>::unsubscribe(name);
		}

		template<typename Event>
		// this needs to be a unique pointer for complex event, 
		// this only works for event with primitive data 
		void enqueue(Event event){
			RB_PROFILE_FUNC();
			auto fn = [ev = std::move(event)]() mutable 
				{
					EventBus<Event>::processAllHandlers(ev);
				};

			this->m_EventQueue.emplace_back(fn);
		}

		template<typename Event>
		void clear(const Event& e) {
			EventBus<Event>::clear();
		}

		void flush();
		static Ref<EventManager> create(uint32_t capacity=128);

	private:
		Vector<std::function<void()>> m_EventQueue;
	};
}

