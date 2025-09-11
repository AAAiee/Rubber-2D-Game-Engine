#pragma once
#include "Rubber/Event/Event.h"
#include "Rubber/Debug/Profiler.h"
#include "Rubber/Core/Search.h"
#include "Rubber/Core/Assert.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <functional>

namespace Rubber {

	template <typename T>
	class EventBus{

	public:
		using EventHandler = std::function<bool(const T& event)>;

		static void registerEventHandler(std::string_view name, EventHandler&& handler) {
			RB_PROFILE_FUNC();
			if (name.empty()) {
				RB_CORE_ASSERT(false); // can not have empty name, empty name is invalid id
			}
			// check to see if the handler is already in the pool
			auto handlerItr = s_NamePool.find(name.data());
			RB_CORE_ASSERT(handlerItr == s_NamePool.end(), "trying to register a handler that already existed");

			s_Handlers.emplace_back(std::move(handler), std::string(name));
			s_NamePool.emplace(name.data());
		}

		static void unregisterEventHandler(std::string_view name){
			RB_PROFILE_FUNC();

			// first check if the name is a existed handler name in the pool
			auto iterator = s_NamePool.find(name.data());
			RB_CORE_ASSERT(iterator != s_NamePool.end(), "the handler's name does not exists!");

			auto vecIt = std::find_if(s_Handlers.begin(), s_Handlers.end(),
				[&](auto& h) { return h.name == name.data(); });


			s_Handlers.erase(vecIt);
			s_NamePool.erase(name.data());
		}

		static void processAllHandlers(T& e){
			RB_PROFILE_FUNC();
			if (e.getHandledRef()) return; // if already handled, do nothing

			for(auto& handlerData : s_Handlers){
				RB_INFO("current processing handler: {}", handlerData.name);
				bool& isEventHandled = e.getHandledRef();
				if (handlerData.handler(e)) {
					isEventHandled = true;
				}
			}
		}

	private:
		struct EventHandlerData {
			EventHandler handler;
			std::string name;  // must be unique for the same event type

			EventHandlerData(EventHandler&& handler, std::string&& name)
				: handler(std::move(handler)), name(std::move(name)) {
			}

			EventHandlerData(EventHandlerData&& other) noexcept
				: handler(std::move(other.handler)), name(std::move(other.name)) {
			}

			EventHandlerData& operator=(EventHandlerData&& other) noexcept {
				if (this != &other) {
					handler = std::move(other.handler);
					name = std::move(other.name);
				}
				return *this;
			}

			EventHandlerData(const EventHandlerData& other) = default;
			EventHandlerData& operator=(const EventHandlerData& other) = default;

			bool operator ==(const EventHandlerData& other) const {
				return  name == other.name;
			}
		};

	private:
		static Vector<EventHandlerData> s_Handlers;
		static std::unordered_set<std::string>s_NamePool;
	};

	template<typename T>
	Vector<typename EventBus<T>::EventHandlerData> EventBus<T>::s_Handlers;

	template<typename T>
	std::unordered_set<std::string> EventBus<T>::s_NamePool;

}

