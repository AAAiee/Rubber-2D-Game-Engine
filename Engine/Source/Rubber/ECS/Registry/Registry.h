#pragma once
#include "Rubber/ECS/EcsCommonHeaders.h"
#include "Rubber/ECS/Entity/EntityManager.h"
#include "Rubber/ECS/Components/TransoformStorage.h"
#include "view.h"


namespace Rubber{


	class Registry{

	public:

		inline Entity CreateEntity() { return m_Em.createEntity(); }
		inline void destroy(Entity e) { m_DestroyQueue.push_back(e); }
		inline bool alive(Entity e) { return m_Em.is_alive(e.id); }

		void flush();

		template<typename Storage, typename... Args>
		void emplace(Entity entity, Args&&... args) {
			assure<Storage>().add(entity, std::forward<Args>(args)...);
		}

		template<class Storage>
		bool has(Entity e){
			return assure<Storage>().contains(e);
		}

		template<class Storage>
		Storage& getStorage() {
			return assure<Storage>();
		}

		//view
		template <class Storage>
		SimpleView<Storage> view(){
			return SimpleView<Storage>(assure<Storage>());
		}

	private:
		EntityManager	m_Em;
		Vector<Entity>  m_DestroyQueue;

		TransformStorage m_Transform;

		template <class Storage>
		Storage& assure();
	};

	
}