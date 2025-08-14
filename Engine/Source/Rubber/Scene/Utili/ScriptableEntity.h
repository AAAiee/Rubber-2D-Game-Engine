#pragma once
#include<Rubber/Scene/Utili/Entity.h>
#include<entt.hpp>

namespace Rubber{

	class ScriptableEntity {
	public:	
		ScriptableEntity() = default;
		virtual ~ScriptableEntity() = default;

		template <typename T>
		T& getComponent(){
			return  m_Entity.getComponent<T>();
		}

		virtual void onCreate() {};
		virtual void onUpdate(float ts) {};
		virtual void onDestory() {};

	public:
		Entity m_Entity;
	};

}