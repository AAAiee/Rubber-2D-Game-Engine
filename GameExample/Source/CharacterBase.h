#pragma once
#include <Rubber.h>
#include <unordered_map>
#include <string>
#include <entt.hpp>

static float inline CHAR_DEFAULT_GROUND_Y = -3.55f;

struct CharacterBase {

	void setClip(RB::Entity& e, std::string_view animeName);

	void updateClipConfigBasedOnFaceDir( RB::Entity& e, std::string_view animeName);


	std::unordered_map<std::string, RB::AnimationClipConfig, RB::stringHash, std::equal_to<>>  m_AnimationPool;

	int m_FaceDirection = +1;
	float m_JumpImpulseSpeed = 5.0f;

};
