#include <pch.h>
#include "Scene.h"
#include "Rubber/Scene/Utili/Component.h"
#include "Rubber/Renderer/Renderer2D.h"

namespace Rubber{



	void Scene::systemsInit()
	{
		if (m_isSystemsInit) {
			return;
		}

		for (auto& systemPtr : m_Systems) {
			systemPtr->init(shared_from_this());
		}
	}

	void Scene::systemShutDow()
	{

		RB_CORE_ASSERT(m_isSystemsInit, "Systems are never initted");

		for (auto& systemPtr : m_Systems) {
			systemPtr->shutdown();
		}
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

    void Scene::onSystemsUpdate(float ts)  
    {  

		RB_CORE_ASSERT(m_isSystemsInit, "Systems are not initted yet!");
		for (auto& systemPtr : m_Systems) {
			systemPtr->onUpdate(ts);
		}
    }

	void Scene::addSystem(Scope<SystemBase> system)
	{
		m_Systems.push_back(std::move(system));
	}

	Ref<Scene> Scene::create()
	{
		return Ref<Scene>(new Scene());
	}


}

