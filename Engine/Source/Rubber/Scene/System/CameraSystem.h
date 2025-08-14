#pragma  once 
#include <entt.hpp>
#include "Rubber/Scene/Utili/Component.h"
#include "Rubber/Scene/System/System.h"

namespace Rubber{

	class CameraSystem : public SystemBase {
	public:	
		CameraSystem() = default;

		void onUpdate(const float ts) override; 
	};
}