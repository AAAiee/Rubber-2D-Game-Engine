#include <entt.hpp>

#include "Ecs/GameComponents/GameComponents.h"
#include "PlayerJumpVFX.h"

void PlayerVFXJump::onCreate()
{
	auto& animationC = getComponent<RB::AnimationComponent>();

	// init animation component
	RB::AnimationClipConfig config;
	config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
	config.clipHandle = RB::AssetHandle("Player_VJump");
	config.frameDuration = 0.05f;
	config.isFlipped = false;
	config.loop = false;
	config.onFinished = [&]() {
			auto& visbC = getComponent<RB::VisibilityControlComponent>();
			visbC.isVisible = false;
		};
	animationC.specs.setSpec(config);
}

