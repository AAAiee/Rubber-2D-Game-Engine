#include <imgui.h>
#include "GameLayer.h"
#include "AssetMetaDataList.h"

#include "Player/PlayerKeyBindings.h" 
#include "Player/Scripts/PlayerVFX/PlayerJumpVFX.h"
#include "Player/Scripts/PlayerVFX/PlayerVFXLand.h"
#include "Player/Scripts/PlayerVFX/PlayerAttack.h"
#include "player/PlayerState.h"
#include "Player/Scripts/Player.h"

#include "Enemy/Scripts/Enemy.h"
#include "Enemy/EnemyStates.h"
#include "Enemy/Scripts/EnemyAttack.h"

#include "ECS/GameComponents/GameComponents.h"
#include "ECS/GameSystems/GroundLandingSystem.h"



void GameLayer::onAttach()
{

	const auto& assetManager = RB::Application::getAssetManager();
	LoadAllAsset(assetManager);

	// background texture  
	RB::AssetHandle handle = { "BackGround" };
	m_Asset = assetManager->getAsset(handle);
	const auto& backGroundTexture = static_pointer_cast<RB::Texture2D>(m_Asset);
	RB::Ref<RB::SubTexture2D> BackGroundSub = RB::SubTexture2D::createFromTexture(backGroundTexture, false);

	// scene creation  
	m_ActiveScene = RB::Scene::create();
	m_ActiveScene->addSystem(RB::makeScope<RB::WindowSystem>());
	m_ActiveScene->addSystem(RB::makeScope<RB::InputSystem>());
	m_ActiveScene->addSystem(RB::makeScope<RB::CameraSystem>());
	m_ActiveScene->addSystem(RB::makeScope<RB::ScriptSystem>());
	m_ActiveScene->addSystem(RB::makeScope<RB::FsmSystem<PlayerState>>());
	m_ActiveScene->addSystem(RB::makeScope<RB::FsmSystem<EnemyStates>>());
	m_ActiveScene->addSystem(RB::makeScope<RB::MoveSystem>());
	m_ActiveScene->addSystem(RB::makeScope<RB::CollisionSystem>());
	m_ActiveScene->addSystem(RB::makeScope<GroundLandingSystem>());
	m_ActiveScene->addSystem(RB::makeScope<RB::FsmPostMovingSystem<PlayerState>>());
	m_ActiveScene->addSystem(RB::makeScope<RB::FsmPostMovingSystem<EnemyStates>>());
	m_ActiveScene->addSystem(RB::makeScope<RB::AnimaitonSystem>());
	m_ActiveScene->addSystem(RB::makeScope<RB::RendererSystem>());
	m_ActiveScene->systemsInit();


	RB::Entity cameraEntity = m_ActiveScene->createEntity("Camera Entity");
	cameraEntity.addComponent<RB::TransformComponent>(glm::vec3{ 0.0f, 0.0f, 0.0f });
	cameraEntity.addComponent<RB::CameraComponent>();
	cameraEntity.getComponent<RB::CameraComponent>().isPrimary = true;
	cameraEntity.getComponent<RB::CameraComponent>().isFixedAspectRatio = false;

	m_Player = m_ActiveScene->createEntity("Player Entity");
	m_Player.addComponent<RB::TransformComponent>();
	m_Player.addComponent<RB::SpriteComponent>();
	m_Player.addComponent<RB::AnimationComponent>();
	m_Player.addComponent<RB::StateMachineComponent<PlayerState>>();
	m_Player.addComponent<RB::InputComponent>();
	m_Player.addComponent<CharacterBaseComponent>();
	m_Player.addComponent<RB::CollisionComponent>();
	m_Player.addComponent<RB::CollisionBoxOnRenderFlag>(true);
	m_Player.addComponent<RB::NativeScriptComponent>();
	m_Player.addComponent<RB::MoveComponent>();
	m_Player.addComponent<RollComponent>();
	m_Player.addComponent<JumpComponent>();
	m_Player.addComponent<AttackComponent>();
	m_Player.addComponent<GroundComponent>();
	m_Player.addComponent<RB::VisibilityControlComponent>(true);
	m_Player.addComponent<PlayerChilds>();
	m_Player.addComponent <HPComponent>();
	m_Player.getComponent<RB::NativeScriptComponent>().bind<Player>();

	//background
	RB::Entity backGroundEntity = m_ActiveScene->createEntity("BackGround Entity");
	backGroundEntity.addComponent<RB::TransformComponent>(glm::vec3{ 0.0f, 0.0f, -0.5f }, glm::vec3{ 17.8f, 10.0f, 0.0f }, glm::vec3(0.0f));
	backGroundEntity.addComponent<RB::SpriteComponent>(BackGroundSub);
	backGroundEntity.addComponent<RB::VisibilityControlComponent>(true);

	//VFX jump
	RB::Entity vfxJump = m_ActiveScene->createEntity("Player Jump VFX");
	vfxJump.addComponent<RB::TransformComponent>();
	vfxJump.addComponent<RB::SpriteComponent>();
	vfxJump.addComponent<RB::AnimationComponent>();
	vfxJump.addComponent<RB::NativeScriptComponent>();
	vfxJump.addComponent<RB::VisibilityControlComponent>(false);
	vfxJump.getComponent<RB::NativeScriptComponent>().bind<PlayerVFXJump>();

	//VFX Land
	RB::Entity vfxLand = m_ActiveScene->createEntity("Player Jump VFX");
	vfxLand.addComponent<RB::TransformComponent>();
	vfxLand.addComponent<RB::SpriteComponent>();
	vfxLand.addComponent<RB::AnimationComponent>();
	vfxLand.addComponent<RB::NativeScriptComponent>();
	vfxLand.addComponent<RB::VisibilityControlComponent>(false);
	vfxLand.getComponent<RB::NativeScriptComponent>().bind<PlayerVFXLand>();

	//VFX Attack
	RB::Entity playerAttack = m_ActiveScene->createEntity("Player Attack");
	playerAttack.addComponent<RB::TransformComponent>();
	playerAttack.addComponent<RB::SpriteComponent>();
	playerAttack.addComponent<RB::AnimationComponent>();
	playerAttack.addComponent<RB::NativeScriptComponent>();
	playerAttack.addComponent<AttackAnimaitonConfigComponent>();
	playerAttack.addComponent<RB::VisibilityControlComponent>(false);
	playerAttack.addComponent<RB::CollisionComponent>();
	playerAttack.addComponent<ParentEntityComponent>(m_Player);
	playerAttack.addComponent<RB::CollisionBoxOnRenderFlag>(true);
	playerAttack.getComponent<RB::NativeScriptComponent>().bind<PlayerAttack>();

	//Enemy
	m_Enemy = m_ActiveScene->createEntity("Enemy");
	m_Enemy.addComponent<RB::TransformComponent>();
	m_Enemy.addComponent<RB::SpriteComponent>();
	m_Enemy.addComponent<RB::AnimationComponent>();
	m_Enemy.addComponent<RB::StateMachineComponent<EnemyStates>>();
	m_Enemy.addComponent<RB::CollisionComponent>();
	m_Enemy.addComponent<RB::CollisionBoxOnRenderFlag>(true);
	m_Enemy.addComponent<RB::MoveComponent>();
	m_Enemy.addComponent<JumpComponent>();
	m_Enemy.addComponent<CharacterBaseComponent>();
	m_Enemy.addComponent<RB::NativeScriptComponent>();
	m_Enemy.addComponent<GroundComponent>();
	m_Enemy.addComponent<RB::VisibilityControlComponent>(true);
	m_Enemy.addComponent<EntityReferenceComponent>(m_Player);
	m_Enemy.getComponent<RB::NativeScriptComponent>().bind<Enemy>();
	m_Enemy.addComponent<StatsComponent>();
	m_Enemy.addComponent<ChildEntityComponent>();
	m_Enemy.addComponent<HPComponent>();
 

	RB::Entity enemyAttack = m_ActiveScene->createEntity("Enemy Weapon");
	enemyAttack.addComponent<RB::TransformComponent>();
	enemyAttack.addComponent<RB::CollisionComponent>();
	enemyAttack.addComponent<RB::SpriteComponent>();
	enemyAttack.addComponent<RB::CollisionBoxOnRenderFlag>(true);
	enemyAttack.addComponent<RB::NativeScriptComponent>();
	enemyAttack.addComponent <ParentEntityComponent>();
	enemyAttack.addComponent<RB::VisibilityControlComponent>(true);
	enemyAttack.getComponent<RB::NativeScriptComponent>().bind<EnemyAttack>();
	
	// add parent-child relationship
	auto& childs = m_Player.getComponent<PlayerChilds>();
	childs.vfxJump = vfxJump;
	childs.vfxland = vfxLand;
	childs.attack = playerAttack;

	auto& enemyChild = m_Enemy.getComponent<ChildEntityComponent>();
	enemyChild.Child = enemyAttack;
	auto& enemyAttackParent = enemyAttack.getComponent<ParentEntityComponent>();
	enemyAttackParent.parent = m_Enemy;
}

