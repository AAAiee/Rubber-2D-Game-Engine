#include "Enemy.h"
#include <ECS/GameComponents/GameComponents.h>
#include "Enemy/EnemyStates.h"
#include "Arena/ArenaConfig.h"

void Enemy::onCreate()
{
	auto& gC = getComponent<GroundComponent>();
	gC.groundY = ARENA_GROUND_Y;

	// initial position
	auto& tsC = getComponent<RB::TransformComponent>();
	tsC.position = glm::vec3{ 3.0f, ARENA_GROUND_Y, -0.4f };
	gC.onGround = true;

	// Move Component
	auto& mvC = getComponent<RB::MoveComponent>();
	mvC.gravity = -9.8f;
	mvC.moveEnabled = true;
	mvC.enableGravity = true;

	//jump Component
	auto& jumpC = getComponent<JumpComponent>();
	jumpC.jumpImpulseSpeed = m_JumpSpeed;
	jumpC.isJumpping = false;

	//hp Component
	auto& hpC = getComponent<HPComponent>();
	hpC.currentHealth = 100.0f;
	
	//collision box
	auto& collisionC = getComponent<RB::CollisionComponent>();
	collisionC.enabled = true;
	collisionC.srcLayer = RB::CollisionLayer::Enemy;
	collisionC.targetLayer = RB::CollisionLayer::None;
	collisionC.size = { 0.9f, 0.9f };
	collisionC.onCollision = [&](RB::Entity ent){
		if (m_IsVulnerable) {
			RB_INFO("Enemy is hit,but invulnerable : {}", hpC.currentHealth);
			return;
		}

		auto& hpC = ent.getComponent<HPComponent>();
		if (hpC.currentHealth >= 10.0f) {
			hpC.currentHealth -= 10.0f;
			makeInvulnerable();
		}
		else {
			hpC.currentHealth = 0.0f;
		}

		RB_INFO("Enemy is hit, current health : {}", hpC.currentHealth);
		};

	//Visibility control component
	auto& visibilityCC = getComponent<RB::VisibilityControlComponent>();
	visibilityCC.isVisible = true;

	//state
	statesTransitionInit();

	//Animation Pool Init
	animationPoolInit();

	//aim timer
	m_AimStateTimer.runOnlyOnce(true);
	m_AimStateTimer.setWaitTime(0.5f); 
	m_AimStateTimer.setOnTimeOut([&]() {
			auto& mvC = getComponent<RB::MoveComponent>();
			auto& gC = getComponent<GroundComponent>();
			float randomFactor = RB::RandomEngine::Float() * 100.f;
			if (!gC.onGround) {
				switchState(EnemyStates::AirDash);
			}
			else {
				switchState(EnemyStates::GroundDash);
			}
		});
	
	//idle Timer
	m_IdleTimer.runOnlyOnce(true);
	m_IdleTimer.setWaitTime(2.0f);
	m_IdleTimer.setOnTimeOut([&]() {
			// set up next move!
			float randomFactor = RB::RandomEngine::Float() * 100.0f;
			if (randomFactor <= 50.f) {
				switchState(EnemyStates::Jump);
			}
			else {
				switchState(EnemyStates::Aim);
			}
		});

	//ground dash timer
	m_GroundDashTimer.runOnlyOnce(true);
	m_GroundDashTimer.setWaitTime(0.75f);
	m_GroundDashTimer.setOnTimeOut([&]() {
		auto& attackCollision = getComponent<ChildEntityComponent>().Child.getComponent<RB::CollisionComponent>();
		attackCollision.enabled = false;
			switchState(EnemyStates::Idle);
		});

	//invulnerable timer
	m_InvulnerableTimer.runOnlyOnce(true);
	m_InvulnerableTimer.setWaitTime(0.5f);
	m_InvulnerableTimer.setOnTimeOut([&]() {
		m_IsVulnerable = false;
		});

	// Enemy's Initial State
	switchState(EnemyStates::Idle);
	m_CharBase.setClip(m_Entity, "Left_Idle");
	m_CharBase.m_FaceDirection = -1;
}

void Enemy::onUpdate(float ts)
{
	auto& tsC = getComponent<RB::TransformComponent>();
	auto& child = getComponent<ChildEntityComponent>();
	child.Child.getComponent<RB::TransformComponent>().position = tsC.position;
	faceDirectionUpdate(m_Entity, ts);
	m_InvulnerableTimer.onUpdate(ts);

	m_IdleTimer.setWaitTime(decisionTimeBasedOnHP());
}

