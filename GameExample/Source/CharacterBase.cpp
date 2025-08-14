#include "CharacterBase.h"


void CharacterBase::setClip(RB::Entity& e,  std::string_view animeName)
{
	auto& animeCC = e.getComponent<RB::AnimationComponent>();
	auto it = m_AnimationPool.find(animeName);
	RB_CORE_ASSERT(it != m_AnimationPool.end(), "Not registered animation");
	RB::AnimationClipConfig& config = it->second;
	animeCC.specs.setSpec(config);
}


void CharacterBase::updateClipConfigBasedOnFaceDir(RB::Entity& e, std::string_view animeName)
{
	std::string key;
	if (m_FaceDirection == +1) {
		key.append("Right_").append(animeName);
		setClip(e, key);
	}
	else if (m_FaceDirection == -1) {
		key.append("Left_").append(animeName);
		setClip(e,key);
	}
}
