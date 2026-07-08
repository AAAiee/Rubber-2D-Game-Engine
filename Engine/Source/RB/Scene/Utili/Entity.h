#pragma once
#include <entt.hpp>
#include <RB/Utility/Utility.h>

namespace RB{
	class Scene;

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entity, const Ref<Scene>& activeScence);

		template<typename T>
		bool hasComponent(){
			RB_CORE_ASSERT(m_Scene.lock(), "Entity must live in a valid scene!");
			return m_Scene.lock()->m_Registry.all_of<T>(m_EntityHandler);
		}


		template <typename T, typename... Args>
		T& addComponent(Args&&... args){
			RB_CORE_ASSERT(m_Scene.lock(), "Entity must live in a valid scene!");
			return m_Scene.lock()->m_Registry.emplace<T>(m_EntityHandler, std::forward<Args>(args)...);
		}


		template <typename T>
		void removeComponent(){
			RB_CORE_ASSERT(m_Scene.lock(), "Entity must live in a valid scene!");
			m_Scene.lock()->m_Registry.remove(m_EntityHandler);
		}


		template<typename T>
		T& getComponent(){
			RB_CORE_ASSERT(m_Scene.lock(), "Entity must live in a valid scene");
			return m_Scene.lock()->m_Registry.get<T>(m_EntityHandler);
		}

		Ref<Scene> getScene() {
			return m_Scene.lock();
		}

		operator uint32_t () const  {
			return  (uint32_t)m_EntityHandler;
		}

		operator bool() const {
			return m_EntityHandler != entt::null && !m_Scene.expired() ;
		}
		
		bool operator ==(const Entity& other) const {
			return  m_EntityHandler == other.m_EntityHandler && m_Scene.lock() == other.m_Scene.lock();
		}

		bool operator != (const Entity& other) const {
			return  !((*this) == other);
		}

	private:
		entt::entity m_EntityHandler{ entt::null };
		weakRef<Scene>  m_Scene;
	};

}

