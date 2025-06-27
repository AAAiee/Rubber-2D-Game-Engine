#include <pch.h>

#include "Registry.h"

namespace Rubber{

	template<> TransformStorage& Registry::assure<TransformStorage>(){
		return  m_Transform;
	}



    void Registry::flush(){
		for(Entity e : m_DestroyQueue){
			if (m_Transform.contains(e)) {
				m_Transform.removeComponent(m_Transform.lookup(e).index);
			}
			m_Em.removeEntity(e.id);
		}

		m_DestroyQueue.clear();
	}

}