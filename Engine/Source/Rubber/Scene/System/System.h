#pragma once
#include <entt.hpp>


namespace Rubber {
	class Scene;

	class SystemBase {
	public:
		virtual void init(Ref<Scene> scene);
		virtual ~SystemBase() {};

	    SystemBase() {};

		virtual void shutdown();

		virtual void onUpdate(float ts);

   protected:
	   Ref<Scene> m_ScenePtr = nullptr;
	   entt::registry* m_Registry = nullptr;
	   bool m_HadInit = false;
	};
}
