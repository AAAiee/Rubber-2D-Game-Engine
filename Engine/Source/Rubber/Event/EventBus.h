#pragma once
#include "Rubber/Event/Event.h"
#include "Rubber/Debug/Profiler.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include "Rubber/Utility/Utility.h"
#include <functional>

namespace Rubber {

	namespace{

		constexpr inline const uint32_t MIN_TO_CLEANUP = 50;
	}

	// TODO: MAY BE USE A DEQUE INSTEAD OF VECTOR 
	template <typename T>
	class EventBus{

	public:
		using EventHandler = typename std::function<bool(const T& event)>;

		static void subscribe(std::string_view name, EventHandler&& handler){
			RB_PROFILE_FUNC();
			auto& callBacks = getCallBacks();
			auto& indexMap = getIndexMap();

			uint64_t hashVal = std::hash<std::string_view>{}(name);
			RB_CORE_ASSERT(indexMap.find(hashVal) == indexMap.end(), "The hanlder with this name is already registered!");

			std::size_t index = callBacks.size(); 
			callBacks.emplace_back(std::move(handler),std::string(name), hashVal);

			RB_CORE_ASSERT(callBacks.size() == index + 1, "handler successfully placed!");
			indexMap.emplace(hashVal, index);
		}

		static void unsubscribe(std::string_view name){
			RB_PROFILE_FUNC();
			auto& callBacks = getCallBacks();
			auto& indexMap = getIndexMap();

			uint64_t hashVal = std::hash<std::string_view>{}(name);
			RB_CORE_ASSERT(indexMap.find(hashVal) != indexMap.end(), "the handler's name does not exists!");

			auto index = indexMap[hashVal];
			RB_CORE_ASSERT(callBacks[index].m_Name == name, "naming conflict!");

			callBacks[indexMap[hashVal]].m_IsAlive = false;

			uint32_t deadCount =accumulateDeadCount();
			uint64_t numsHanlders = callBacks.size();
			if (numsHanlders > MIN_TO_CLEANUP && (double)deadCount / (double)numsHanlders > 0.5){
				Vector<HandlerData>  newBuffer;
				newBuffer.reserve(numsHanlders);

				for (auto& it : callBacks) {
					if (it.m_IsAlive){
						indexMap.insert_or_assign(it.m_HashKey, newBuffer.size());
						newBuffer.emplace_back(std::move(it));
					}else{
						indexMap.erase(it.m_HashKey);
					}
				}
				RB_CORE_ASSERT(newBuffer.size() == numsHanlders - deadCount, "something wrong happenede !");
				callBacks.swap(newBuffer);
				deadCount = 0;
				RB_CORE_ASSERT(callBacks.size() == numsHanlders - deadCount, "something wrong happenede !");
			}
		}

		static void processAllHandlers(const T& e){
			RB_PROFILE_FUNC();
			for(auto& it : getCallBacks()){
				RB_INFO("current processing handler: {}", it.m_Name);
				if (it.m_Handler(e))
					break;
			}
		}

		static void clear(){
			getCallBacks().clear();
			getIndexMap().clear();
		}
		
	private:
		struct HandlerData {
			bool m_IsAlive;
			uint64_t m_HashKey;
			EventHandler m_Handler;
			std::string m_Name; //debug purpose

			HandlerData(EventHandler&& handler, std::string&& name, uint64_t hashKey, bool isAlive = true)
				: m_Handler(std::move(handler)), 
				m_Name(name),m_HashKey(hashKey), 
				m_IsAlive(isAlive) {};

			HandlerData(HandlerData&& src) noexcept {
				this->m_IsAlive = src.m_IsAlive;
				this->m_HashKey = src.m_HashKey;
				this->m_Handler = std::move(src.m_Handler);
				this->m_Name = std::move(src.m_Name);
			}
		};

		 static Vector<HandlerData>& getCallBacks() {
			 static Vector<HandlerData> s_AllCallBacks;
			 return s_AllCallBacks;
		 }

		 static std::unordered_map <uint64_t, uint64_t>& getIndexMap(){
			 static std::unordered_map<uint64_t, uint64_t> s_indexMap;
		     return s_indexMap;
		 }

		 static uint32_t& accumulateDeadCount(){
			 static uint32_t deadCount = 0;
			 deadCount++;
			 return  deadCount;
		 }
	};

}

