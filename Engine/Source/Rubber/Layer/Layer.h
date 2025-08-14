#pragma once
#include <Rubber/Core/Core.h>
#include <string>

namespace Rubber
{
	class Event;
	class EventManager;
	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() = 0;
		virtual void onDetach()  = 0;
		virtual void onUpdate(float ts) {};
		virtual void onImGuiRender() {};

		inline const std::string& getName() const {
			return m_DebugName;
		}

	private:
		// name for debug purpose
		std::string m_DebugName;
	};

}
