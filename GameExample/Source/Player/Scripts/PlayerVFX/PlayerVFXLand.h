#pragma once

#include "Rubber.h"


class PlayerVFXLand : public RB::ScriptableEntity {
public:
	void onCreate() override;
	void onUpdate(float ts) override;;
	void onDestory() override;;
};


