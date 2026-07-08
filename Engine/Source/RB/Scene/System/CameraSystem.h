#pragma  once 
#include <entt.hpp>
#include "RB/Scene/Utili/Component.h"
#include "RB/Scene/System/System.h"

namespace RB{

	class CameraSystem : public SystemBase {
	public:	
		CameraSystem() = default;

		void onUpdate(const float ts) override; 
	};
}