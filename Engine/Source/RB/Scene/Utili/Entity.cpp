#include <pch.h>
#include "Entity.h"
#include "RB/Scene/Scene.h"


namespace RB{

	Entity::Entity(entt::entity entity, const Ref<Scene>& activeScence)
		:m_EntityHandler(entity), m_Scene(activeScence)
	{

	}

}

