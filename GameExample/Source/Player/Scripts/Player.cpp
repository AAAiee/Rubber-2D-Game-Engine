#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>

#include "Player.h"
#include "Player/PlayerKeyBindings.h"
#include "ECS/GameComponents/GameComponents.h"
#include "Arena/ArenaConfig.h"


void Player::onCreate()
{
	m_CharBaseC = &getComponent<CharacterBaseComponent>();
	m_TsC = &getComponent<RB::TransformComponent>();
	m_InputC = &getComponent<RB::InputComponent>();
	m_Gc = &getComponent<GroundComponent>();
	m_MoveC = &getComponent<RB::MoveComponent>();
	m_JumpC = &getComponent<JumpComponent>();
	m_CollisionC = &getComponent<RB::CollisionComponent>();
	m_VisbC = &getComponent<RB::VisibilityControlComponent>();
	m_FsmC = &getComponent<RB::StateMachineComponent<PlayerState>>();

	// Initial position
	m_TsC->position = glm::vec3{ -3.0f, ARENA_GROUND_Y, -0.4f };

	//Customized keyBindings
	m_InputC->setKeyBindings(PlayerKeyBindings);

	//Init ground component
	m_Gc->groundY = ARENA_GROUND_Y;
	m_Gc->onGround = true;

	// Move Component
	m_MoveC->gravity = -9.8f;
	m_MoveC->moveEnabled = true;
	m_MoveC->enableGravity = true ;

	// Jump component
	m_JumpC->isJumpping = false;
	m_JumpC->jumpImpulseSpeed = m_jumpImpulsSpeed;

	//Collision box
	m_CollisionC->enabled = true;
	m_CollisionC->srcLayer = RB::CollisionLayer::Player;
	m_CollisionC->targetLayer = RB::CollisionLayer::None;
	m_CollisionC->onCollision = [this](RB::Entity ent) {
			decreaseHP(ent);
		};

	//Visibility control component
	m_VisbC->isVisible = true;

	//State Machine Init
	stateMachineInit();

	//Animation Pool Init
	animationPoolInit();

	//Roll CD Timer Init
	auto& rollC = getComponent<RollComponent>();
	rollC.isRollCdComplete = true;
	rollC.isRolling = false;
	rollC.isRollCdComplete = true;
	rollC.timerRollCD.setWaitTime(2.0f);
	rollC.timerRollCD.runOnlyOnce(true);
	rollC.timerRollCD.setOnTimeOut([&]() {
		    rollC.isRollCdComplete = true;
		});

	//Attack CD timer config
	auto& attackC = getComponent<AttackComponent>();
	attackC.isAttackCDComplete = true;
	attackC.isAttacking = false;
	attackC.timerAttackCD.setWaitTime(0.5f);
	attackC.timerAttackCD.runOnlyOnce(true);
	attackC.timerAttackCD.setOnTimeOut([&]() {
			attackC.isAttackCDComplete = true;
		});

	//Invincible timer config:  1s invincible when hit(blinking visual effect)
	m_TimerInvulnerableBlink.runOnlyOnce(false);
	m_TimerInvulnerableBlink.setWaitTime(0.075f);
	m_TimerInvulnerableBlink.setOnTimeOut([this]() {
		m_VisbC->isVisible = !m_VisbC->isVisible;
		});
	m_TimerInvulnerableBlink.Pause();

	// Invulnerable /blinking total duration
	m_TimerInvulnerableStatus.runOnlyOnce(true);
	m_TimerInvulnerableStatus.setWaitTime(1.0f);
	m_TimerInvulnerableStatus.setOnTimeOut([this]() {
		m_IsInvulnerable = false;
		m_VisbC->isVisible = true;
		});
	
	// Player's Initial State
	m_FsmC->currentState = PlayerState::Idle;
	m_CharBaseC->setClip(m_Entity,"Right_Idle");
	m_CharBaseC->faceDirection = +1;
}

void Player::onUpdate(float ts)
{
	float movingDirX = (float)m_InputC->actionState["Move_Right"] - (float)m_InputC->actionState["Move_Left"];
	if (movingDirX != 0.0f) {
		m_CharBaseC->lastFaceDirection = m_CharBaseC->faceDirection;
		m_CharBaseC->faceDirection = (movingDirX > 0.0f) ? +1 : -1;
	}

	if (m_InputC->actionState["Attack"]) {
		m_AttackDir = getAttackDir({m_InputC->mouseXWorld, m_InputC->mouseYWorld});
	}

	m_TimerInvulnerableBlink.onUpdate(ts);
	m_TimerInvulnerableStatus.onUpdate(ts);
}


