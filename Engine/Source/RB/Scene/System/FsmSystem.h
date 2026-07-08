#pragma once
#include <entt.hpp>
#include "RB/Scene/Utili/Component.h"

namespace RB {

	template <isStateEnum StateEnum>
	class FsmSystem : public SystemBase{
	public:

		void onUpdate(float ts) override;
	};

	//implementation
	template <isStateEnum StateEnum>
	void FsmSystem<StateEnum>::onUpdate(float ts)
	{

		auto fsmView = m_Registry->view<StateMachineComponent<StateEnum>>();
		for (entt::entity e : fsmView) {
			StateMachineComponent<StateEnum>& fsmCC = fsmView.get<StateMachineComponent<StateEnum>>(e);

			fsmCC.elapsedTime += ts;

			if (fsmCC.justEntered) {
				if (fsmCC.onEnter) fsmCC.onEnter(e, fsmCC.currentState);
				fsmCC.justEntered = false;
			}

			if (fsmCC.onUpdate) fsmCC.onUpdate({ e, m_ScenePtr }, fsmCC.currentState,  ts);
		}

	}

}
