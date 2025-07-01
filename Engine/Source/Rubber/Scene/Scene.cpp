#include <pch.h>
#include "Rubber/Scene/Component.h"
#include "Scene.h"
#include "Rubber/Renderer/Renderer2D.h"

namespace Rubber{

    Entity Scene::createEntity(std::string_view tag)
	{
		entt::entity entityHandler =  m_Registry.create();
		auto me = shared_from_this();
		Entity entity(entityHandler, me);


		//every entity has a default tag component
		std::string_view tagText = (tag.empty()) ? "Unknown Entity" : tag;
		entity.addComponent<TagComponent>(tagText);
		return entity;
	}

    void Scene::onSceneUpdate()  
    {  
		Renderer2D::beginScene(Camera(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f)));
       auto view = m_Registry.view<TransformComponent, SpriteComponent>();
       for (auto entity : view)  
       {  
           auto& transform = view.get<TransformComponent>(entity);  
           auto& sprite = view.get<SpriteComponent>(entity);  

           Renderer2D::drawColorQuad(transform.transform, sprite.color);  
       }  

	   Renderer2D::endScene();
    }

	Rubber::Ref<Rubber::Scene> Scene::create()
	{
		return makeRef<Scene>();
	}

}

