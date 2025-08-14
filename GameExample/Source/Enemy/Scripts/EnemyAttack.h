#pragma once

#include "Rubber.h"
#include <cstdint>


class EnemyAttack : public RB::ScriptableEntity {

public:
	void onCreate() override;
	void onUpdate(float ts) override {};
	void onDestory() override {};
};

