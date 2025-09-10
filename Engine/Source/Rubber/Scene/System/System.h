#pragma once
#include <entt.hpp>


namespace Rubber {
	class Scene;

	class SystemBase {
	public:
		virtual void init(Scene* scene);
		virtual ~SystemBase() {};

	    SystemBase() {};

		virtual void shutdown();

		virtual void onUpdate(float ts);
		
		virtual bool hasInit() const { return m_HadInit; }

   protected:
	   Scene* m_ScenePtr = nullptr;
	   entt::registry* m_Registry = nullptr;
	   bool m_HadInit = false;
	};
}
