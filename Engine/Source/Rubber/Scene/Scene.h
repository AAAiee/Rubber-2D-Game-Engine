#pragma once  
#include <entt.hpp>
#include<glm/glm.hpp>
#include <memory>
#include "Rubber/Scene/Utili/Entity.h"  
#include "Rubber/Scene/System/System.h"


namespace Rubber {  

	class SceneHierachyPanel;

	class Scene : public std::enable_shared_from_this<Scene> {  
	public:
		void systemsInit();

		void systemShutDown();

		Entity createEntity(std::string_view tag="");  
		
		void removeEntity(Entity entity);

		void onSystemsUpdate(const float ts);  

		void addSystem(Scope<SystemBase> system);

		inline entt::registry& getRegistry() {
			return m_Registry;
		}

		~Scene() = default;

		void updatesViewportSize(glm::vec2 dimensions);

	private:
		Scene() =default;
		void publishViewportResize();

	private:
		template<typename T>
		void onComponentConstruct(Entity ent);

		template<typename T>
		void onComponentDelete(Entity ent);

	public:  
		static Ref<Scene> create();  

	private:  
		entt::registry m_Registry;
		Vector<Scope<SystemBase>>  m_Systems;
		bool m_isSystemsInit = false;

		glm::vec2 m_ViewPortDimension;
		friend class Entity;
		friend class SceneHierachyPanel;
	};  

	

}
