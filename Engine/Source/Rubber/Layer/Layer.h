#pragma once
#include <Rubber/Core/Core.h>
#include <string>

namespace Rubber
{
	class Event;
	class EventManager;
	class RB_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach(const Ref<EventManager>& em = {}) = 0;
		virtual void onDetach()  = 0;
		virtual void onUpdate(const float ts) {};
		virtual void onImGuiRender() {};

		inline const std::string& getName() const {
			return m_DebugName;
		}

	private:
		// name for debug purpose
		std::string m_DebugName;
	};

}
