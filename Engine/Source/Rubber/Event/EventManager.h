#pragma once
#include "Rubber/Event/EventBus.h"


namespace Rubber{

	class EventManager{
	private:
		public: 
		
		EventManager()
		{
			m_EventQueue.reserve(128);
		}

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

		void flush() {
			RB_PROFILE_FUNC();
			for(auto& fn : m_EventQueue){
				fn();
			}
			m_EventQueue.clear();
		}


		template<typename Event>
		void clear(const Event& e){
			EventBus<Event>::clear();
		}

	private:
		Vector<std::function<void()>> m_EventQueue;
	};
	


}

