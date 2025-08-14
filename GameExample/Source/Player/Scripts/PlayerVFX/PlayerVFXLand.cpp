#include "PlayerVFXLand.h"



void PlayerVFXLand::onCreate()
{
	auto& animationC = getComponent<RB::AnimationComponent>();

	// init animation component
	RB::AnimationClipConfig config;
	config.anchorMode = RB::AnimationSpec::AnchorMode::BOTTOMCENTERED;
	config.clipHandle = RB::AssetHandle("Player_VLand");
	config.frameDuration = 0.07f;
	config.isFlipped = false;
	config.loop = false;
	config.onFinished = [&]() {
			auto& visbC = getComponent<RB::VisibilityControlComponent>();
			visbC.isVisible = false;
		};
	animationC.specs.setSpec(config);
}

void PlayerVFXLand::onUpdate(float ts)
{

}

void PlayerVFXLand::onDestory()
{

}
