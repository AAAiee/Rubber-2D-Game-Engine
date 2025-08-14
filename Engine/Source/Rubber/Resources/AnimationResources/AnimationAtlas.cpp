#include <pch.h>
#include "AnimationAtlas.h"
#include "Rubber/Renderer/Texture.h"
#include "Rubber/Renderer/Subtexture2D.h"
#include "Rubber/Resources/AssetManager.h"


namespace Rubber {

	void AnimationAtlas::addFrame(AssetHandle& handler)
	{
		m_Frames.push_back(handler);

	}

	const Ref<Texture2D> AnimationAtlas::getTextureAtIndex(uint32_t index) const
	{
		RB_CORE_ASSERT(index < m_Frames.size(), "Out of Range!");
		
		AssetHandle handle =  m_Frames[index];
		auto& asset = m_Manager->getAsset(handle);
		return  static_pointer_cast<Texture2D> (asset);
	}

}