void Player::onDestory()
{

}


Player::~Player() {


}

void Player::animationPoolInit()
{
	/************************AnimationPoolSetUp************************/
	//Idle
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_Right_Idle");
		config.frameDuration = 0.5f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		RB::AnimationClipConfig& rightIdleConfig = m_CharBaseC->m_AnimationPool["Right_Idle"];
		rightIdleConfig = config;

		RB::AnimationClipConfig& leftIdleConfig = m_CharBaseC->m_AnimationPool["Left_Idle"];
		config.isFlipped = true;
		leftIdleConfig = config;
	}

	// running 
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_Right_Run");
		config.frameDuration = 0.2f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		RB::AnimationClipConfig& rightRunConfig = m_CharBaseC->m_AnimationPool["Right_Run"];
		rightRunConfig = config;

		RB::AnimationClipConfig& leftRunConfig = m_CharBaseC->m_AnimationPool["Left_Run"];
		config.isFlipped = true;
		leftRunConfig = config;
	}


	//jump
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_Right_Jump");
		config.frameDuration = 0.75f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		RB::AnimationClipConfig& rightJumpConfig = m_CharBaseC->m_AnimationPool["Right_Jump"];
		rightJumpConfig = config;

		RB::AnimationClipConfig& leftJumpConfig = m_CharBaseC->m_AnimationPool["Left_Jump"];
		config.isFlipped = true;
		leftJumpConfig = config;
	}


	//fall
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_Right_Fall");
		config.frameDuration = 0.75f;
		config.isFlipped = false;
		config.loop = true;
		config.onFinished = nullptr;

		RB::AnimationClipConfig&  RightFallConfig = m_CharBaseC->m_AnimationPool ["Right_Fall"];
		RightFallConfig = config;

		RB::AnimationClipConfig& leftFallConfig = m_CharBaseC->m_AnimationPool ["Left_Fall"];
		config.isFlipped = true;
		leftFallConfig = config;
	}

	//Roll
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_Right_Roll");
		config.frameDuration = 0.05f;
		config.isFlipped = false;
		config.loop = false;
		config.onFinished = [this]() {
			auto& rollC = getComponent<RollComponent>();
			auto& collisionC = getComponent<RB::CollisionComponent>();
			rollC.isRolling = false;
			m_IsInvulnerable = false;
			};

		RB::AnimationClipConfig& RightRollConfig = m_CharBaseC->m_AnimationPool["Right_Roll"];
		RightRollConfig = config;

		RB::AnimationClipConfig& leftRollConfig = m_CharBaseC->m_AnimationPool["Left_Roll"];
		config.isFlipped = true;
		leftRollConfig = config;
	 }

	//Attack
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_Right_Attack");
		config.frameDuration = 0.05f;
		config.isFlipped = false;
		config.loop = false;
		config.onFinished = [this]() {
			auto& attackC = getComponent<AttackComponent>(); 
			attackC.isAttacking = false;
			};

		RB::AnimationClipConfig& RightAttackConfig = m_CharBaseC->m_AnimationPool["Right_Attack"];
		RightAttackConfig = config;

		RB::AnimationClipConfig& leftAttackConfig = m_CharBaseC->m_AnimationPool["Left_Attack"];
		config.isFlipped = true;
		leftAttackConfig = config;
	}


	// dead
	{
		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_Right_Dead");
		config.frameDuration = 0.25f;
		config.isFlipped = false;
		config.loop = false;
		config.onFinished = nullptr;

		RB::AnimationClipConfig& RightAttackConfig = m_CharBaseC->m_AnimationPool["Right_Dead"];
		RightAttackConfig = config;

		RB::AnimationClipConfig& leftAttackConfig = m_CharBaseC->m_AnimationPool["Left_Dead"];
		config.isFlipped = true;
		leftAttackConfig = config;
	}
}


