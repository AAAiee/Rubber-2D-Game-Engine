#include<pch.h>
#include "Tranform.h"
#include <random>
#include <glm/gtc/quaternion.hpp>
#include <Rubber/ECS/Entity/EntityManager.h>


namespace{

	std::random_device rd;
	std::mt19937 randomEngine{rd()};
	std::uniform_int_distribution<uint32_t>  dist {0, Rubber::ID::inValidIndex - 1};
}

namespace Rubber{


	void Rubber::Component::TransformManager::garbageCollector(const Rubber::EntityManager& em)
	{
		int32_t aliveInRow = 0;
		uint32_t n;
		while ((n = m_Data.n) > 0 && aliveInRow < 4) {
			uint32_t randomIndex = dist(randomEngine) % n;
			if (em.is_alive(this->m_Data.Entities[randomIndex].id)){
				aliveInRow++;
				continue;
			}

			aliveInRow = 0;
			removeComponent(randomIndex);
		}
	}

	Rubber::Component::Instance Rubber::Component::TransformManager::lookup(Entity e)
	{
		return  makeInstance(this->m_Map[e.id]);
	}

	void Rubber::Component::TransformManager::simulate()
	{
		//TODO::

	}

	void Rubber::Component::TransformManager::removeComponent(uint32_t index)
	{
		uint32_t last = m_Data.n - 1;
		RB_CORE_ASSERT(index <= last, "Delete things that are not existed");

		Entity e = this->m_Data.Entities[index];
		Entity lastE = this->m_Data.Entities[last];

		if (index != last) {
			m_Data.Entities[index] = m_Data.Entities[last];
			m_Data.position[index] = m_Data.position[last];
			m_Data.scale[index] = m_Data.scale[last];
			m_Data.oritation[index] = m_Data.oritation[last];

			this->m_Map[lastE.id] = index;
		}

		this->m_Map[e.id] = ID::invalidId;
		m_Data.n--;
	}

	void Rubber::Component::TransformManager::add(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, Entity owner)
	{
		if (this->m_Data.n >= this->m_Data.allocated) {
			uint32_t resizeTo = (this->m_Data.allocated == 0) ? 16 : this->m_Data.allocated * RESCALE_FACTOR;
			allocate(resizeTo);
		}

		// make sure the entity's id now gives correct index to the correct transform instance
		this->m_Map[owner.id] = this->m_Data.n;

		// add data over;
		this->m_Data.Entities[this->m_Data.n] = owner;
		std::memcpy(this->m_Data.position + this->m_Data.n, &position, sizeof(glm::vec3));
		std::memcpy(this->m_Data.scale + this->m_Data.n, &scale, sizeof(glm::vec3));
		std::memcpy(this->m_Data.oritation + this->m_Data.n, &rotation, sizeof(glm::vec3));

		std::cout << sizeof(this->m_Data.buffer);
		this->m_Data.n++;
	}

	void Rubber::Component::TransformManager::allocate(uint32_t newCapacity)
	{
		RB_CORE_ASSERT(newCapacity > this->m_Data.n, "No need to resize !");

		instanceData newInstanceData;
		newInstanceData.n = this->m_Data.n;
		newInstanceData.allocated = newCapacity;
		const uint32_t byteToAllocate = ((sizeof(glm::vec3) * 2  + sizeof(glm::quat)) *  newCapacity) + (sizeof(Entity) * newCapacity);

		// TODO::REPLACE WITH CUSTOMER ALLOCATOR
		newInstanceData.buffer = operator new(byteToAllocate, std::align_val_t(sizeof(glm::quat)));

		// make each pointer points to their belonged position
		newInstanceData.Entities = reinterpret_cast<Entity*>(newInstanceData.buffer);
		newInstanceData.position = reinterpret_cast<glm::vec3*> (newInstanceData.Entities + newCapacity);
		newInstanceData.scale = newInstanceData.position + newCapacity;
		newInstanceData.oritation = reinterpret_cast<glm::quat*> (newInstanceData.scale + newCapacity);

		//copy old data over
		std::memcpy(newInstanceData.Entities, this->m_Data.Entities, sizeof(Entity) * this->m_Data.n);
		std::memcpy(newInstanceData.position, this->m_Data.position, sizeof(glm::vec3) * this->m_Data.n);
		std::memcpy(newInstanceData.scale, this->m_Data.scale, sizeof(glm::vec3) * this->m_Data.n);
		std::memcpy(newInstanceData.oritation, this->m_Data.oritation, sizeof(glm::quat) * this->m_Data.n);

		operator delete(this->m_Data.buffer, std::align_val_t(sizeof(glm::quat)));
		this->m_Data = newInstanceData;

		this->m_Map.resize(newCapacity, ID::inValidIndex);
	}

	Rubber::Component::TransformManager::~TransformManager()
	{
		operator delete(m_Data.buffer, std::align_val_t(sizeof(glm::quat)));
	}

	Rubber::Component::TransformManager::TransformManager(uint32_t size) :m_Map(size, 0)
	{
		RB_CORE_ASSERT(size != 0, "size can not be zero!");
		allocate(size);
	}

	Rubber::Component::TransformManager::TransformManager() :m_Map(10, 0)
	{
		allocate(10);
	}


}// namespace Rubber
