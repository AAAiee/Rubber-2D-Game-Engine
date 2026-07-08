#pragma  once 
#include <entt.hpp>
#include "System.h"
#include "RB/Utility/Utility.h"

namespace RB {
	class EventManager;
	class ViewPortResizeEvent;

	class WindowSystem : public SystemBase
	{
	public:
		WindowSystem() = default;
		void init(Ref<Scene>  scene) override;
		void shutdown() override;
		
	private:
		bool onViewPortResize(const ViewPortResizeEvent& e);

	private:
		Ref<EventManager> m_EventManegr;
	};
}