AttackDir Player::getAttackDir(glm::vec2 mouseCoord)
{
	glm::vec2 playerPos = { m_TsC->position.x, m_TsC->position.y };
	float anglesInRad = glm::atan(mouseCoord.y - playerPos.y, mouseCoord.x - playerPos.x);

	constexpr float PI = 3.1415936f;

	if (anglesInRad >= -PI / 4.0f && anglesInRad < PI / 4.0f) {
		return AttackDir::RIGHT;
	}
	
	if(anglesInRad >= PI / 4.0f && anglesInRad < 3 * PI / 4.0f){
		return AttackDir::UP;
	}
    
	if (anglesInRad >= 3.0f * PI / 4.0f && anglesInRad < 5 * PI / 4.0f) {
		return AttackDir::LEFT;
	}
	
	return AttackDir::DOWN;
}



void Player::stateMachineInit()
{
	auto& fsmCC = getComponent<RB::StateMachineComponent<PlayerState>>();

	/************************StateMachineSetUp************************/
	fsmCC.transitions = {
		{PlayerState::Idle, PlayerState::Run, [](RB::Entity e) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& inputC = e.getComponent<RB::InputComponent>();
			return  gC.onGround && (inputC.actionState["Move_Left"] ^ inputC.actionState["Move_Right"]);
		}},

		{PlayerState::Idle, PlayerState::Roll, [](RB::Entity e) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& rollC = e.getComponent<RollComponent>();
			
			bool canRoll = rollC.isRollCdComplete && gC.onGround;
			return canRoll && inputC.actionState["Roll"];
		}},
			
		{PlayerState::Idle, PlayerState::Attack, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackComponent = e.getComponent<AttackComponent>();
			bool canAttack = attackComponent.isAttackCDComplete && attackComponent.isAttacking == false;
			return inputC.actionState["Attack"] && canAttack;
		}},

		{PlayerState::Idle, PlayerState::Jump, [](RB::Entity e ) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& inputC = e.getComponent<RB::InputComponent>();
			return gC.onGround && inputC.actionState["Jump"];
		}},

		{PlayerState::Idle, PlayerState::Dead, [](RB::Entity e) {
			auto& hpC = e.getComponent<HPComponent>();
			return hpC.currentHealth <= 0;
		}},

		{PlayerState::Run, PlayerState::Idle, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& gC = e.getComponent<GroundComponent>();
			// netZero is true if both keys are the same (both up or both down)
			bool netZero = inputC.actionState["Move_Left"]
				== inputC.actionState["Move_Right"];
			return gC.onGround && netZero;
		}},

		{PlayerState::Run, PlayerState::Roll, [](RB::Entity e) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& rollC = e.getComponent<RollComponent>();

			bool canRoll = rollC.isRollCdComplete && gC.onGround;
			return canRoll && inputC.actionState["Roll"];
		}},

		{PlayerState::Run, PlayerState::Jump, [](RB::Entity e ) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& gC = e.getComponent<GroundComponent>();
			return inputC.actionState["Jump"];
		}},

		{PlayerState::Run, PlayerState::Attack, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackComponent = e.getComponent<AttackComponent>();
			bool canAttack = attackComponent.isAttackCDComplete && attackComponent.isAttacking == false;
			return inputC.actionState["Attack"] && canAttack;
		}},

		{PlayerState::Run, PlayerState::Dead, [](RB::Entity e) {
			auto& hpC = e.getComponent<HPComponent>();
			return hpC.currentHealth <= 0;
		}},

		{PlayerState::Jump, PlayerState::Fall, [](RB::Entity e) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& mvC = e.getComponent<RB::MoveComponent>();
			return !gC.onGround && mvC.velocity.y <= 0.0f;
		}},

		{PlayerState::Jump, PlayerState::Attack, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackComponent = e.getComponent<AttackComponent>();
			bool canAttack = attackComponent.isAttackCDComplete && attackComponent.isAttacking == false;
			return inputC.actionState["Attack"] && canAttack;
		}},

		{PlayerState::Jump, PlayerState::Dead, [](RB::Entity e) {
			auto& hpC = e.getComponent<HPComponent>();
			return hpC.currentHealth <= 0;
		}},

		{PlayerState::Fall, PlayerState::Idle, [](RB::Entity e) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& inputC = e.getComponent<RB::InputComponent>();
			return  gC.onGround && !inputC.actionState["Move_Left"] && !inputC.actionState["Move_Right"];
		}},

		{PlayerState::Fall, PlayerState::Run, [](RB::Entity e) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& inputC = e.getComponent<RB::InputComponent>();
			return  gC.onGround && (inputC.actionState["Move_Left"] || inputC.actionState["Move_Right"]);
		}},

		{PlayerState::Fall, PlayerState::Roll, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& gC = e.getComponent<GroundComponent>();
			return gC.onGround && inputC.actionState["Roll"];
		}},

		{PlayerState::Fall, PlayerState::Attack, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackComponent = e.getComponent<AttackComponent>();
			bool canAttack = attackComponent.isAttackCDComplete && attackComponent.isAttacking == false;
			return inputC.actionState["Attack"] && canAttack;
		}},

		{ PlayerState::Fall, PlayerState::Dead, [](RB::Entity e) {
			auto& hpC = e.getComponent<HPComponent>();
			return hpC.currentHealth <= 0;
		}},

		{PlayerState::Roll, PlayerState::Idle, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& rollC = e.getComponent<RollComponent>();

			bool netZero = inputC.actionState["Move_Left"]
				== inputC.actionState["Move_Right"];
			return rollC.isRolling == false && netZero;
		}} ,

		{PlayerState::Roll, PlayerState::Run, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& rollC = e.getComponent<RollComponent>();
			return rollC.isRolling == false && inputC.actionState["Move_Left"] ^ inputC.actionState["Move_Right"];
		}},


		{PlayerState::Roll, PlayerState::Jump, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& rollC = e.getComponent<RollComponent>();
			return rollC.isRolling == false && inputC.actionState["Jump"];
		}},

		{ PlayerState::Roll, PlayerState::Attack, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& rollC = e.getComponent<RollComponent>();
			auto& attackComponent = e.getComponent<AttackComponent>();

			bool canAttack = attackComponent.isAttackCDComplete && attackComponent.isAttacking == false;
			return rollC.isRolling == false && inputC.actionState["Attack"]&& canAttack;
		}},

		{ PlayerState::Attack, PlayerState::Idle, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackC  = e.getComponent<AttackComponent>();
			auto& gC = e.getComponent<GroundComponent>();
			bool netZero = inputC.actionState["Move_Left"]
				== inputC.actionState["Move_Right"];
			return  !attackC.isAttacking && netZero && gC.onGround;
		}},

		{ PlayerState::Attack, PlayerState::Run, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackC = e.getComponent<AttackComponent>();
			auto& gC = e.getComponent<GroundComponent>();
			bool moveIntent = inputC.actionState["Move_Left"]
				^ inputC.actionState["Move_Right"];
			return  !attackC.isAttacking && moveIntent && gC.onGround;
		}},

		{ PlayerState::Attack, PlayerState::Jump, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackC = e.getComponent<AttackComponent>();
			auto& gC = e.getComponent<GroundComponent>();
			return  inputC.actionState["Jump"] && gC.onGround;
		}},

		{ PlayerState::Attack, PlayerState::Fall, [](RB::Entity e) {
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& attackC = e.getComponent<AttackComponent>();
			auto& gC = e.getComponent<GroundComponent>();
			auto& mvC = e.getComponent<RB::MoveComponent>();
			return  !attackC.isAttacking && mvC.velocity.y <= 0.0f && !gC.onGround;
		}},

		{ PlayerState::Attack, PlayerState::Dead, [](RB::Entity e) {
			auto& hpC = e.getComponent<HPComponent>();
			return hpC.currentHealth <= 0.0f; 
		}},

		{ PlayerState::Attack, PlayerState::Roll, [](RB::Entity e) {
			auto& gC = e.getComponent<GroundComponent>();
			auto& inputC = e.getComponent<RB::InputComponent>();
			auto& rollC = e.getComponent<RollComponent>();
			auto& attackC = e.getComponent<AttackComponent>();

			bool canRoll = rollC.isRollCdComplete && gC.onGround;
			return  canRoll && inputC.actionState["Roll"];
		}},
	};
	

	fsmCC.onEnter = [this](entt::entity e, PlayerState newState) {
		switch (newState)
		{
		case PlayerState::Idle:
			onIdleEnter();
			break;
		case PlayerState::Run:
			onRunEnter();
			break;
		case PlayerState::Jump:
			onJumpEnter();
			break;
		case PlayerState::Fall:
			onFallEnter();
			break;
		case PlayerState::Roll:
			onRollEnter();
			break;
		case PlayerState::Attack:
			onAttackEnter();
			break;
		case PlayerState::Dead:
			onDeadEnter();
			break;
		}
	};
		

	fsmCC.onExit = [this](entt::entity e, PlayerState currentState) {
		switch (currentState)
		{
		case PlayerState::Idle:
			onIdleExit();
			break;
		case PlayerState::Run:
			onRunExit();
			break;
		case PlayerState::Jump:
			onJumpExit();
			break;
		case PlayerState::Fall:
			onFallExit();
			break;
		case PlayerState::Roll:
			onRollExit();
			break;
		case PlayerState::Attack:
			onAttackExit();
			break;
		}
	}; 


	fsmCC.onUpdate = ([this](RB::Entity e, PlayerState state,  float ts) {

		auto& fsmCC = e.getComponent<RB::StateMachineComponent<PlayerState>>();
		auto& animC = e.getComponent<RB::AnimationComponent>();
		auto& rollC = e.getComponent<RollComponent>();
		auto& attackC = e.getComponent<AttackComponent>();
		auto& hpC = e.getComponent<HPComponent>();

		if (hpC.currentHealth <= 0.0f) return;

		// update player's facing direction
		if (m_CharBaseC->faceDirection != m_CharBaseC->lastFaceDirection) {
			m_CharBaseC->lastFaceDirection = m_CharBaseC->faceDirection;
			if (fsmCC.currentState != PlayerState::Roll) {
				animC.specs.m_IsOpposizeDireciton = (m_CharBaseC->faceDirection < 0);
				m_MoveC->velocity.x = -m_MoveC->velocity.x;
			}
		}

		// update VFX attack 's position to align with player's position
		auto& playerChilds = getComponent<PlayerChilds>();
		auto& vAttackTs = playerChilds.attack.getComponent<RB::TransformComponent>();

		// add a little bit offset to player's attack entity's collision box to feel more natural
		switch (m_AttackDir)
		{
		case AttackDir::DOWN:
			vAttackTs.position = m_TsC->position + glm::vec3{ 0.0f,  -0.25f, 0.0f };
			break;
		case AttackDir::LEFT:
			vAttackTs.position = m_TsC->position + glm::vec3{ -0.25f, 0.0f, 0.0f };
			break;
		case AttackDir::RIGHT:
			vAttackTs.position = m_TsC->position + glm::vec3{ 0.25f, 0.0f, 0.0f };
			break;
		case AttackDir::UP:
			vAttackTs.position = m_TsC->position + glm::vec3{ 0.0f,  0.25f, 0.0f };
			break;
		case AttackDir::NONE:
			vAttackTs.position = m_TsC->position;
			break;
		}

		// CD timer update
		rollC.timerRollCD.onUpdate(ts);
		attackC.timerAttackCD.onUpdate(ts);
	});

}


