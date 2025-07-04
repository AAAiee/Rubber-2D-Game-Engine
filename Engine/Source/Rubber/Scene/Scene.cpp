#include <pch.h>
#include "Scene.h"
#include "Rubber/Scene/Utili/Component.h"
#include "Rubber/Renderer/Renderer2D.h"

namespace Rubber{

	SystemList::SystemList() = default;

	Scene::Scene()
	{
		// init all system;
		std::apply([this](auto&... system) {
			(system.init(m_Registry), ...);
			}, m_Systems.allSystem);
	}

	Scene::~Scene()
	{
		std::apply([this](auto&... system) {
			(system.shutdown(), ...);
			}, m_Systems.allSystem);

	}

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

    void Scene::onSceneUpdate(const float ts)  
    {  
		std::apply([this,ts](auto&... system) {

			(system.onUpdate(ts),...);

			}, m_Systems.allSystem);
    }

	Rubber::Ref<Rubber::Scene> Scene::create()
	{
		return makeRef<Scene>();
	}


}

