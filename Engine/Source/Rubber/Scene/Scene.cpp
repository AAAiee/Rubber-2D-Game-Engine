#include <pch.h>
#include "Scene.h"
#include "Rubber/Scene/Utili/Component.h"
#include "Rubber/Core/Application.h"
#include "Rubber/Event/EventManager.h"
#include "RUbber/Event/AppEvent.h"
#include "Rubber/Renderer/Renderer2D.h"

namespace Rubber {

	void Scene::systemsInit()
	{
		if (m_isSystemsInit) {
			return;
		}

		m_isSystemsInit = true;

		for (auto& systemPtr : m_Systems) {
			systemPtr->init(shared_from_this());
		}
	}

	void Scene::systemShutDown()
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

	void Scene::removeEntity(Entity entity)
	{
		m_Registry.destroy(entity);
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

	void Scene::updatesViewportSize(glm::vec2 dimensions)
	{
		RB_CORE_ASSERT(dimensions.x * dimensions.y > 0.0f, "invalid dimensions");
		m_ViewPortDimension = dimensions;
	}

	Ref<Scene> Scene::create()
	{
		return Ref<Scene>(new Scene());
	}


	template<typename T>
	void Rubber::Scene::onComponentConstruct(Entity ent)
	{
		static_assert(false);
	}


	template<>
	void Rubber::Scene::onComponentConstruct<TagComponent>(Entity ent)
	{
		// do something
	}


	template<>
	void Rubber::Scene::onComponentConstruct<SpriteComponent>(Entity ent)
	{
		// do something
	}

	template<>
	void Rubber::Scene::onComponentConstruct<CameraComponent>(Entity ent)
	{
		publishViewportResize();
	}

	template<>
	void Rubber::Scene::onComponentConstruct<TransformComponent>(Entity ent)
	{
	}

	template<>
	void Rubber::Scene::onComponentConstruct<VisibilityControlComponent>(Entity ent)
	{
	}



	template<typename T>
	void Scene::onComponentDelete(Entity ent)
	{
		static_assert(false);
	}

	template<>
	void Rubber::Scene::onComponentDelete<TagComponent>(Entity ent)
	{
		// do something
	}


	template<>
	void Rubber::Scene::onComponentDelete<SpriteComponent>(Entity ent)
	{
		// do something
	}

	template<>
	void Rubber::Scene::onComponentDelete<TransformComponent>(Entity ent)
	{
		// do something
	}

	template<>
	void Rubber::Scene::onComponentDelete<VisibilityControlComponent>(Entity ent)
	{
		// do something
	}

	template<>
	void Rubber::Scene::onComponentDelete<CameraComponent>(Entity ent)
	{
		publishViewportResize();
	}


	void Scene::publishViewportResize()
	{
		Ref<EventManager> eventManager = Application::getEventManager();
		// push an viewport resize event whenever a new camera component is added(to be handled by the WindowSystem)
		eventManager->enqueue<ViewPortResizeEvent>({ (uint32_t)m_ViewPortDimension.x, (uint32_t)m_ViewPortDimension.y });
	}




	



}