void Player::onIdleEnter() {
	m_MoveC->velocity = { 0.0f, 0.0f };
	m_CharBaseC->updateClipConfigBasedOnFaceDir(m_Entity,"Idle");
}

void Player::onRunEnter() {
	m_CharBaseC->updateClipConfigBasedOnFaceDir(m_Entity,"Run");
	m_MoveC->velocity.x = m_RunningSpeed * m_CharBaseC->faceDirection;
}

void Player::onJumpEnter() {
	m_MoveC->velocity.y = m_JumpC->jumpImpulseSpeed;
	m_MoveC->velocity.x = m_RunningSpeed * m_CharBaseC->faceDirection;
	m_JumpC->isJumpping = true;
	
	m_CharBaseC->updateClipConfigBasedOnFaceDir(m_Entity,"Jump");

	{ // VFX Jump 
		auto& childEntitiesC = getComponent<PlayerChilds>();
		RB::Entity  vfxJumpEntity = childEntitiesC.vfxJump;

		auto& visbC = vfxJumpEntity.getComponent<RB::VisibilityControlComponent>();
		auto& vfxJumpTs = vfxJumpEntity.getComponent<RB::TransformComponent>();
		auto& animeCC = vfxJumpEntity.getComponent<RB::AnimationComponent>();

		visbC.isVisible = true;
		animeCC.specs.m_Timer.restart();
		animeCC.specs.m_FrameIndex = 0;
		vfxJumpTs.position.x = m_TsC->position.x;
		vfxJumpTs.position.y = m_TsC->position.y;
		vfxJumpTs.position.z = m_TsC->position.z + 0.05f;
	}
}


