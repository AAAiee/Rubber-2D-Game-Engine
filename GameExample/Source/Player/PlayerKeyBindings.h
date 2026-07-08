#pragma once
#include <RB.h>

static inline RB::Vector<RB::KeyBinding> PlayerKeyBindings = {
	{"Move_Left" , RB_KEY_A },
	{"Move_Right", RB_KEY_D},
	{"Jump",  RB_KEY_W} ,
	{"Roll", RB_KEY_LEFT_SHIFT},
	{"Attack", RB_MOUSE_BUTTON_1}
};
