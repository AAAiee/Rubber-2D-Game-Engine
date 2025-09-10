#pragma once  
#include "Rubber/Scene/System/System.h"

#include <entt.hpp>
#include<glm/glm.hpp>
#include <memory>
#include <string_view>


namespace Rubber {  

	class SceneHierachyPanel;
	class Entity;

	class Scene {  
	public:
		Entity createEntity(std::string_view tag="");  
		
		void removeEntity(Entity entity);

		entt::registry& getRegistry();
		void updatesViewportSize(glm::vec2 dimensions);

		void publishViewportResize();

		~Scene();

		void onSceneUpdate(float ts);

	    void systemsInit();
	    void systemShutDown();
	    void onSystemsUpdate(float ts);


	private:
		Scene();

		template<typename T>
		void onComponentConstruct(Entity ent);

		template<typename T>
		void onComponentDelete(Entity ent);

	public:  
		static Ref<Scene> create();  

	private:  
		entt::registry m_Registry;
		Vector<Scope<SystemBase>> m_Systems;
		glm::vec2 m_ViewPortDimension = { 1600.0f, 900.0f };

		friend class Entity;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};  

	

}
