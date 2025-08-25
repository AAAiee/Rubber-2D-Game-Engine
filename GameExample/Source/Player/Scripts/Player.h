#pragma once  
#include "Rubber.h"  
#include <unordered_map>

#include "Player/PlayerState.h"
#include "Player/PlayerAttackDir.h"
#include "ECS/GameComponents/GameComponents.h"


class Player final : public RB::ScriptableEntity {

public:
	void onCreate() override;
	void onUpdate(float ts) override;
	void onDestory() override;
    ~Player() override;

private:
	void onIdleEnter();
	void onRunEnter();
	void onJumpEnter();
	void onFallEnter();
	void onRollEnter();
	void onDeadEnter();

	void onIdleExit() {};
	void onRunExit() {};
	void onJumpExit();;
	void onFallExit() ;
	void onRollExit() {};
	void onAttackEnter();
	void onAttackExit();

private:
	void decreaseHP(RB::Entity ent);
	void makeInvulnerable();

private:
	void stateMachineInit();
	void animationPoolInit();
	AttackDir getAttackDir(glm::vec2 mouseCoord); 

private:

	const float m_RunningSpeed = 2.0f;
	const float m_jumpImpulsSpeed = 6.0f;
	const float m_RollImpulseSpeed = 4.0f;

	AttackDir m_AttackDir = AttackDir::NONE;

	//invisible status
	RB::AnimationTimer  m_TimerInvulnerableBlink;
	RB::AnimationTimer	m_TimerInvulnerableStatus;
	bool m_IsInvulnerable = false;


private:// cached component's pointer
	RB::TransformComponent* m_TsC = nullptr;
	RB::SpriteComponent* m_SpriteC = nullptr;
	RB::MoveComponent* m_MoveC = nullptr;
	GroundComponent* m_Gc = nullptr;
	RB::InputComponent* m_InputC = nullptr;
	RollComponent* m_RollC = nullptr;
	JumpComponent* m_JumpC = nullptr;
	CharacterBaseComponent* m_CharBaseC = nullptr;
	RB::CollisionComponent* m_CollisionC = nullptr;
	RB::VisibilityControlComponent* m_VisbC = nullptr;
	RB::StateMachineComponent<PlayerState>* m_FsmC = nullptr;
};


