#pragma once
#include <cstdint>


enum class PlayerState : uint8_t {
	Idle = 0,
	Run,
	Jump,
	Fall,
	Roll,
	Attack,
	Dead
};

