#pragma once
#include "RB/Scene/System/System.h"


namespace RB {

	class MoveSystem : public SystemBase
	{
	public:
		void onUpdate(float ts) override;
	};


}
