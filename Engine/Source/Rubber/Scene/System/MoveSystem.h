#pragma once
#include "Rubber/Scene/System/System.h"


namespace Rubber {

	class MoveSystem : public SystemBase
	{
	public:
		void onUpdate(float ts) override;
	};


}
