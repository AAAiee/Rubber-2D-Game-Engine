#pragma once

#include "Rubber/Scene/System/System.h"
#include "Rubber/Scene/Utili/Component.h"


namespace Rubber {

	template<isStateEnum StateEnum>
	class FsmPostMovingSystem : public SystemBase {
	public:
		void onUpdate(float ts) override;
	};

	template<isStateEnum StateEnum>
	void Rubber::FsmPostMovingSystem<StateEnum>::onUpdate(float ts)
	{

		auto view = m_Registry->view<StateMachineComponent<StateEnum>>();
		for (auto e : view) {
			auto& fsm = view.get<StateMachineComponent<StateEnum>>(e);
			for (auto& t : fsm.transitions) {
				if (fsm.currentState == t.from && t.condition(Entity{e,m_ScenePtr})) {
					if (fsm.onExit)  fsm.onExit(e, fsm.currentState);
					fsm.previousState = fsm.currentState;
					fsm.currentState = t.to;
					fsm.elapsedTime = 0.0f;
					fsm.justEntered = true;
					break;
				}
			}
		}
	}

}
