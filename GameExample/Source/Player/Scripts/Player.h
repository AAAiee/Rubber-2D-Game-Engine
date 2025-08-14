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
	void setClip(std::string_view animeName);
	void updateClipConfig(std::string_view animeName);
	void stateMachineInit();
	void animationPoolInit();
	AttackDir getAttackDir(glm::vec2 mouseCoord); 

private:
	int m_LastFaceDireciton = +1;
	int m_FaceDirection = +1;

	const float m_Ground = -3.55f; 
	const float m_RunningSpeed = 2.0f;
	const float m_jumpImpulsSpeed = 6.0f;
	const float m_RollImpulseSpeed = 4.0f;

	const float m_LeftBound = -4.0f;
	const float m_RightBound = 4.0f;

	AttackDir m_AttackDir = AttackDir::NONE;

	//invisible status
	RB::AnimationTimer  m_TimerInvulnerableBlink;
	RB::AnimationTimer	m_TimerInvulnerableStatus;
	bool m_IsInvulnerable = false;

	std::unordered_map<std::string, RB::AnimationClipConfig, RB::stringHash, std::equal_to<>>  m_AnimationPool;

private:// cached component's pointer
	RB::TransformComponent* tsC = nullptr;
	RB::SpriteComponent* spC = nullptr;
	RB::MoveComponent* mvC = nullptr;
	GroundComponent* gC = nullptr;
	RB::InputComponent* inputC = nullptr;
	RollComponent* rollC = nullptr;
	JumpComponent* jumpC = nullptr;
};


