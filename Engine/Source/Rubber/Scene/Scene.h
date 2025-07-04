#pragma once  
#include <entt.hpp>
#include <memory>
#include "Rubber/Scene/Utili/Entity.h"  
#include "Rubber/Scene/System/CameraSystem.h"
#include "Rubber/Scene/System/RenderSystem.h"
#include "Rubber/Scene/System/ScriptSystem.h"


namespace Rubber {  

	struct SystemList {
		SystemList();
		std::tuple<ScriptSystem, CameraSystem, RendererSystem> allSystem;
	};


	class Scene : public std::enable_shared_from_this<Scene> {  
	public:  
		Scene();
		~Scene();

		Entity createEntity(std::string_view tag="");  
		void onSceneUpdate(const float ts);  

	public:  
		static Ref<Scene> create();  

	private:  
		entt::registry m_Registry;
		friend class Entity;  
		SystemList m_Systems;
	};  

}