void Player::onAttackEnter()
{

	auto& playerChilds = getComponent<PlayerChilds>();

	auto& attackC = getComponent<AttackComponent>();
	bool& isVisble = playerChilds.attack.getComponent<RB::VisibilityControlComponent>().isVisible;

	attackC.timerAttackCD.restart();
	attackC.isAttacking = true;
	attackC.isAttackCDComplete = false;
	isVisble = true;

	//player's animation update
	if (m_AttackDir == AttackDir::LEFT) {
		m_CharBaseC->faceDirection = -1;
	}
	else if(m_AttackDir == AttackDir::RIGHT) {
		m_CharBaseC->faceDirection = +1;
	}
	m_CharBaseC->updateClipConfigBasedOnFaceDir(m_Entity,"Attack");

	//VFX effect
	auto& vAnimeCC = playerChilds.attack.getComponent<RB::AnimationComponent>();
	auto& vAttackAnimeC = playerChilds.attack.getComponent<AttackAnimaitonConfigComponent>();
	auto& parentTsC = getComponent<RB::TransformComponent>();
	auto& vfxTsC = playerChilds.attack.getComponent<RB::TransformComponent>();

	switch (m_AttackDir)
	{
	case AttackDir::DOWN:
		vAnimeCC.specs.setSpec(vAttackAnimeC.donwAttack);
		break;
	case AttackDir::LEFT:
		vAnimeCC.specs.setSpec(vAttackAnimeC.leftAttack);
		break;
	case AttackDir::RIGHT:
		vAnimeCC.specs.setSpec(vAttackAnimeC.rightAttack);
		break;
	case AttackDir::UP:
		vAnimeCC.specs.setSpec(vAttackAnimeC.upAttack);
		break;
	}

	auto& collisionCC = playerChilds.attack.getComponent<RB::CollisionComponent >();
	collisionCC.enabled = true;
} 


