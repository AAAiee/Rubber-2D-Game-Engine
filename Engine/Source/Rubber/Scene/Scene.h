#pragma once  
#include <entt.hpp> 
#include <memory>
#include <Rubber/Scene/Entity.h>  

namespace Rubber {  

class Scene : public std::enable_shared_from_this<Scene> {  
public:  
	Scene() = default;
	Entity createEntity(std::string_view tag="");  

	void onSceneUpdate();  


public:  
	static Ref<Scene> create();  

private:  
	entt::registry m_Registry;  
	friend class Entity;  
};  

}
