#pragma once
#include <Rubber/Core.h>
#include <string>

namespace Rubber
{
	class Event;
	class RB_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();


		virtual void onAttach() {};
		virtual void onDetach() {};
		virtual void onUpdate() {};
		virtual void onEvent(Event& event) {};

		inline const std::string& getName() const {
			return m_DebugName;
		}

	private:
		// name for debug purpose
		std::string m_DebugName;
	};

}
