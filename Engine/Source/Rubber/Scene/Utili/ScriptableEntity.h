#include<Rubber/Scene/Utili/Entity.h>

namespace Rubber{

	class ScriptableEntity {
	public:	
		ScriptableEntity()          = default;
		virtual ~ScriptableEntity() = default;

		template <typename T>
		T& getComponent(){
			return m_Entity.getComponent<T>();
		}

		virtual void onCreate() {};
		virtual void onUpdate(const float ts) {};
		virtual void onDestory() {};

	private: 
		Entity m_Entity;
	};

}