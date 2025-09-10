#include <pch.h>
#include "Entity.h"
#include "Rubber/Scene/Scene.h"


namespace Rubber{

	Entity::Entity(entt::entity entity, Scene* activeScence)
		:m_EntityHandler(entity), m_Scene(activeScence)
	{
	}

}