void Enemy::statesTransitionInit()
{
	auto& fsmC = getComponent<RB::StateMachineComponent<EnemyStates>>();

	fsmC.onEnter = [this](entt::entity ent, EnemyStates state) {
		switch (state)
		{
		case EnemyStates::Idle:
			onIdleEnter();
			break;
		case EnemyStates::Aim:
			onAimEnter();
			break;
		case EnemyStates::Jump:
			onJumpEnter();
			break;
		case EnemyStates::AirDash:
			onAirDashEnter();
			break;
		case EnemyStates::GroundDash:
			onGroundDashEnter();
			break;
		case EnemyStates::Dead:
			onDeadEnter();
			break;
		}
	};

	fsmC.onUpdate = [this](RB::Entity ent,EnemyStates state , float ts) {
			auto& hpC = getComponent<HPComponent>();
			if (hpC.currentHealth <= 0.0f)
				switchState(EnemyStates::Dead);

			switch (state)
			{
			case EnemyStates::Idle:
				m_IdleTimer.onUpdate(ts);
				break;
			case EnemyStates::Aim:
				m_AimStateTimer.onUpdate(ts);
				break;
			case EnemyStates::Jump:
				onJumpUpdate(ts);
				break;
			case EnemyStates::AirDash:
				onAirDashUpdate(ts);
				break;
			case EnemyStates::GroundDash:
				m_GroundDashTimer.onUpdate(ts);
				break;
			default:
				break;
			}

		};
}

/****************************States Enter CallBacks**************************/
void Enemy::onIdleEnter()
{
	auto& mvC = getComponent<RB::MoveComponent>();
	mvC.velocity = { 0.0f, 0.0f };

	mvC.enableGravity = true; 
	m_CharBase.updateClipConfigBasedOnFaceDir(m_Entity, "Idle");

	m_IdleTimer.restart();
 }

void Enemy::onDeadEnter() {

	auto& mvC = getComponent<RB::MoveComponent>();
	mvC.velocity = { 0.0f, 0.0f };
	mvC.enableGravity = true;

}


void Enemy::onJumpEnter()
{
	auto& mvC = getComponent<RB::MoveComponent>();
	auto& jumpC = getComponent<JumpComponent>();
	mvC.enableGravity = false;

	// add a jump in air speed
	jumpC.isJumpping = true;
	mvC.velocity.y = jumpC.jumpImpulseSpeed;
	m_CharBase.updateClipConfigBasedOnFaceDir(m_Entity, "Jump");
}

void Enemy::onAimEnter()
{
	auto& mvC = getComponent<RB::MoveComponent>();
	mvC.enableGravity = false;
	mvC.velocity = { 0.0f, 0.0f };
	
	m_AimStateTimer.restart();
	m_CharBase.updateClipConfigBasedOnFaceDir(m_Entity, "Aim");

}

void Enemy::onAirDashEnter()
{
	auto& player = getComponent<EntityReferenceComponent>().Entity; 
	auto& mvC = getComponent<RB::MoveComponent>();
	auto& collisionC = getComponent<RB::CollisionComponent>();
	auto& attackCollision = getComponent<ChildEntityComponent>().Child.getComponent<RB::CollisionComponent>();
	attackCollision.enabled = true;
	
	mvC.enableGravity = false;
	glm::vec3 dir = getDirecitonTo(m_Entity, player);
	
	mvC.velocity.x = dir.x * m_DashInAirSpeed;
	mvC.velocity.y = dir.y * m_DashInAirSpeed;
	m_CharBase.updateClipConfigBasedOnFaceDir(m_Entity, "AirDash");
}


void Enemy::onGroundDashEnter()
{
	auto& player = getComponent<EntityReferenceComponent>().Entity;
	auto& mvC = getComponent<RB::MoveComponent>();
	auto& attackCollision = getComponent<ChildEntityComponent>().Child.getComponent<RB::CollisionComponent>();
	attackCollision.enabled = true;

	m_GroundDashTimer.restart();

	mvC.enableGravity = false;
	glm::vec3 dir = getDirecitonTo(m_Entity, player);
	
	float dashDir = (dir.x > 0.0f) ? 1.0f : -1.0f;

	mvC.velocity.x = dashDir * m_DashOnGround;
	m_CharBase.updateClipConfigBasedOnFaceDir(m_Entity, "DashOnFloor");
}


/****************************States Updates CallBacks**************************/
void Enemy::onAirDashUpdate(float ts) 
{
	auto& gC = getComponent<GroundComponent>();
	
	if (gC.onGround ) {
		switchState(EnemyStates::Idle);
		auto& attackCollision = getComponent<ChildEntityComponent>().Child.getComponent<RB::CollisionComponent>();
		attackCollision.enabled = false;
	}
}

void Enemy::onJumpUpdate(float ts)
{
	auto& jumpC = getComponent<JumpComponent>();
	if (jumpC.isJumpping) {
		return;
	}

	float randomNum = RB::RandomEngine::Float() * 100.f;
	if (randomNum <= 100.0f) {
		switchState(EnemyStates::Aim);
	}
}