void GameLayer::onDetach()
{

	m_ActiveScene->systemShutDow();
}

void GameLayer::onUpdate(float ts)
{

	m_ActiveScene->onSystemsUpdate(ts);
}

void GameLayer::onImGuiRender()
{
	// the game engine currently does not support rendering text so here uses dear imgui instead
	ImGui::Begin("PLayer Health");
	auto& playerHpC = m_Player.getComponent<HPComponent>();
	ImGui::TextColored({ 1.0f, 0.0f, 0.0f,1.0f }, "Player Health: %.2f", playerHpC.currentHealth);
	ImGui::End();

	ImGui::Begin("Enemy Health");
	auto& enemyHpC = m_Enemy.getComponent<HPComponent>();
	ImGui::TextColored({ 1.0f,0.0f, 0.0f,1.0f }, "Enemy Health: %.2f", enemyHpC.currentHealth);
	ImGui::End();

}

void GameLayer::LoadAllAsset(const RB::Ref<RB::AssetManager>& assetManager)
{
	for (const auto& metadata : s_Atlas)
	{
		assetManager->loadAsset(metadata);
	}

	for (const auto& metadata : s_SpriteSheet)
	{
		assetManager->loadAsset(metadata);
	}

	for (const auto& metadata : s_Texture2DImage)  
	{
		assetManager->loadAsset(metadata);
	}
}
