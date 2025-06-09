#pragma once
#include "Rubber/ECS/EcsCommonHeaders.h"

namespace Rubber {

	class EntityManager {

	public:
		Entity createEntity();

		void removeEntity(EntityID id);
		// if this id has the same generation with the generation val for the slot, it is alive 
		bool is_alive(EntityID id) const;


	private:
		// update when a entity is dead
		Utility::deque<ID::IdType>  m_FreeIndices;
		// keep track of our index and generation  for a specific entity slot
		Utility::vector<ID::GenerationType> m_Generations;
	};
}
