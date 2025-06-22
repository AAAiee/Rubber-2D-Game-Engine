#include <pch.h>
#include "Rubber/ECS/Entity/EntityManager.h"

Rubber::Entity Rubber::EntityManager::EntityManager::createEntity()
{
	EntityID  newEntityID;
	//case 1:: when freed at least 1024 entities, reuse entity slot
	if (this->m_FreeIndices.size() > ID::minDeletedElements) {
		newEntityID = EntityID{ m_FreeIndices.front() };
		//double check to make sure the index is free now , do we really need this?
		RB_CORE_ASSERT(!is_alive(newEntityID), "This index corresponds to a entity that is still alive!");
		this->m_FreeIndices.pop_front();

		newEntityID = EntityID{ ID::newGeneration(newEntityID)}; //  newEntityId will be invalid f after generation exceeds limit
		// we know for sure index is valid 
		ID::IdType index = ID::indexOf(newEntityID);
		this->m_Generations[index] = static_cast<ID::GenerationType> (ID::generationOf(newEntityID));
	}
	else {
	    ID::IdType index = static_cast<ID::IdType>(this->m_Generations.size());
		RB_CORE_ASSERT(ID::isValidIndex(index), "Entity Index exceeds the limit!");

		// if index is not valid, simply return a invalid entity ( an entity with invalid index)
		if (!ID::isValidIndex(index)){
			newEntityID = static_cast<EntityID> (ID::invalidId);
		}else{
			this->m_Generations.push_back(ID::GenerationType(0));
			newEntityID = EntityID(index);
		}
	}
	Entity newEntity{ newEntityID };
	return newEntity;
}
bool Rubber::EntityManager::EntityManager::is_alive(EntityID  id) const
{
	RB_CORE_ASSERT(id != ID::invalidId, "id is not valid !");
	ID::IdType index = ID::indexOf(id);
	if (index >= this->m_Generations.size()){
		return false;
	}

	ID::GenerationType geneariton = ID::generationOf(id);
	return geneariton == this->m_Generations[index];
}

void Rubber::EntityManager::EntityManager::removeEntity(EntityID  id)
{
	// check the id of the entity
	RB_CORE_ASSERT(id != ID::invalidId, "id is not valid !");
	RB_CORE_ASSERT(is_alive(id), "The entity is not alive, already removed!");

	// if it is then push it to the free_ids for reuse
	this->m_FreeIndices.push_back(id);
	++m_Generations[id];
}
