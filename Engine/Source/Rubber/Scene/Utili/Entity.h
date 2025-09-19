#pragma once
#include <entt.hpp>
#include <Rubber/Core/Utility.h>
#include "Rubber/Scene/Scene.h" 

namespace Rubber{

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene*  activeScence);

		template<typename T>
		bool hasComponent(){
			RB_CORE_ASSERT(m_Scene, "Entity must live in a valid scene!");
			if (m_EntityHandler == entt::null || !m_Scene->m_Registry.valid(m_EntityHandler))
				return false;
			return m_Scene->m_Registry.all_of<T>(m_EntityHandler);
		}


		template <typename T, typename... Args>
		T& addComponent(Args&&... args){
			RB_CORE_ASSERT(m_Scene, "Entity must live in a valid scene!");
			m_Scene->template onComponentConstruct<T>(*this);
			return m_Scene->m_Registry.template emplace<T>(m_EntityHandler, std::forward<Args>(args)...);
		}


		template <typename T>
		void removeComponent(){
			RB_CORE_ASSERT(m_Scene, "Entity must live in a valid scene!");
			m_Scene->template onComponentDelete<T>(*this);
			m_Scene->m_Registry.remove<T>(m_EntityHandler);
		}


		template<typename T>
		T& getComponent(){
			RB_CORE_ASSERT(m_Scene, "Entity must live in a valid scene");
			return m_Scene->m_Registry.get<T>(m_EntityHandler);
		}

		Scene* getScene() {
			return m_Scene;
		}

		operator uint32_t () const  {
			return  (uint32_t)m_EntityHandler;
		}

		operator bool() const {
			return m_EntityHandler != entt::null && m_Scene != nullptr;
		}
		
		operator entt::entity() const {
			return m_EntityHandler;
		}

		operator uintptr_t() const {
			return (uintptr_t)m_EntityHandler;
		}
		
		bool operator ==(const Entity& other) const {
			return  m_EntityHandler == other.m_EntityHandler && m_Scene == other.m_Scene;
		}

		bool operator != (const Entity& other) const {
			return  !((*this) == other);
		}

		

	private:
		entt::entity m_EntityHandler{ entt::null };
		Scene*  m_Scene = nullptr;
	};

}