/****************************Utilities**************************/
void Enemy::faceDirectionUpdate(RB::Entity ent, float ts)
{

	auto& fsmC = ent.getComponent<RB::StateMachineComponent<EnemyStates>>();
	if (fsmC.currentState == EnemyStates::AirDash) return;
	if (fsmC.currentState == EnemyStates::GroundDash) return;

	auto& player = ent.getComponent<EntityReferenceComponent>().Entity;
	auto& animeC = ent.getComponent<RB::AnimationComponent>();
	auto& mvC = getComponent<RB::MoveComponent>();

	glm::vec3 enemyPos = ent.getComponent<RB::TransformComponent>().position;
	glm::vec3 playerPosition = player.getComponent<RB::TransformComponent>().position;

	int shouldFaceTo = (enemyPos.x - playerPosition.x > 0) ? -1 : 1;
	if (shouldFaceTo != m_CharBase.m_FaceDirection) {
		m_CharBase.m_FaceDirection = shouldFaceTo;
		animeC.specs.m_IsOpposizeDireciton = (m_CharBase.m_FaceDirection > 0);
		mvC.velocity.x = -mvC.velocity.x;
	}
}

void Enemy::switchState(EnemyStates toState)
{
	auto& fsmC = getComponent<RB::StateMachineComponent<EnemyStates>>();
	fsmC.currentState = toState;
	fsmC.justEntered = true;
}

glm::vec3 Enemy::getDirecitonTo(RB::Entity src, RB::Entity dst)
{

	glm::vec3& srcCoord = src.getComponent<RB::TransformComponent>().position;
	glm::vec3& dstCoord = dst.getComponent<RB::TransformComponent>().position;

	glm::vec3 direction = glm::normalize(dstCoord - srcCoord);
	return direction;
}


void Enemy::makeInvulnerable()
{
	m_InvulnerableTimer.restart();
	m_IsVulnerable = true;
}

float Enemy::decisionTimeBasedOnHP()
{
	auto& hpC = getComponent<HPComponent>();
	return glm::clamp(2.0f * hpC.currentHealth / 100.0f, 0.75f, 2.0f);
}

void Enemy::animationPoolInit()
{
	/************************AnimationPoolSetUp************************/
	//Idle
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Enemy_Left_Idle");
		config.frameDuration = 0.25f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		auto& leftIdle = m_CharBase.m_AnimationPool["Left_Idle"];
		leftIdle = config;

		auto& rightIdle = m_CharBase.m_AnimationPool["Right_Idle"];
		config.isFlipped = true;
		rightIdle = config;
	}

	// Run
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Enemy_Left_Run");
		config.frameDuration = 0.1f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		auto& leftRun = m_CharBase.m_AnimationPool["Left_Run"];
		leftRun = config;

		auto& rightRun = m_CharBase.m_AnimationPool["Right_Run"];
		config.isFlipped = true;
		rightRun = config;
	}

	// Aim
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Enemy_Left_Aim");
		config.frameDuration = 0.10f;
		config.isFlipped = false;
		config.loop = false;
		config.onFinished = nullptr;

		auto& leftAim = m_CharBase.m_AnimationPool["Left_Aim"];
		leftAim = config;

		auto& rightAim = m_CharBase.m_AnimationPool["Right_Aim"];
		config.isFlipped = true;
		rightAim = config;
	}

	// Jump
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Enemy_Left_Jump");
		config.frameDuration = 0.15f;
		config.isFlipped = false;
		config.loop = false;
		config.onFinished = [this]() {
			auto& jumpC = getComponent<JumpComponent>();
			jumpC.isJumpping = false;

			};

		auto& leftAim = m_CharBase.m_AnimationPool["Left_Jump"];
		leftAim = config;

		auto& rightAim = m_CharBase.m_AnimationPool["Right_Jump"];
		config.isFlipped = true;
		rightAim = config;
	}


	//Air Dash
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Enemy_Left_DashInAir");
		config.frameDuration = 0.50f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		auto& leftAim = m_CharBase.m_AnimationPool["Left_AirDash"];
		leftAim = config;

		auto& rightAim = m_CharBase.m_AnimationPool["Right_AirDash"];
		config.isFlipped = true;
		rightAim = config;
	}



	//groundDash
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Enemy_Left_DashOnFloor");
		config.frameDuration = 0.50f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		auto& leftAim = m_CharBase.m_AnimationPool["Left_DashOnFloor"];
		leftAim = config;

		auto& rightAim = m_CharBase.m_AnimationPool["Right_DashOnFloor"];
		config.isFlipped = true;
		rightAim = config;
	}

	//Dead 
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Enemy_Left_Dead");
		config.frameDuration = 0.50f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		auto& leftAim = m_CharBase.m_AnimationPool["Left_Dead"];
		leftAim = config;

		auto& rightAim = m_CharBase.m_AnimationPool["Right_Dead"];
		config.isFlipped = true;
		rightAim = config;
	}


}

