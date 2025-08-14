#include "PlayerAttack.h"
#include "ECS/GameComponents/GameComponents.h"
#include "player/PlayerAttackDir.h"
#include "entt.hpp"

void PlayerAttack::onCreate()
{
	//Attack animation Init
	{
		auto& atkClips = getComponent<AttackAnimaitonConfigComponent>();

		RB::AnimationClipConfig config;
		config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
		config.clipHandle = RB::AssetHandle("Player_VAttackLeft");
		config.frameDuration = 0.12f;
		config.isFlipped = false;
		config.loop = false;
		config.onFinished = [this]() {
			auto& visbC = getComponent<RB::VisibilityControlComponent>();
			visbC.isVisible = false;
		};
		atkClips.leftAttack = config;

		config.clipHandle = RB::AssetHandle("Player_VAttackRight");
		atkClips.rightAttack = config;

		config.clipHandle = RB::AssetHandle("Player_VAttackUp");
		atkClips.upAttack = config;

		config.clipHandle = RB::AssetHandle("Player_VAttackDown");
		atkClips.donwAttack = config;
	}

	//collision box
	RB::CollisionBoxOnRenderFlag& flag = getComponent<RB::CollisionBoxOnRenderFlag>();
	flag.onDebugRender = true;

	RB::CollisionComponent& CC = getComponent<RB::CollisionComponent>();
	CC.enabled = false;
	CC.srcLayer = RB::CollisionLayer::None; 
	CC.targetLayer = RB::CollisionLayer::Enemy; 
	CC.onCollision = nullptr; 
}

