#pragma once 
#include <cstdint>

enum class EnemyStates : uint8_t {
	Idle, 
	Run,
	Attack,
	Aim,
	Jump,
	AirDash,
	GroundDash,
	Dead
};

