#include "pch.h"
#include "AnimationSpec.h"
#include "Rubber/Core/Application.h"
#include "Rubber/Resources/AssetManager.h"
#include "Rubber/Resources/AssetMetaData.h"


namespace Rubber {

	AnimationSpec::AnimationSpec(const AnimationClipConfig& config)
		:m_ActiveAnimationClip(config.clipHandle),
		m_FrameIndex(0),
		m_IsLoop(config.loop),
		m_OnFinished(config.onFinished),
		m_IsOpposizeDireciton(config.isFlipped),
		m_AnchorMode(config.anchorMode)
	{
		m_Timer.setWaitTime(config.frameDuration);
		Ref<AssetManager> manager = Application::getAssetManager();
		const AssetMetaData& metadata = manager->getAssetMetadata(config.clipHandle);
		RB_CORE_ASSERT(metadata.type == AssetType::AnimationSpriteSheet || metadata.type == AssetType::AnimationAtlas, "wrong type");
		const AnimationResourcesMetaData& animationMetaData = static_cast<const AnimationResourcesMetaData&> (metadata);
		m_FrameCount = animationMetaData.frameCount;
	}

    void AnimationSpec::setSpec(const AnimationClipConfig& config)  
    {  
       m_ActiveAnimationClip = config.clipHandle;  
       m_FrameIndex = 0;  
       m_IsLoop = config.loop;  
       m_OnFinished = config.onFinished;  
       m_IsOpposizeDireciton = config.isFlipped;  
       m_AnchorMode = config.anchorMode;  

	   m_Timer.setWaitTime(config.frameDuration);
	   Ref<AssetManager> manager = Application::getAssetManager();
	   const AssetMetaData& metadata = manager->getAssetMetadata(config.clipHandle);
	   RB_CORE_ASSERT(metadata.type == AssetType::AnimationSpriteSheet || metadata.type == AssetType::AnimationAtlas, "wrong type");
	   const AnimationResourcesMetaData& animationMetaData = static_cast<const AnimationResourcesMetaData&> (metadata);
	   m_FrameCount = animationMetaData.frameCount;
    }

}



