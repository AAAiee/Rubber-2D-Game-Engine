#pragma once

#include "Rubber.h"
#include <cstdint>
#include "Player/PlayerAttackDir.h"

class PlayerAttack : public RB::ScriptableEntity {

public:
	void onCreate() override;
	void onUpdate(float ts) override {};
	void onDestory() override {};
private:
	RB::AnimationClipConfig m_LeftAttack;
	RB::AnimationClipConfig m_RightAttack;
	RB::AnimationClipConfig m_UpAttack;
	RB::AnimationClipConfig m_DonwAttack;
};