void Player::onFallEnter(){

	m_CharBaseC->updateClipConfigBasedOnFaceDir(m_Entity,	"Fall");
}

void Player::onRollEnter()
{
	auto& rollC = getComponent<RollComponent>();
	rollC.timerRollCD.restart();
	rollC.isRolling = true;
	rollC.isRollCdComplete = false;

	m_IsInvulnerable = true; // only makes it invulnerable, no blinking

	m_CharBaseC->updateClipConfigBasedOnFaceDir(m_Entity,"Roll");

	m_MoveC->velocity.x = m_RollImpulseSpeed * m_CharBaseC->faceDirection;
}

void Player::onDeadEnter()
{
	auto& mvC = getComponent<RB::MoveComponent>();
	mvC.velocity = { 0.0f, 0.0f };
	m_CharBaseC->updateClipConfigBasedOnFaceDir(m_Entity,"Dead");
}

void Player::onJumpExit()
{
	m_JumpC->isJumpping = false;
}

void Player::onFallExit()
{
	auto& childEntitiesC = getComponent<PlayerChilds>();
	RB::Entity  vfxLandEntity = childEntitiesC.vfxland;


	auto& gc = getComponent<GroundComponent>();
	if (!gc.onGround) {
		return;
	}

	auto& visbC = vfxLandEntity.getComponent<RB::VisibilityControlComponent>();
	auto& animeCC = vfxLandEntity.getComponent<RB::AnimationComponent>();
	auto& vfxLandTs = vfxLandEntity.getComponent<RB::TransformComponent>();

	animeCC.specs.m_FrameIndex = 0;
	animeCC.specs.m_Timer.restart();
	visbC.isVisible = true;
	vfxLandTs.position.x = m_TsC->position.x;
	vfxLandTs.position.y = m_TsC->position.y;
	vfxLandTs.position.z = m_TsC->position.z + 0.05f;
}

void Player::onAttackExit()
{
	auto& attackC = getComponent<AttackComponent>();
	auto& playerChildsC = getComponent<PlayerChilds>();

	attackC.isAttacking = false;

	RB::Entity attackEntity = playerChildsC.attack;
	auto& collisionC = attackEntity.getComponent<RB::CollisionComponent>();
	collisionC.enabled = false;
}

void Player::decreaseHP(RB::Entity ent)
{
	auto& hpC = ent.getComponent<HPComponent>();
	if (m_IsInvulnerable) {
		return;
	}

	if (hpC.currentHealth >= 10) {
		hpC.currentHealth -= 5.f;
		makeInvulnerable();
	}
	else {
		hpC.currentHealth = 0;
	}

	RB_INFO("Current Health is: {}", hpC.currentHealth);

}

void Player::makeInvulnerable()
{
	m_IsInvulnerable = true;
	m_TimerInvulnerableStatus.restart();
	m_TimerInvulnerableBlink.restart();
	m_TimerInvulnerableBlink.resume();
}

