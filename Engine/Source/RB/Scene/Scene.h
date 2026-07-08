#pragma once  
#include <entt.hpp>
#include <memory>
#include "RB/Scene/Utili/Entity.h"  
#include "RB/Scene/System/System.h"


namespace RB {  

	class SceneHierachyPanel;

	class Scene : public std::enable_shared_from_this<Scene> {  
	public:
		void systemsInit();

		void systemShutDow();

		Entity createEntity(std::string_view tag="");  

		void onSystemsUpdate(const float ts);  

		void addSystem(Scope<SystemBase> system);

		inline entt::registry& getRegistry() {
			return m_Registry;
		}

		~Scene() = default;

	private:
		Scene() =default;

	public:  
		static Ref<Scene> create();  

	private:  
		entt::registry m_Registry;
		Vector<Scope<SystemBase>>  m_Systems;

		friend class Entity;
		friend class SceneHierachyPanel;
	};  

}
