#pragma once

#include <Rubber/Core.h>
#include <Rubber/Event/Event.h>

namespace Rubber
{
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

		std::string m_DebugName;
	};
		
}
