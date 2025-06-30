#include <pch.h>
#include "EventManager.h"



namespace Rubber{

	void EventManager::flush()
	{
		RB_PROFILE_FUNC();
		for (auto& fn : m_EventQueue) {
			fn();
		}
		m_EventQueue.clear();
	}

	Ref<Rubber::EventManager> EventManager::create(uint32_t size)
	{
		return makeRef<EventManager>(size);
	}

	EventManager::EventManager(uint32_t capacity)
	{
		m_EventQueue.reserve(capacity);
	}

}
