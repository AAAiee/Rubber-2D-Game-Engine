#include <pch.h>
#include "EventManager.h"



namespace Rubber{

	void EventManager::dispatchAllEvents()
	{
		RB_PROFILE_FUNC();
		for (auto& fn : m_CurrentQueue) {
			fn();
		}
		std::swap(m_CurrentQueue, m_NextQueue);
		m_NextQueue.clear();
	}

	Ref<Rubber::EventManager> EventManager::create()
	{
		static Ref<EventManager> instance = nullptr;
		if (!instance) {
			instance = Ref<EventManager>(new EventManager());
		}
		return instance;
	}


}
