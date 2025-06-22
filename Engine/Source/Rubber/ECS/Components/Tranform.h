#pragma once
#include <Rubber/ECS/EcsCommonHeaders.h>

namespace Rubber {
	class EntityManager; // Forward declaration to resolve the error  
}
namespace Rubber::Component{
	
	//TODO::MAKE SURE YOU HAVE QUAT AS ORIENTATION
	class TransformManager{

	public:
		TransformManager();

		TransformManager(uint32_t size);

		~TransformManager();

		void allocate(uint32_t newCapacity);

		void add(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, Entity owner);

		void removeComponent(uint32_t index );

		void simulate();

		Instance lookup(Entity e);
		
	
		// randomly check died entity and destroy them, stop until
		// we meets four alive entities in a row, otherwise keeps delete
		void garbageCollector(const Rubber::EntityManager& em);


	private:
		//TODO:: can be further optimized by break vec3 into 3 differnet component, for simd optimization?
		struct instanceData {
			uint32_t n = 0; // number of used instances;
			uint32_t allocated = 0; // currently allocated;
			void* buffer = nullptr; // a buffer that contains all position, scale and rotations

			Entity* Entities = nullptr;
			glm::vec3* position = nullptr;
			glm::vec3* scale = nullptr;
			glm::quat* oritation = nullptr;
		};

		instanceData m_Data;

		Vector<uint32_t> m_Map;
	};

}

