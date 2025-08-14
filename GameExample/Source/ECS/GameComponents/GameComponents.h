#pragma once
#include <Rubber.h>
#include <Player\PlayerAttackDir.h>

//*******************HPComponent**************************//
struct HPComponent {
	float  currentHealth = 100.0f;
};

//*******************HPComponent**************************//
struct GroundComponent {
	float groundY = 0.0f;
	float leftBound = -8.35f;
	float rightBound = 8.35f;  
	bool onGround = true;
};

// move related components
struct RollComponent {
	RB::AnimationTimer timerRollCD;
	bool isRolling = false;
	bool isRollCdComplete = true;
};

struct JumpComponent {
	float jumpImpulseSpeed = 0.0f;
	bool isJumpping = false;
};


struct PlayerChilds {
	RB::Entity vfxJump;
	RB::Entity vfxland;
	RB::Entity attack;
};

struct AttackIntent {
	AttackDir dir;
};

struct AttackComponent {
	RB::AnimationTimer timerAttackCD;
	bool isAttacking = false;
	bool isAttackCDComplete = true;
};

struct AttackAnimaitonConfigComponent {
	RB::AnimationClipConfig leftAttack;
	RB::AnimationClipConfig rightAttack;
	RB::AnimationClipConfig upAttack;
	RB::AnimationClipConfig donwAttack;
};

struct ParentEntityComponent {
	RB::Entity parent;
};

struct ChildEntityComponent {
	RB::Entity Child;
};

struct EntityReferenceComponent {
	RB::Entity Entity;
};

struct StatsComponent {
	bool isAttacking = false;
	bool isRunning =false;
	bool isDashingOnGround = false;
	bool isDashingInAir = false ;
};







