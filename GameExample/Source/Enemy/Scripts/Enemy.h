#pragma once  
#include <Rubber.h>  
#include "Enemy/EnemyStates.h"

class Enemy final: public RB::ScriptableEntity {  
public:  
	void onCreate() override;  
	void onUpdate(float ts) override;
	void onDestory() override {};
	~Enemy() override {};

	void animationPoolInit();
	void statesTransitionInit();
	void onIdleEnter();
	void onDeadEnter();
	void onGroundDashEnter();
	void onJumpEnter();
	void onAirDashEnter();
	void onAimEnter();

	void onJumpUpdate(float ts);
	void onAirDashUpdate(float ts);
	void faceDirectionUpdate(RB::Entity ent, float ts);

private:
	void switchState(EnemyStates toState);
	glm::vec3 getDirecitonTo(RB::Entity src, RB::Entity dst);
	void makeInvulnerable();
	float decisionTimeBasedOnHP();
private:

	// state timer
	RB::AnimationTimer m_AimStateTimer;
	RB::AnimationTimer m_IdleTimer;
	RB::AnimationTimer m_GroundDashTimer;
	RB::AnimationTimer m_InvulnerableTimer;


	float m_RunningSpeed = 2.0f;
	float m_JumpSpeed = 3.5f;
	float m_DashInAirSpeed = 8.0f;
	float m_DashOnGround = 8.0f;
	bool m_IsVulnerable = false;

private:
	CharacterBaseComponent* m_CharBase = nullptr;
};
